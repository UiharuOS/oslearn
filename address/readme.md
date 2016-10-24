# 地址, section, vstart

## 地址
地址, 就是符号(指令、数据)相对于文件开头的偏移量, 由编译器负责分配.
CPU通过地址总线访问地址.<br/>
偏移量自然是**从0开始的**异世界生活.

例子:<br/>
~address.S

    mov ax, $$          ; $$当前段地址, 默认整个文件看作一个段section: 0x0
    mov ds, ax
    mov ax, [var]       ; []取址: 0xd
    label: mov ax, $    ; label行标号, $当前行地址: 0x8
    jmp label           ; 死循环, 分割数据和代码: 0x8
    var dw 0x99         ; dw 双字, x86CPU是小端字节序, 低位在低字节(99),
                        ;                              高位在高字节(00)

~反汇编

      地址  内容(机器码或数据)  反汇编代码
    00000000  B80000            mov ax,0x0
    00000003  8ED8              mov ds,ax
    00000005  A10D00            mov ax,[0xd]
    00000008  B80800            mov ax,0x8
    0000000B  EBFB              jmp short 0x8
    0000000D  99                cwd
    0000000E  00                db 0x00

通过反汇编可以很清晰的看出

    编译器分配的地址=相对于文件开头的偏移量=上一个符号的偏移量+上一个符号的内存大小

## section
section是汇编中的**伪指令**, 只是给程序员用的, 编译器不会给section分配地址,
所以CPU是不知道section的, CPU看到的就是连续的指令.

为什么需要section

    ~可以通过section将代码划分为代码段和数据段, 程序员在逻辑上更加清晰.
    ~而且数据段不能和代码段弄混, CPU眼中只有指令,
     如果CPU把数据定义指令当作代码指令读取并译码运行, 可能会造成破坏.

例子:<br/>
~section.S

    section code                ; 编译器没有分配地址给这个指令, section是伪指令
    mov ax, $$                  ; $$当前段地址仍然是0x0, 从0开始的
    mov ax, section.data.start  ; section.data.start是数据段地址:0x10
    mov ax, [var1]              ; var1的地址是0x10, section.data没有被分配地址
    mov ax, [var2]
    label: jmp label            ; 死循环分隔
    
    section data
    var1 dd 0x4
    var2 dw 0x99

~反汇编

    00000000  B80000            mov ax,0x0
    00000003  B81000            mov ax,0x10
    00000006  A11000            mov ax,[0x10]
    00000009  A11400            mov ax,[0x14]
    0000000C  EBFE              jmp short 0xc
    0000000E  0000              add [bx+si],al
    00000010  0400              add al,0x0
    00000012  0000              add [bx+si],al
    00000014  99                cwd
    00000015  00                db 0x00

可见, section只是给程序员看的, 做逻辑上的区分, 编译器不会分配给section地址,
CPU也不知道section的存在.

## vstart
vstart配合section使用, 用来计算在**该section内**所有内存**引用地址**.<br/>
vstart就是告诉编译器, 当前段、当前行、变量的地址相对于vstart声明的地址分配,
但是编译器分配给每个符号的地址仍然是实际地址, 即相对于起始地址的偏移量.<br/>
如果你知道你的程序会被加载器加载到内存何处, 那么你就可以用vstart声明这个地址,
这样程序中的地址就会相对于这个地址分配. <br/>

例子:<br/>
~section.S

    section code vstart=0x7c00   ; vstart+section: 编译器没有给这个指令分配地址,
                                 ; 所以CPU不知道,
                                 ; 但是vstart会影响编译器的地址分配策略
    mov ax, $$                   ; 当前段地址相对于vstart分配: 0x7c00
    mov ax, section.code.start   ; section.code.start指向的真实地址仍然是
                                 ; 相对于文件开头的偏移量: 0x0
    mov ax, section.data.start   ; 同上
    mov ax, $                    ; 当前行地址也相对于0x7c00: 0x7c00+9
    mov ax, [var1]               ; var1的地址相对于data section的vstart
    mov ax, [var2]               ; 同上
    jmp $                        ; 这里是0x12!!~!!是真实地址?(貌似是ndisasm的bug)
    section data vstart=0x900
    var1 dd 0x4
    var2 dw 0x99

~反汇编

    00000000  B8007C            mov ax,0x7c00
    00000003  B80000            mov ax,0x0
    00000006  B81400            mov ax,0x14
    00000009  B8097C            mov ax,0x7c09
    0000000C  A10009            mov ax,[0x900]
    0000000F  A10409            mov ax,[0x904]
    00000012  EBFE              jmp short 0x12
    00000014  0400              add al,0x0
    00000016  0000              add [bx+si],al
    00000018  99                cwd
    00000019  00                db 0x00

BIOS进入MBR是通过jmp 0: 7c00指令, 此时cs变为0, 相当于"平坦模型",
所以所有地址相对于0x7c00也就不会出错了(都是偏移地址).
