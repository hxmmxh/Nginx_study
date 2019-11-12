# Nginx_study






## 编写说明

* 在实现log模块之前，简单用printf语句把信息输入到一个txt文件中

## 1.定义基本数据结构(1)

### 1.1 封装整形
* intptr_t长度总是所在平台的位数，32位系统为4字节，64位系统为8字节
* [ngy_int](ngy_int.h)




## 2. 实现一个简易log系统
* 先编写一个简易的日志输入模块，实现基本接口，但简化功能，之后再编写完整的模块
* 实现接口为void log_s(char *fmt, ...);把日志消息输入到标准输出



## 3. 实现内存池
* 在nginx的源码中内存池的定义和实现在core/ngx_palloc.c和core/ngx_palloc.h中，调用的系统函数位于os/ngx_alloc.h和os/ngx_alloc.c中
* 目标: 使模块开发者只用关心内存的分配，内存池会解决内存的释放

### 源码设计思路

### 代码编写
1. 编写在ngy_alloc.h和ngy_alloc.c中 

2. 实现四个管理内存分配与释放的基础函数，
    * ngy_alloc，分配内存 
    * ngy_calloc，分配内存并把内存清0
    * ngx_free， 释放内存
    * ngx_memalign, 内存对齐单位分配内存

3. 涉及的系统函数有malloc, realloc, free, 用法见[alloc](./系统函数介绍/alloc.md)

4. 
