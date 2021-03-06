# CPU
CPU: 中央处理器, 是计算机系统的核心, 负责取值、译码、执行指令.

## CPU基础工作原理
CPU由控制单元、运算单元、存储单元组成, 而控制单元大致由指令寄存器(IR),
指令译码器(ID), 操作控制器OC组成. <br/>
CPU基本工作原理如图:

             内存
             DRAM                              CPU

           | 指令1 |         指令指针寄存器IP(存储待运行的下一条指令地址)
           | 指令2 |                 |得到待运行指令得地址
    代码段  | ...  |<-(取指令)--操作控制器OC|----------\
           | 指令n |(得到指令)->指令寄存器IR|          |
           |      |                 ^   | 控制单元   |
    数据段  | 数据1 |                 |   |          |
           | 数据2 |<-(取操作数)指令译码器ID|          |
           | .....|                                |
           | 数据n |(得操作数)->存储单元SRAM           |
                                                    |
                              运算单元<<------------/

⚠ :

+ 存储单元是指CPU内部的L1, L2缓存及寄存器, 缓存采用SRAM寄存器, 用来存储指令用到的数据
+ 运算单元只是执行部件, 没有自主意识. 由控制单元OC控制, 负责算术运算和逻辑运算
+ 指令被装载到IR中,此时CPU还不能理解01..需要译码器译码

## 实模式
实模式是指8086 CPU的**寻址方式**、**寄存器大小**、**指令用法**等,
是用来反应CPU在该环境下如何工作的概念. <br/>
原本是没有实模式的概念的, 有了保护模式以后先前CPU的工作状态就叫实模式了. <br/>

