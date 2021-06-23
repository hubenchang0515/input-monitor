#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <linux/input.h>
#include <linux/limits.h>
#include <unistd.h>
#include <fcntl.h>
#include "input_event.h"
#include "input_event_code_name.h"

/**************************************************
 * @brief 读取一个输入事件
 * @param[in] fd 事件文件的文件描述符
 * @param[out] event 返回事件
 * @return 是否成功
 * ***********************************************/
bool input_event_read(int fd, struct input_event* event)
{
    if(event == NULL)
    {
        return false;
    }

    return read(fd, event, sizeof(struct input_event)) > 0;
}

/**************************************************
 * @brief 打印一个输入事件
 * @param[in] event 要打印的输入事件
 * ***********************************************/
void input_event_print(struct input_event* event)
{
    printf("\ntype:%s(%u), \tcode:%s(%u), \n"
            "value:%d,        \ttime:%ld.%06ld\n", 
            input_event_type_string(event->type), event->type,
            input_event_code_string(event->type, event->code), event->code, 
            event->value, event->time.tv_sec, event->time.tv_usec);
}

/**************************************************
 * @brief 将输入事件的类型码转换为字符串
 *        参考：https://www.kernel.org/doc/Documentation/input/event-codes.txt
 * @param[in] t 类型码
 * @return 类型名称
 * ***********************************************/
const char* input_event_type_string(int t)
{
    switch(t)
    {
    case EV_SYN:
        return "EV_SYN"; // 同步事件
    
    case EV_KEY:
        return "EV_KEY"; // 按键事件

    case EV_REL:
        return "EV_REL"; // 相对坐标事件（鼠标移动）
    
    case EV_ABS:
        return "EV_ABS"; // 绝对坐标事件（触摸屏点击）
    
    case EV_MSC:
        return "EV_MSC"; // 其它事件

    case EV_SW:
        return "EV_SW";  // 二值开关事件

    case EV_LED:
        return "EV_LED"; // LED开关事件

    case EV_SND:
        return "EV_SND"; // 声音事件

    case EV_REP:
        return "EV_REP"; // 自动重复设备事件

    case EV_FF:
        return "EV_FF";  // 力反馈事件

    case EV_PWR:
        return "EV_PWR"; // 电源开关事件

    case EV_FF_STATUS:
        return "EV_FF_STATUS"; // 力反馈设备状态事件
    }

    return "unknown";
}


/**************************************************
 * @brief 将输入事件的code换为字符串
 *        参考：https://www.kernel.org/doc/Documentation/input/event-codes.txt
 * @param[in] t 事件的类型码
 * @param[in] code 事件的code
 * @return 类型名称
 * ***********************************************/
const char* input_event_code_string(int t, int code)
{
    static bool is_init = false;
    if(!is_init)
    {
        is_init = init_all_tables();
    }

    const char* str = NULL;
    switch(t)
    {
    case EV_SYN:
        str = EV_SYN_CODE_NAME_TABLE[code]; // 同步事件
        break;
    
    case EV_KEY:
        str = EV_KEY_CODE_NAME_TABLE[code]; // 按键事件
        break;

    case EV_REL:
        str = EV_REL_CODE_NAME_TABLE[code]; // 相对坐标事件（鼠标移动）
        break;
    
    case EV_ABS:
        str = EV_ABS_CODE_NAME_TABLE[code]; // 绝对坐标事件（触摸屏点击）
        break;
    
    case EV_MSC:
        str = EV_MSC_CODE_NAME_TABLE[code]; // 其它事件
        break;

    case EV_SW:
        str = EV_SW_CODE_NAME_TABLE[code];  // 二值开关事件
        break;

    case EV_LED:
        str = EV_LED_CODE_NAME_TABLE[code]; // LED开关事件
        break;

    case EV_SND:
        str = EV_SND_CODE_NAME_TABLE[code]; // 声音事件
        break;

    case EV_REP:
        str = EV_REP_CODE_NAME_TABLE[code]; // 自动重复设备事件
        break;
    }

    return str != NULL ? str : "";
}