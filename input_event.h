#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H

#include <stdbool.h>
#include <linux/input.h>
#include <linux/limits.h>

/**************************************************
 * @brief 读取一个输入事件
 * @param[in] fd 事件文件的文件描述符
 * @param[out] event 返回事件
 * @return 是否成功
 * ***********************************************/
bool input_event_read(int fd, struct input_event* event);

/**************************************************
 * @brief 打印一个输入事件
 * @param[in] event 要打印的输入事件
 * ***********************************************/
void input_event_print(struct input_event* event);

/**************************************************
 * @brief 将输入事件的类型码转换为字符串
 *        参考：https://www.kernel.org/doc/Documentation/input/event-codes.txt
 * @param[in] t 类型码
 * @return 类型名称
 * ***********************************************/
const char* input_event_type_string(int t);

/**************************************************
 * @brief 将输入事件的code换为字符串
 *        参考：https://www.kernel.org/doc/Documentation/input/event-codes.txt
 * @param[in] t 事件的类型码
 * @param[in] code 事件的code
 * @return 类型名称
 * ***********************************************/
const char* input_event_code_string(int t, int code);

#endif // INPUT_EVENT_H