## 实模式下的寄存器
寄存器(SRAM)->[触发器](https://zh.wikipedia.org/wiki/%E8%A7%A6%E5%8F%91%E5%99%A8)实现->速度快可以匹配CPU速度->用于存储CPU相关数据、操作数、缓存

### 程序员"可见"寄存器
所谓可见, 就是程序员可以使用进行存储的寄存器. <br/>

+ 段寄存器
    - CS: 代码段寄存器: 存储代码段基址,cs:ip搭配:实模式下内存访问~段基址+段内偏移地址
    - DS: 数据段寄存器: 存储数据段基址
    - ES: 附加段寄存器: 辅助存储
    - FS: 附加段寄存器: 辅助存储
    - GS: 附加段寄存器: 辅助存储
    - SS: 栈段寄存器: 存储内存栈地址
+ 通用寄存器(8个)
    + 所谓通用, 就是这些寄存器可以用来保存任何数据和地址
    - AX: AL(低8位)+AH(高8位), EAX(在AX基础上扩展为32位)
    - BX: BL(低8位)+BH(高8位)
    - CX: CL+CH
    - DX: DL+DH
    - SI:
    - DI:
    - BP:
    - SP:

这些寄存器程序可以使用, 进行存储. <br/>
通用寄存器约定功能如下:

    ax: 累加器: 使用频度最高, 用于算术、逻辑运算, 保存与外设输入输出的数据
    bx: 基址寄存器: 常用来存储内存地址, 用此地址作为基址, 用来遍历一边内存区域
    cx: 计数器: 计数, 常用于循环指令中的循环次数
    dx: 数据寄存器: 用于存放数据, 用于保存外设控制器的端口号和地址
    si: 源变址寄存器: 常用于字符串操作中的数据源地址, 即**被传送的数据在哪里**.
        通常需要配合其他指令使用, 如批量数据传送指令族movs[bwd]
    di: 目的变址寄存器: 和si一样, 常用于字符串操作. 但di是用于数据的目的地址,
        即**数据被传送到哪里**
    sp: 栈指针寄存器: 段基址存SS, 用来指向栈顶. push和pop会修改sp的值.
    bp: 基址指针:
            访问栈的方式有2种: 第一种通过sp栈指针寄存器, 但是sp只能获取stack
            top数据.  如果希望把栈当成普通的一块内存区域访问可以采用bp基址指针寄存器,
            bp默认的段寄存器是SS, 所以可以通过SS:bp的方式把栈当作普通的内存区域访问.

### 程序员"不可见"寄存器
不可见相对于可见, 程序员无法利用这类寄存器进行存储. <br/>

+ GDTR: 全局描述符表寄存器
+ IDTR: 中断描述符表寄存器
+ LDTR: 局部描述符表寄存器
+ TR: 任务寄存器
+ IP: 指令指针寄存器
+ flags: 标志寄存器
+ DR0~7: 调试寄存器

这类寄存器做CPU运行的数据支撑, 保证CPU能长久稳定运行下去. <br/>
程序虽然不能使用这些寄存器做存储, 但是这些寄存器的初始化往往需要靠程序执行. <br/>

## 实模式下的内存分段
**为什么要内存分段**:

    8086CPU之前, 没有段机制, 程序员是直接使用物理内存地址编写代码, 显然,
    如果当前地址被其他程序占用, 哪怕只有一位, 你都需要等待这个程序执行结束.
    所以8086CPU引入了分段机制

**段基址+段内偏移地址**

    内存分段后, 编译器分配偏移地址, CPU引入了段寄存器,
    通过改变段寄存器(CS,DS)存储的基址可以很方便的进行程序的重定位.

**内存访问**

    寄存器是16位宽的, 而8086的地址总线是20位宽(0~19), 寻址范围是1MB内存.
    如果不做处理, cs:ip最多访问17位宽的内存, 没有办法访问全部内存.
    所以8086CPU对段寄存器做了处理, 段内偏移地址自动乘以16(左移4位),
    这样无论段内偏移地址是多少, 都可以访问1MB内存了.

**地址回卷wrap-around**

    采用段基址左移4位+段内偏移地址最多可以访问的最大内存是
    0xffff*16+0xffff = 0xffff0+0xffff = 0xfffff+0xfff0 = 0xfffff+0xffff-0x000f = 1MB+64KB-16byte
    所以最大内存相比于1MB会多出64KB-16byte
    
    不过多出来的这部分可以不用处理,CPU只能通过20位宽的地址总线最多访问1MB的内存, 多出来的这部分CPU会从0开始计数
    即: 地址回卷

## 实模式下CPU内存寻址方式
寻址就是寻找地址, 所谓的地址在CPU眼中就是二进制数.
找到地址就可以获取该地址处存储的指令或数据, 从而执行相关操作. <br/>
8086 CPU实模式下寻址分为以下几种, 每种寻址方式都对应着特定的硬件电路: <br/>

**寄存器寻址** <br/>
地址存储在寄存器中, 比如

    mov ax, 0x10
    mov dx, 0x9
    mul dx, ax

这里执行乘法操作, 先把数据存入寄存器是立即数寻址, mul操作dx, ax是寄存器寻址.
<br/>

**立即数寻址** <br/>
直接把数据写在指令中, CPU不需要再去找了, 所以称为"立即", 立即数就是常数. <br/>

**内存寻址**<br/>
最常见的寻址方式, 虽然程序员可以将数据存入通用寄存器或者段寄存器,
但是寄存器太少, 一般数据都是存入内存中. <br/>
这时候CPU就需要根据指令中给定的内存地址(也就是段内偏移地址, 默认段基址是DS)去寻址.

**直接寻址** <br/>
就是直接把地址写在指令中, 通过地址符[]告诉CPU去取这个地址中的内容, 比如:

    mov ax [0x1234]

注意, []给出的地址也是偏移地址(8086 CPU), 默认段基址是DS. <br/>

**基址寻址** <br/>
基址寻址, 就是使用bx, bp寄存器作为偏移地址, 配合基址寄存器使用:

    DS:bx   SS:bp

DS:bx寄存器好理解, 就是一般的内存段访问. <br/>
SS:bp则有点特殊了, 因为SS是栈段寄存器,存储的是内存栈的地址.
这就要说到栈的2种访问方式: <br/>

***1.栈作为LIFO的数据结构, 通过栈顶指针(sp寄存器)访问*** <br/>
此时, 栈最重要的是2个方法: push 和 pop, 8086 [CPU字长](http://baike.baidu.com/view/32465.htm)是16,
默认入栈的内存空间是2个字节. 每次push, pop都会造成sp的移动(值改变), 比如: <br/>
push
    
    sub sp, 2
    mov sp, ax

pop

    mov ax, [sp]
    add sp 2

注意, 内存中的栈段push, pop是硬件实现的, 栈顶指针sp也是硬件维护的(寄存器),
CPU的运行离不开栈.
<br/>
但是使用sp只能访问栈顶元素的值,
如果希望访问栈顶到栈底中的任意值, 就不得不把栈当成普通的内存段来访问.

***2.栈作为普通的内存代码段, 通过段基址+段内偏移地址(SS:bp)访问*** <br/>
以一段(伪)代码为例

    int a = 0;
    function(int b, int c) {
        int d;
    }
    a++;

调用function(1, 2)时, 实参和形参匹配入栈(从右向左顺序),
然后将下一条指令的地址push进栈(也就是a++;语句的地址)

    stack

            return(a++) <-sp
            b(1)
            c(2)        <- ss

然后push bp入栈, 并将当前栈顶地址赋值给bp

    stack
            bp          <- sp
            return(a++)
            b(1)
            c(2)        <- ss

此时函数体中出现了局部变量d, 局部变量是存储在栈上的,
bp其实起到了分隔局部变量和其他部分的作用. <br/>
bp依据局部变量的总大小, "腾出"部分空间用于存储局部变量:

    stack
            --
            d
            bp
            --          <- sp
            return(a++)
            b(1)
            c(2)        <- ss

函数执行结束时, 将bp的值赋给sp, pop bp出栈, 直接跳过局部变量区.

    mov sp bp
    pop bp

    stack

           return(a++)  <- sp
           b(1)
           c(2)         <- ss

然后继续执行下一条语句. <br/>
在函数执行的时候我们就可以很方便的用ss:bp寻址了, 比如:

    [bp-4]: 局部变量d
    [bp+8]: 变量b
    [bp+12]: 变量c

<br/>
**变址寻址** <br/>
变址寻址利用si(source index),di(destination index)两个寄存器存储偏移量,
默认段寄存器是ds <br/>

**基址变址寻址** <br/>
就是基址寻址搭配变址寻址, 比如:

    mov [bx+di], ax
    add [bx+si], ax

## 到底什么是栈?!
这里的栈, 是由**硬件**实现push, pop指令(栈顶操作)作用于连续内存区域的栈(线性结构), 和数据结构中的栈是一样的, 不过是借由硬件实现的, 数据结构的栈更偏向于逻辑. <br/>
内存栈段的起始地址存放于SS寄存器, SP寄存器指向当前栈顶(由于栈是向下扩展的,
所以栈顶的地址低于栈底的地址), 栈段任意内存访问由ss:bp实现. <br/>
push ax 就是将ax寄存器中的值放入这段内存段中. <br/>
所以内存中的栈, 数据结构的栈本质上都是实现了: 线性结构+栈顶存取 的逻辑结构,
只不过CPU使用的内存栈段push, pop, 栈顶指针sp是由硬件维护的.<br/>
这段代码就利用了硬件实现的push, pop指令, SS, SP寄存器打造自己的 stack section:

    ; 8086cpu, 字长16

    section my_stack align=16 vstart=0
        times 128 db 0
    stack_top:

    mov ax, section.my_stack.start ; 自定义栈段的地址
    shr ax, 4                      ; CPU会自动左移ss四位, 所以先右移四位
    mov ss, ax                     ; 初始化段基址寄存器ss
    mov sp, stack_top              ; 初始化栈指针寄存器sp

    ; 可以使用自定义的栈了
    push word [var2]

    ; 分隔代码段和数据段
    jmp $    
 
    var2 dd 0x6

## 实模式下的ret  
ret是取返回地址的指令. <br/>
返回地址适合使用栈的方式存储(后进先出) <br/>
对于计算CS:IP的下一条指令, 下一条指令地址=上一条指令地址+上一条指令所占的内存大小. 这里的内存大小就可以放在程序计数器PC中, 用于替换IP寄存器或CS:IP寄存器(非跨段或者跨段) <br/>
而对于函数调用, 返回地址可能数目不定(嵌套调用), 所以存于内存栈中,
对于近返回(ret)弹出栈顶2个字节(16字长8086CPU一次进栈2个字节)替换IP寄存器(非跨段);
对于远返回(retf)弹出栈顶4个字节, 替换CS:IP寄存器(跨段).

## 实模式下的call
### 16位实模式相对近调用
首先, 近调用call只会改变ip寄存器的值, 且call指令的操作数是目标函数的相对地址:

    目标函数的相对地址 = 目标函数的绝对地址(物理地址) - 指令起始地址 - 3(call指令大小一般是3个字节)

call 指令的格式:

    call near function_name
    ; 这里的function_name只是函数起始地址的标号,
    不是指令, 编译器不会分配地址, 所以不会被加载到内存中, CPU也看不到,
    就不会执行.
    ; call的操作码是0xe8(1个字节), near关键字可以省略, 操作数占2个字节.

汇编代码示例(1call.S):

    call near near_proc
    jmp $
    addr dd 4
    near_proc:
        mov ax, 0x1234
            ret

反汇编结果(1call.s):

    00000000  E80600            call word 0x9
    00000003  EBFE              jmp short 0x3
    00000005  0400              add al,0x0      ; dd
    00000007  0000              add [bx+si],al  ; dd
    00000009  B83412            mov ax,0x1234 ; function name is just a simbol
    0000000C  C3                ret

可以看出```call near near_proc```编译后的机器码是```E80600```,
```0xe8```是call指令的操作码, 0x0600则是操作数,
可以看出这里的操作数是相对地址. near_proc是函数名只是一个函数起始地址的标号, 函数起始地址是0x9, 根据相对近调用的计算公式

    目标函数相对地址 = 0x9 - 0x0 - 0x3 = 0x6

由于是8086 CPU 小端字节序, 所以是 06 00, 组合起来的机器码就是 0xE80600 <br/>
不过我们也看到, CPU最终使用的还是0x9:目标函数的绝对地址.
相对地址是为了适应硬件, 也就是约定(规则). <br/>

### 16位实模式间接绝对近调用
相比于相对近调用: 都是近调用, 也就是16位64KB的地址范围内, 不会跨段,
只改变IP寄存器的值. <br/>
但是: <br/>

+ 间接: 目标函数地址不是直接通过函数名(其实就是函数地址)给出, 可能通过寄存器, 内存给出(不是立即数)
+ 绝对: 编译后call操作数是目标函数的绝对地址

代码示例

    section call_test vstart=0x900
        mov word [addr], near_proc
        call [addr]
        mov ax, near_proc
        call ax
    jmp $
    addr dd 4
    near_proc:
        mov ax, 0x1234
        ret

反汇编代码

    00000000  C7060C091009      mov word [0x90c],0x910
    00000006  FF160C09          call word [0x90c]
    0000000A  EBFE              jmp short 0xa
    0000000C  0400              add al,0x0
    0000000E  0000              add [bx+si],al
    00000010  B83412            mov ax,0x1234
    00000013  C3                ret

### 16位实模式直接绝对远调用

+ 直接: 内存地址直接以立即数的形式给出.
+ 绝对: 编译后机器码操作数是绝对地址.
+ 远调用: 跨段, 这里的跨段只是段基址更新, 至于值有没有变, CPU是不管的.

代码示例:

    section call_test vstart=0x900
    call 0: far_proc  ; call far
    jmp $
    far_proc:
        mov ax, 0x1234
        retf

反汇编代码:

    00000000  9A07090000        call word 0x0:0x907
    00000005  EBFE              jmp short 0x5
    00000007  B83412            mov ax,0x1234
    0000000A  CB                retf

```call word 0x0:0x907```, word表示16字节长度,
0x0:0x907就是目标函数的段基址:段内偏移地址. <br/>

### 16位实模式间接绝对远调用

+ 间接: 内存地址不再是立即数直接给出, 而是存于内存中(仅在内存)!
+ CPU需要先去内存寻址, 然后找到新的段基址:段内偏移地址; 首先call会把旧的CS寄存器的值push进栈, 然后把旧的IP寄存器的值push进栈, 再用寻址得到的新的值更新cs:ip. 进栈的值由retf(ret)pop弹栈返回.

代码示例:

    section call_test vstart=0x900
    call far [addr]
    jmp $
    addr dw far_proc, 0
    far_proc:
        mov ax, 0x1234
        retf

反汇编代码:

    00000000  FF1E0609          call word far [0x906]
    00000004  EBFE              jmp short 0x4
    00000006  0A09              or cl,[bx+di]
    00000008  0000              add [bx+si],al
    0000000A  B83412            mov ax,0x1234
    0000000D  CB                retf

## 实模式下的jmp
### 16位实模式相对短调用

+ 指令格式: jmp short
+ 指令大小: 2字节, 操作码eb占一个字节, 第二个字节是操作数
+ 短: jpm short 的操作数是相对的, 所以有符号. 范围是-128~127(一个字节有符号数)
+ 相对: 编译后的操作数(立即数)指向的地址是相对的, 但是反汇编可以看出CPU使用的还是计算的实际地址

相对地址计算:

    编译后jmp操作数的相对地址 = 操作数指向的目标函数的绝对地址-当前jmp指令的偏移地址(IP)-2(jmp指令大小)

代码示例:

    section jmp_test vstart=0x900
        mov ax, 0x1234
        jmp short start
        times 127 db 0
    start:
        mov ax, 0x1234
        jmp $

反汇编代码:

    00000000  EB7F              jmp short 0x81
    00000002  0000              add [bx+si],al
    ......... 127字节           add [bx+si],al
    0000007E  0000              add [bx+si],al
    00000080  00B83412          add [bx+si+0x1234],bh
    00000084  EBFE              jmp short 0x84   

xxd查看:

    00000000: b834 12eb 7f00 0000 0000 0000 0000 0000  .4..............
    00000010: 0000 0000 0000 0000 0000 0000 0000 0000  ................
    00000020: 0000 0000 0000 0000 0000 0000 0000 0000  ................
    00000030: 0000 0000 0000 0000 0000 0000 0000 0000  ................
    00000040: 0000 0000 0000 0000 0000 0000 0000 0000  ................
    00000050: 0000 0000 0000 0000 0000 0000 0000 0000  ................
    00000060: 0000 0000 0000 0000 0000 0000 0000 0000  ................
    00000070: 0000 0000 0000 0000 0000 0000 0000 0000  ................
    00000080: 0000 0000 b834 12eb fe                   .....4...

从xxd中可以看出编译后, ```jmp short start```的机器码是```0xeb7f00```,
目标函数start绝对地址是0x81(反汇编, 可见CPU最终使用的是绝对地址). <br/>

    0x7f = 0x81 - 0x0 - 2 !! great~!!!

### 16位实模式相对近转移

+ 指令格式: jmp near
+ 指令大小: 操作码(0xe9) + 操作数(16位) = 3个字节
+ 近: 近相对于短, 操作数跳转的范围更大了, 16字节, 当然对于16字长8086CPU, 这是IP寄存器的极限了, 再大就要更改CS寄存器的值(跨段啦)

至于相对, 和上面是一样的.

### 16位实模式间接绝对近转移

+ 指令格式: jmp near []/寄存器
+ 间接: 目标函数地址(操作数)不是直接用立即数给出, 而是存储在寄存器或内存中
+ 绝对: 操作数就是段内偏移地址IP

### 16位实模式直接绝对远转移

+ 直接: 立即数寻址(标号...)
+ 绝对: 给出的地址就是绝对地址, 而不是计算过的相对地址
+ 远转移: 跨段, 所以指令的格式是: jmp 段基址:段内偏移地址

代码示例:

    section jmp_test vstart=0x900
    jmp 0: start
    times 128 db 0
    start:
        mov ax, 0x1234
        jmp $

反编译代码:

    00000000  EA85090000        jmp 0x0:0x985
    00000083  0000              add [bx+si],al
    ........  ..... 省略128字节 ...............
    00000085  B83412            mov ax,0x1234
    00000088  EBFE              jmp short 0x88

xxd查看:

    00000000: ea85 0900 0000 0000 0000 0000 0000 0000  ................
    00000010: 0000 0000 0000 0000 0000 0000 0000 0000  ................
    00000020: 0000 0000 0000 0000 0000 0000 0000 0000  ................
    00000030: 0000 0000 0000 0000 0000 0000 0000 0000  ................
    00000040: 0000 0000 0000 0000 0000 0000 0000 0000  ................
    00000050: 0000 0000 0000 0000 0000 0000 0000 0000  ................
    00000060: 0000 0000 0000 0000 0000 0000 0000 0000  ................
    00000070: 0000 0000 0000 0000 0000 0000 0000 0000  ................
    00000080: 0000 0000 00b8 3412 ebfe                 ......4...   

可以看到我们指令给出的[jmp 段基址:段内偏移地址]是方便程序员习惯的格式,
实际的机器指令是0xea850900, 段内偏移地址在前, 段基址在后.
因为一般跨段的情况比较少见, 所以偏移地址在前, CPU处理更高效.

### 16位实模式间接绝对远转移

+ 间接: 不是立即数寻址的方式, 而是给出地址所在的内存的地址, CPU去寻址
+ 远转移: 因为是远转移所以需要存储段基址和段内偏移地址2个数, 所以此时存在内存中
+ 指令格式: jmp far 内存寻址, far是告诉CPU从内存中取4个字节的数据(区别间接绝对近转移)

代码示例:


反汇编代码:

    00000000  FF2E8409          jmp far [0x984]
    ........ ...... 省略128字节 ............... 
    00000086  0000              add [bx+si],al
    00000088  B83412            mov ax,0x1234
    0000008B  EBFE              jmp short 0x8b

xxd查看:

    00000000: ff2e 8409 0000 0000 0000 0000 0000 0000  ................
    00000010: 0000 0000 0000 0000 0000 0000 0000 0000  ................
    00000020: 0000 0000 0000 0000 0000 0000 0000 0000  ................
    00000030: 0000 0000 0000 0000 0000 0000 0000 0000  ................
    00000040: 0000 0000 0000 0000 0000 0000 0000 0000  ................
    00000050: 0000 0000 0000 0000 0000 0000 0000 0000  ................
    00000060: 0000 0000 0000 0000 0000 0000 0000 0000  ................
    00000070: 0000 0000 0000 0000 0000 0000 0000 0000  ................
    00000080: 0000 0000 8809 0000 b834 12eb fe         .........4...

## 标志寄存器flags
flags寄存器存储标志->所谓标志不是直接存储结果,
而是存储结果的特征(也就是产生这个结果计算机做了什么, 有没有溢出) <br/>

> 每个标志意义的详细介绍见《操真》99页

## 有条件转移
有条件转移不是某个指令, 而是一个指令族, 简称jxx, 指令格式: jxx 目标地址 <br/>
条件满足则跳转到目标地址, 否则顺序执行下一条指令. <br/>
几点⚠:

+ 目标地址只能是段内偏移地址, 编译器会自动比较当前指令地址和目标地址来编译近转移还是远转移
+ 条件转移指令, 判断的就是上一条指令对标志位的"影响"

**jxx有条件转移, 判断的条件就是上一条执行指令对标志寄存器flags的影响**

## 实模式小结
实模式终于**总结**完了....未来会以保护模式为主,
因为实模式段基址+段内偏移地址可以访问任意内存地址, 在内存空间中"漂移",
这意味着程序可能会覆盖操作系统内核代码! 而且实模式下CPU没有特权级.... <br/>
为了安全, 保护模式就是更好的选择. <br/>
![railgun](https://images2.alphacoders.com/580/thumb-350-58042.jpg)

