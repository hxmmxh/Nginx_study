# 可变参数列表和va_list

### 在函数中使用省略符形参
* void foo ( parm_list , …); 
* 省略号前面必须有至少一个参数
* 对于与显示声明的形参相对应的实参进行类型检查，而对于与省略符对应的实参则暂停类型检查。形参声明后面的逗号是可选的。
* 省略号的优先级别最低，所以在函数解析时，只有当其它所有的函数都无法调用时，编译器才会考虑调用省略号函数

### va_list的使用方法
1. 首先在函数中定义一个具有va_list型的变量，这个变量是指向参数的指针。
2. 然后用va_start宏初始化变量刚定义的va_list变量，使其指向第一个可变参数的地址。
3. 然后va_arg返回可变参数，va_arg的第二个参数是你要返回的参数的类型（如果多个可变参数，依次调用va_arg获取各个参数）。
4. 最后使用va_end宏结束可变参数的获取。
5. 如果不需要一一详解每个参数，只需要将可变列表拷贝到某个缓冲区，可以用vsprintf函数。
6. 函数的参数一般从右至左先后入栈，根据栈的特性，也就是，最左边的参数最先出栈


### 函数接口
```c
#include <stdarg.h>

typedef struct {
        char *a0;       /* pointer to first homed integer argument */
        int offset;     /* byte offset of next parameter */
} va_list;

#define _INTSIZEOF(n) ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))
// 获取类型占用的空间长度，最小占用长度为int的整数倍

#define va_start(ap, v) (ap = (va_list)&v + _INTSIZEOF(v))
//获取可变参数列表的第一个参数的地址（ap是类型为va_list的指针，v是可变参数最左边的参数

#define va_arg(ap, t) (*(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))
//获取可变参数的当前参数，返回指定类型并将指针指向下一参数（t参数描述了当前参数的类型）
//在循环中可用0!=va_arg()作为判断条件

#define va_end(ap) (ap = (va_list)0)
//清空va_list可变参数列表

 vsprintf(char *buffer, char *format, va_list param);
// 格式化输出到串中
// 根据参数format字符串来转换并格式化数据, 然后将结果复制到buffer字符串数组
```