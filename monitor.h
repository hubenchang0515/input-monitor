#ifndef MONITOR_H
#define MONITOR_H

#include <stdbool.h>

typedef struct monitor_t monitor_t;

/**********************************************************
 * @brief 过滤器函数指针
 * @param[in] filename 文件名，不含路径
 * @param[in] userdata 用户数据
 * @return 是否需要被过滤
 * *******************************************************/
typedef bool (*filter_t)(const char* filename, void* userdata);

/**********************************************************
 * @brief 触发器器函数指针
 * @param[in] fd 激活的文件描述符
 * @param[in] userdata 用户数据
 * @return 是否成功
 * *******************************************************/
typedef bool (*trigger_t)(int fd, void* userdata);


/**********************************************************
 * @brief 创建一个监视器，监视指定文件夹下的文件
 * @param[in] dir 文件夹路径，要监视的文件夹
 * @param[in] filter 过滤器，过滤部分不需要监视的文件
 * @param[in] userdata 传给filter的用户数据
 * @return 监视器
 * *******************************************************/
monitor_t* monitor_create(const char* dir, filter_t filter, void* userdata);

/**********************************************************
 * @brief 删除一个监视器
 * @param[in] dir 监视器
 * *******************************************************/
void monitor_delete(monitor_t* monitor);

/**********************************************************
 * @brief 运行一个监视器
 * @param[in] monitor 监视器的文件描述符
 * @param[in] trigger 触发器
 * @param[in] userdata 传给trigger的用户数据
 * *******************************************************/
bool monitor_run(monitor_t* monitor, trigger_t trigger, void* userdata);

#endif // MONITOR_H