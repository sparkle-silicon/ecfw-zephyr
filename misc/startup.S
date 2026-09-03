/*
 * Copyright (c) 2026 Sparkle Silicon Technology Corp., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *
 * AE103 启动头部汇编。对齐旧固件 Firmware/AE20X/START.S 的 .vtable / ._start
 * 布局，使芯片 ROM（复位后读 FlashInfo 的 IVT/ReStart）能正确搬运向量表并跳转。
 *
 * 向量表是「蜂鸟 E203 CLIC mtvt」风格：32 个 .word 处理器地址（jump by address），
 * 偏移 0x80 处一条 j 指令作异常/硬件错误兜底，合计 0x84 字节。ROM 会把这一段
 * 拷到 IVT_BASE_ADDR（0x30800，mtvt 固定指向）。启动骨架阶段向量统一占位指向
 * zephyr 的 _isr_wrapper（exception.entry 统一入口），中断子系统落地后再替换为
 * 各外设真实 handler（CPUS/CPUT/.../INTR0/INTR1）。
 */
#include <startup.H>
.section .vtable, "ax"		/* 定义 .vtable 段, a :可分配, x :可执行 */
.globl vector_base			/* 全局符号 vector_base 供链接器和其他代码调度用*/
vector_base:				/* 基地址标签 全局符号 vector_base 的起始*/
	.rept 32				/* 重复32次 下述内容到 .endr 为止 */
	.word _isr_wrapper      /* IRQ0..31 占位：统一指向 zephyr 异常入口 */
	.endr					/* 结束上述重复 */
	j _isr_wrapper          /* 偏移 0x80：异常/硬件错误兜底 */

/*
 * _start：复位入口桩，落点 0x80084（FlashInfo.ReStart + FLASH_BASE_ADDR）。
 * 直接跳 zephyr __start（vector.S，链接在 0x80200）接管启动。
 */
.section .ae_start, "ax" 	/* 定义 ae_start 段, a :可分配, x :可执行 */
.globl _start			 	/* 全局符号 _start 供链接器和其他代码调度用*/
_start:					 	/* 基地址标签 全局符号 vector_base 的起始*/
	j __start			 	/* 跳转到 __start */

#Old Code
# #include <RISCV.H>
# /*********************************INTERRUPT_VECTOR_TABLE.S*********************************/
# 	.section .vtable
#     .globl vector_base
#     //定义中断向量表
# vector_base:
#     .word  CPUS_HANDLER//0//software error
#     .word  CPUT_HANDLER//1//time error  
#     .word  CPUR_HANDLER//2//reg error
#     .word  SWUC_HANDLER//3//WU26
#     .word  LPC_ESPI_RST_HANDLER//4
#     .word  PWRSW_HANDLER//5
#     .word  PS2_0_HANDLER//6//ps2 mouse
#     .word  KBS_SDV_HANDLER//7
#     .word  KBS_PRESS_HANDLER//8
#     .word  TACH0_HANDLER//9
#     .word  TACH1_HANDLER//10
#     .word  TACH2_HANDLER//11
#     .word  TACH3_HANDLER//12
#     .word  KBC_IBF_HANDLER//13
#     .word  KBC_OBE_HANDLER//14
#     .word  PMC1_IBF_HANDLER//15
#     .word  PMC1_OBE_HANDLER//16
#     .word  WU42_HANDLER//17
#     .word  RTC_HANDLER//18
#     .word  WDT_HANDLER//19
#     .word  ADC_HANDLER//20
#     .word  UART0_HANDLER//21
#     .word  UARTA_HANDLER//22
#     .word  UARTB_HANDLER//23
#     .word  SM_EC_HANDLER//24
#     .word  SM_HOST_HANDLER//25
#     .word  TIMER0_HANDLER//26
#     .word  TIMER1_HANDLER//27
#     .word  TIMER2_HANDLER//28
#     .word  TIMER3_HANDLER//29
#     .word  INTR0_HANDLER//30
#     .word  INTR1_HANDLER//31
#      j (trap_entry+0x80080-0x30880)//j trap_entry//跳转硬件错误处理函数//由于IVT导致此处偏移有问题；
# 	/*原计算公式：offset = trap_entr -0x80080
# 	需求计算公式：offset = trap_entr -0x30880
# 	因此推导出需求关系和原公式关系可用：offset =trap_entr-0x30880+0x80080-0x80080= （trap_entr+ 0x80080-0x30880）-0x80080
# 	*/
# /*********************************INTERRUPT_VECTOR_TABLE.S*********************************/
# /*********************************START.S*********************************/
# /****************************************************************************
# * SPKAE20X start FLOW :
# * 1. Hardware Initialization
# * 2. Hardware Configuration CPU Program Counter(PC)
# * 3. Software Turns Off CPU Counts And CPU Global Interrupts
# * 4. Software Configuration Stack Pointers And Global Pointers
# * 5. Software Copies SECTION .data To RAM Space
# * 6. Software Initializes SECTION .bss To RAM Space
# * 7. Software Initializes EC SPACE Values 0 To RAM Space
# * 8. SPKAE20X Init Flow(ODM Custom)
# * 9. SPKAE20X Main Flow(ODM Custom)
# * 10.SPKAE20X Exit (while(1))
# ****************************************************************************/
#     .align 2
# 	.section ._start//声明section段名.scatterload
# 	.globl _start//全局标签_start
# 	.type _start,@function//定义为函数
# _start://标签位置
#     csrc CSR_MSTATUS, MSTATUS_MIE//中断全局关闭
#     csrsi CSR_MCOUNTINHIBIT, 0x5//关闭全局计数
    
