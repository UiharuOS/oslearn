#ifndef __THREAD_THREAD_H
#define __THREAD_THREAD_H
#include "stdint.h"

/* 自定义通用函数类型, 在线程函数中作为行参原型 */
typedef void thread_func(void*);

/* 进程或线程状态 */
enum task_status {
    TASK_RUNNING,
    TASK_READY,
    TASK_BLOCK,
    TASK_WAITING,
    TASK_HANGING,
    TASK_DIED
};

/************ 中断栈intr_stack ***********
 *  此结构用于中断发生时保护程序(线程或进程)的上下文环境:
 *  进程或线程被外部中断或软中断打断时, 会按照此结构压入上下文
 *  寄存器, inir_exit中的出栈操作是此结构的逆操作
 *  此栈在线程自己的内核栈中位置固定, 所在页的最顶端
 */
struct intr_stack {
    uint32_t vec_no;        // kernel.S 宏VECTOR中push %1压入的中断号
    // ...未完待续, 先去解决同步、互斥问题....
}
