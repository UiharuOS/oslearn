# oslearn: uiharu os study repo

+ -> 知识主要来源:《操作系统真相还原》
+ -> 总结

<details><summary>目录</summary>
<!-- toc -->

+ [**BIOS**](https://github.com/neo1218/neox/tree/master/ext/bios)
    - [BIOS到底在哪里](https://github.com/neo1218/neox/tree/master/ext/bios#bios到底在哪里)
    - [计算机的启动过程](https://github.com/neo1218/neox/tree/master/ext/bios#计算机的启动过程)
    - [魔数](https://github.com/neo1218/neox/tree/master/ext/bios#魔数)
        - [0x55, 0xaa](https://github.com/neo1218/neox/tree/master/ext/bios#0x55-0xaa)
        - [0x7c00](https://github.com/neo1218/neox/tree/master/ext/bios#0x7c00)
+ [**Address: 地址**](https://github.com/neo1218/neox/tree/master/ext/address)
    - [偏移量](https://github.com/neo1218/neox/tree/master/ext/address#地址)
    - [section](https://github.com/neo1218/neox/tree/master/ext/address#section)
    - [vstart](https://github.com/neo1218/neox/tree/master/ext/address#vstart)
+ [**CPU**](https://github.com/neo1218/neox/tree/master/ext/cpu)
    - [基础工作原理](https://github.com/neo1218/neox/tree/master/ext/cpu#cpu基础工作原理)
    - ~~~~~~~[💗 CPU实模式开始❤️ ](https://github.com/neo1218/neox/tree/master/ext/cpu#实模式)~~~~~~
    - [实模式下的寄存器](https://github.com/neo1218/neox/tree/master/ext/cpu#实模式下的寄存器)
        - [程序员"可见"寄存器](https://github.com/neo1218/neox/tree/master/ext/cpu#程序员可见寄存器)
        - [程序员"不可见"寄存器](https://github.com/neo1218/neox/tree/master/ext/cpu#程序员不可见寄存器)
    - [实模式下的内存分段](https://github.com/neo1218/neox/tree/master/ext/cpu#实模式下的内存分段)
    - [实模式下CPU内存寻址方式](https://github.com/neo1218/neox/tree/master/ext/cpu#实模式下cpu内存寻址方式)
    - [到底什么是栈?](https://github.com/neo1218/neox/tree/master/ext/cpu#到底什么是栈)
    - [16位实模式下的ret调用](https://github.com/neo1218/neox/tree/master/ext/cpu#实模式下的ret)
    - [16位实模式下的call调用](https://github.com/neo1218/neox/tree/master/ext/cpu#实模式下的call)
        - [call相对近调用](https://github.com/neo1218/neox/tree/master/ext/cpu#16位实模式相对近调用)
        - [call间接绝对近调用](https://github.com/neo1218/neox/tree/master/ext/cpu#16位实模式间接绝对近调用)
        - [call直接绝对远调用](https://github.com/neo1218/neox/tree/master/ext/cpu#16位实模式直接绝对远调用)
        - [call间接绝对远调用](https://github.com/neo1218/neox/tree/master/ext/cpu#16位实模式间接绝对远调用)
    - [16位实模式下的jmp调用](https://github.com/neo1218/neox/tree/master/ext/cpu#实模式下的jmp)
        - [jmp相对短转移](https://github.com/neo1218/neox/tree/master/ext/cpu#16位实模式相对短调用)
        - [jmp相对近转移](https://github.com/neo1218/neox/tree/master/ext/cpu#16位实模式相对近转移)
        - [jmp间接绝对近转移](https://github.com/neo1218/neox/tree/master/ext/cpu#16位实模式间接绝对近转移)
        - [jmp直接绝对远转移](https://github.com/neo1218/neox/tree/master/ext/cpu#16位实模式直接绝对远转移)
        - [jmp间接绝对远转移](https://github.com/neo1218/neox/tree/master/ext/cpu#16位实模式间接绝对远转移)
    - [标志寄存器flags](https://github.com/neo1218/neox/tree/master/ext/cpu#标志寄存器flags)
    - [有条件转移jxx](https://github.com/neo1218/neox/tree/master/ext/cpu#有条件转移)
    - ~~~~~~[❤️ CPU实模式结束💗 ](https://github.com/neo1218/neox/tree/master/ext/cpu#实模式小结)~~~~~~~
+ [**MRR**]()
+ [**Job Schedule**](https://github.com/neo1218/neox/tree/master/ext/job)
+ [**Thread**](https://github.com/neo1218/neox/tree/master/ext/thread)
    - [线程概述]()
    - [进程控制表PCB]()
    - [实现线程的方式]()
        - [用户进程线程]()
        - [内核进程线程]()

<!-- tocstop -->
</details>
