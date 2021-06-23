#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/epoll.h>
#include <linux/limits.h>
#include "monitor.h"

struct monitor_t
{
    int fd;         // epoll fd
    int* fd_set;    // 监视的文件描述符
    size_t count;      // 监视的文件描述符的个数
};

/**********************************************************
 * @brief 创建一个监视器，监视指定文件夹下的文件
 * @param[in] dir 文件夹名路径，要监视的文件夹
 * @param[in] filter 过滤器，过滤部分不需要监视的文件
 * @param[in] userdata 传给filter的用户数据
 * @return 监视器文件描述符
 * *******************************************************/
monitor_t* monitor_create(const char* dir, filter_t filter, void* userdata)
{
    monitor_t* monitor = NULL;
    DIR* dir_ptr = opendir(dir);
    if(dir_ptr == NULL)
    {
        fprintf(stderr, "failed to open '%s': %s\n", dir, strerror(errno));
        goto failed0;
    }

    // 计算文件的个数
    struct dirent* entry = NULL;
    int count = 0;
    for(;(entry = readdir(dir_ptr)) != NULL;)
    {
        if(filter != NULL && filter(entry->d_name, userdata))
        {
            continue;
        }
        count++;
    }

    int epoll_fd = epoll_create(count);
    if(epoll_fd < 0)
    {
        fprintf(stderr, "failed to create epoll: %s\n", strerror(errno));
        goto failed1;
    }

    monitor = malloc(sizeof(monitor_t));
    if(monitor == NULL)
    {
        fprintf(stderr, "bad alloc: %s\n", strerror(errno));
        goto failed2;
    }
    monitor->fd = epoll_fd;
    monitor->fd_set = malloc(count * sizeof(int));
    if(monitor->fd_set == NULL)
    {
        fprintf(stderr, "bad alloc: %s\n", strerror(errno));
        goto failed3;
    }

    rewinddir(dir_ptr);
    for(count = 0; (entry = readdir(dir_ptr)) != NULL;)
    {
        if(filter != NULL && filter(entry->d_name, userdata))
        {
            continue;
        }

        char path[PATH_MAX+1];
        snprintf(path, PATH_MAX+1, "%s/%s", dir, entry->d_name);
        int fd = open(path, O_RDONLY);
        if(fd < 0)
        {
            fprintf(stderr, "failed to open '%s': %s\n", path, strerror(errno));
            continue;
        }

        struct epoll_event event;
        event.events = EPOLLIN;
        event.data.fd = fd;
        monitor->fd_set[count] = fd;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
        count++;
    }
    monitor->count = count;

    closedir(dir_ptr);
    return monitor;

failed3:
    free(monitor);
failed2:
    close(epoll_fd);
failed1:
    closedir(dir_ptr);
failed0:
    return NULL;
}

/**********************************************************
 * @brief 删除一个监视器
 * @param[in] dir 监视器
 * *******************************************************/
void monitor_delete(monitor_t* monitor)
{
    if(monitor == NULL)
        return;

    for(size_t i = 0; i < monitor->count; i++)
    {
        close(monitor->fd_set[i]);
    }

    close(monitor->fd);
    free(monitor->fd_set);
    free(monitor);
}

/**********************************************************
 * @brief 运行一个监视器
 * @param[in] monitor 监视器的文件描述符
 * @param[in] trigger 触发器
 * @param[in] userdata 传给trigger的用户数据
 * *******************************************************/
bool monitor_run(monitor_t* monitor, trigger_t trigger, void* userdata)
{
    struct epoll_event event;
    if(epoll_wait(monitor->fd, &event, 1, 100) <= 0)
    {
        return false;
    }

    if(trigger != NULL)
    {
        return trigger(event.data.fd, userdata);
    }

    return false;
}