# 	//li   t1, (3)	//pioc8 pioc9 high
#     //li   t0, 0x3005
#     //sb   t1, 0(t0)
#     //li   t0, 0x3001
#     //sb   t1, 0(t0)

#     la sp, _sp //将标签_sp所处的地址赋值给sp寄存器
#     //注意：标签_sp在链接脚本中定义，参见链接脚本的_sp标签
#     .option push
#     .option norelax
#     //设置全局指针
# 	la gp, __global_pointer$//将标签__global_pointer$所处的地址赋值给gp寄存器
# 	//注意：标签__global_pointer在链接脚本中定义参见链接脚本的__global_pointer$标签
#     .option pop
#     //设置堆栈指针
    
#     call __scatterload_ram//将FLASH中指定的内容拷贝到RAM中
# 	call Init//调用Init
# 	call main //调用main函数，开始执行main函数
# 	tail exit//如果完成了main函数后，调用exit函数（Newlib桩函数之一，参见第2.3.1
#             //节了解Newlib桩函数的更多信息）
# 1:
# 	j 1b //最后的死循环，程序理论上不可能执行到此处
# /*********************************START.S*********************************/
# /*********************************SCATTERLOAD.S*********************************/
#     .align 2
# 	.section .scatterload//声明section段名.scatterload
# 	.globl __scatterload_ram//全局标签_start
# 	.type __scatterload_ram,@function//定义为函数
# __scatterload_ram://将FLASH中指定的内容拷贝到RAM中

# 	 /* 使用与上述相同的原理，通过一个循环，将 数据从Flash中搬运到DLM中*/
# #if (defined(flash))
# //更新后防止被复写
#     la a1,0x30418
#     lw a0,0(a1)
#     andi a0,a0,0x100
#     bnez a0,__scatterload_data  //若0x304C0的bit3为1，则表示为固件更新后的首次启动，不能清除EC SPACE和通用内存空间，需跳过此部分
# __scatterload_globalmem://将EC_SPACE和通用内存空间的空间清零
# 2:	    //ec space
#     la a0, MEM_ADDR
# 	la a1, DATA_ADDR
# 	bgeu a0, a1, 2f
# 1:
# 	sw zero, (a0)
# 	addi a0, a0, 4
# 	bltu a0, a1, 1b
# __scatterload_shareram://将SRAM的空间清零
# 2:	    //sram
#     la a0, 0x31000
# 	la a1, 0x32000
# 	bgeu a0, a1, 2f
# 1:
# 	sw zero, (a0)
# 	addi a0, a0, 4
# 	bltu a0, a1, 1b
# __scatterload_data: //将常量数据从flash搬运到ram中
# 2:
# 	la a0, _data_lma
# 	la a1, _data
# 	la a2, _edata
# 	bgeu a1, a2, 2f
# 1:
# 	lw t0, (a0)
# 	sw t0, (a1)
# 	addi a0, a0, 4
# 	addi a1, a1, 4
# 	bltu a1, a2, 1b
# __scatterload_bss:
# 2:
# 	//BSS段是链接器预留的未初始化变量所处的地址段，引导程序必须对其初始化为0
# 	//此处通过一个循环来初始化BSS段
# 	la a0, __bss_start
# 	la a1, _end
# 	bgeu a0, a1, 2f
# 1:
# 	sw zero, (a0)
# 	addi a0, a0, 4
# 	bltu a0, a1, 1b
# __scatterload_vector:
# 2:
# 	la a0, vector_base
# 	la a1, 0x30800
# 	la a2, 0x30884
# 1:
# 	lw t0, (a0)
# 	sw t0, (a1)
# 	addi a0, a0, 4
# 	addi a1, a1, 4
# 	bltu a1, a2, 1b

#  #if 0//将FLASH中ROM的内容拷贝到SRAM（FPGA 中将ROM看作可写的RAN空间）中
# #if defined(AE201)&&(!defined(DOUBLE_FIRMWARE2))   
# __scatterload_rom:
# 2:
# 	la a0, 0xE0000//384k-416k作为rom代码段
#     la a1, 0x10000//32k
# 	la a2, 0x18000
# 	bgeu a1, a2, 2f
# 1:
# 	lw t0, (a0)
# 	sw t0, (a1)
# 	addi a0, a0, 4
# 	addi a1, a1, 4
# 	bltu a1, a2, 1b
# #endif
# #endif
# #endif
# 2:
#     ret
# /*********************************SCATTERLOAD.S*********************************/
# /*********************************CSR_CONFIG.S*********************************/
# 	.section .text.csr_config//声明section段名

# 	.global disable_mcycle_minstret
# disable_mcycle_minstret: //用于控制计数器的关闭(低功耗考虑) 
#         csrsi CSR_MCOUNTINHIBIT, 0x5
# 	ret
# 	.global enable_mcycle_minstret
# enable_mcycle_minstret: //用于控制计数器的开启    
#         csrci CSR_MCOUNTINHIBIT, 0x5
# 	ret
# # 	.global core_wfe
# # core_wfe: //休眠时，Wait for Event
# #     	csrc CSR_MSTATUS, MSTATUS_MIE 
# #     	csrs CSR_WFE, 0x1
# #     	wfi   //WFI的休眠模式
# #     	csrc CSR_WFE, 0x1
# #     	csrs CSR_MSTATUS, MSTATUS_MIE
# # 	ret
# /*********************************CSR_CONFIG.S*********************************/
