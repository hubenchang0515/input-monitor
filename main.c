#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include "input_event.h"
#include "monitor.h"

/*******************************************************
 * @brief 判断字符串是否不含有指定的前缀
 * @param[in] str 被判断的字符串
 * @param[in] prefix 前缀
 * @return str不含prefix前缀则为true，否则为false
 * ****************************************************/
bool not_have_prefix(const char* str, const char* prefix)
{
    if(prefix == NULL)
    {
        return false;
    }

    if(str == NULL)
    {
        return true;
    }

    
    for(ptrdiff_t diff = 0;  ; diff++)
    {
        if(*(prefix + diff) == 0)
        {
            return false;
        }

        if(*(str + diff) != *(prefix + diff))
        {
            return true;
        }
        // else continue
    } 
}


/*******************************************************
 * @brief 读取input event并打印
 * @param[in] fd 事件文件描述符
 * @param[in] userdata 回调用户数据
 * @return 是否成功
 * ****************************************************/
bool print_event(int fd, void* userdata)
{
    (void)(userdata);
    struct input_event event;
    if(!input_event_read(fd, &event))
    {
        return false;
    }

    input_event_print(&event);
    return true;
}

static bool running = true;

void handle_exit_signal(int num)
{
    (void)(num);
    running = false;
}

int main()
{
    monitor_t* monitor = monitor_create("/dev/input", (filter_t)(not_have_prefix), "event");
    if(monitor == NULL)
    {
        return EXIT_FAILURE;
    }

    signal(SIGINT, handle_exit_signal);
    signal(SIGTERM, handle_exit_signal);
    signal(SIGHUP, handle_exit_signal);

    running = true;
    while(running)
        monitor_run(monitor, print_event, NULL);

    monitor_delete(monitor);
    return EXIT_SUCCESS;
}

