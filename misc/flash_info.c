/*
 * Copyright (c) 2026 Sparkle Silicon Technology Corp., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Fix_flash_info：芯片 ROM 引导契约数据，落在 .FlashInfo.Fixed 段（链接 0x80100）。
 * ROM 复位后读此结构体的 IVT(@32)/ReStart(@36) 搬运向量表并跳转入口，因此
 * 字段值相对 FLASH_BASE_ADDR 由链接符号编译期算出，不做运行期改写。
 */

#include "flash_info.h"

/*
 * 段名必须精确为 .FlashInfo.Fixed（startup.ld 里 KEEP(*(.FlashInfo.Fixed))），
 * 故不用项目 __in_section(...)（其生成 .a.b.c 带后缀段名），直接指定 section。
 */
__attribute__((section(".FlashInfo.Fixed"), used))
sFixedFlashInfo Fix_flash_info = {
	.Firmware_ID = FLASH_INFO_FIRMWARE_ID, // 特殊字符
	.Compiler_Version = FLASH_INFO_COMPILER_VERSION,
	// FLASH	地址
	.IVT = FLASH_INFO_IVT, // 中断向量表
	.ReStart = FLASH_INFO_RESTART,  // 起始地址
#if FLASH_INFO_ENABLE_BACKUP
	.BackUp_Enable = FLASH_INFO_BACKUP_ENABLE,
	.BackUp_FlashAddr = FLASH_INFO_BACKUP_FLASHADDR,//备份域FLASH处绝对地址,默认紧随其后
#else
	.BackUp_Enable = FLASH_INFO_BACKUP_DISABLE,
#endif
	.Dynamic_FlashInfo = __UINT32_MAX__,//0xffffffff// 由编译填入

	// 外部FLASH控制
#if FLASH_INFO_ENABLE_EXTERNAL_FLASH
	.External_Flash_Control.Enable_Used = FLASH_INFO_EXTERNAL_FLASH_ENABLE,
	.External_Flash_Control.Last8M_Disable = FLASH_INFO_LAST8M_DISABLE,
	.External_Flash_Control.LowAddr_Offset = FLASH_INFO_LOWADDR_RANGE_4K,
	.External_Flash_Control.HighAddr_Space = FLASH_INFO_HIGHADDR_SPACE_BIT23,
	.External_Flash_Control.Must_Mirror_Disable = FLASH_INFO_MIRROR_WHEN_DIFFERENT,
#if FLASH_INFO_ENABLE_PWM
	.External_Flash_Control.PWM_Enable = FLASH_INFO_PWM_ENABLE,
	.External_Flash_Control.PWMn_Switch = FLASH_INFO_PWM_SWITCH,
#else
	.External_Flash_Control.PWM_Enable = FLASH_INFO_PWM_DISABLE,
#endif
	.External_Flash_Control.SPI_Switch = FLASH_INFO_SPI_DUAL,
	.External_Flash_Control.WP_Switch = FLASH_INFO_WP_GPIOB17,
	// .External_Flash_Control.CSN_Switch = FLASH_INFO_CSN_GPIOB22,

	.External_Flash_Control.Firmware_4KSector = FLASH_INFO_FIRMWARE_4KSECTOR,
	.External_Flash_Control.Mirror2RunCache = FLASH_INFO_MIRROR_CACHE,
#else
	.External_Flash_Control.Enable_Used = FLASH_INFO_EXTERNAL_FLASH_DISABLE,			// 允许使用外部FLASH（内部为准）
#endif

	.MainFrequency = FLASH_INFO_MAIN_FREQ_DEFAULT,
	.Exit_ReBoot_Switch = FLASH_INFO_EXIT_REBOOT,
	.LPC_Enable = FLASH_INFO_LPC_ENABLE,//使能LPC口
#if FLASH_INFO_ENABLE_JTAG
	.JTAG_Enable = FLASH_INFO_JTAG_ENABLE,
	.JTAG_Switch = FLASH_INFO_JTAG_SWITCH_PIOE,
#else
	.JTAG_Enable = FLASH_INFO_JTAG_DISABLE,
#endif
#if PRINTF_UART_SWITCH<=UARTB_CHANNEL
	.LOG_PRINTF_Enable = FLASH_INFO_LOG_ENABLE,
	.LOG_PRINTF_UARTn_Switch = PRINTF_UART_SWITCH,
	.LOG_PRINTF_LEVEL = FLASH_INFO_LOG_BRIEF,
	.LOG_PRINTF_BAUD_RATE = FLASH_INFO_LOG_PRINTF_BAUD,
	.LOG_PRINTF_DLS = FLASH_INFO_DLS_8BITS,
	.LOG_PRINTF_STOP = FLASH_INFO_STOP_1BIT,
	.LOG_PRINTF_PE = FLASH_INFO_PARITY_DISABLE,
	.LOG_PRINTF_EPE = FLASH_INFO_PARITY_EVEN,
#else
	.LOG_PRINTF_Enable = FLASH_INFO_LOG_DISABLE,
#endif
#if SUPPORT_DEBUGGER_ENABLE
	.DEBUGGER_Enable = FLASH_INFO_DEBUGGER_ENABLE,
#if DEBUGGER_OUTPUT_SWITCH == 0
	.DEBUGGER_UART_BAUD_RATE = FLASH_INFO_DEBUGGER_UART_BAUD,//默认用UART1
	.DEBUGGER_UART_DLS = FLASH_INFO_DLS_8BITS,
	.DEBUGGER_UART_STOP = FLASH_INFO_STOP_1BIT,
	.DEBUGGER_UART_PE = FLASH_INFO_PARITY_DISABLE,
	.DEBUGGER_UART_EPE = FLASH_INFO_PARITY_EVEN,
#else
	.DEBUGGER_UART_Enable = FLASH_INFO_DEBUGGER_UART_DISABLE,
#endif
#if DEBUGGER_OUTPUT_SWITCH == 1
	.DEBUGGER_SMBUS_Enable = FLASH_INFO_DEBUGGER_SMBUS_ENABLE,
	.DEBUGGER_SMBUS_SLAVE_Addr = DEBUGGER_SMBUS_SLAVE_ADDRESS,
	.DEBUGGER_SMBUS_Switch = DEBUGGER_I2C_CHANNEL,
	.DEBUGGER_SMBUS_SPEED_Switch = (DEBUGGER_SMBUS_SPEED <= 100000 ? 1 : 0),
	.DEBUGGER_SMBUS_CLK_Switch = DEBUGGER_SMBUS_CLK_SWITCH,
	.DEBUGGER_SMBUS_DAT_Switch = DEBUGGER_SMBUS_DAT_SWITCH,
#else
	.DEBUGGER_SMBUS_Enable = FLASH_INFO_DEBUGGER_SMBUS_DISABLE,
#endif
#else
	.DEBUGGER_Enable = FLASH_INFO_DEBUGGER_DISABLE,
#endif
#if PRINTF_UART_SWITCH == UART0_CHANNEL
	.UART0_TXD_Switch = FLASH_INFO_UART0_TXD_SWITCH,
#endif
#if SUPPORT_DEBUGGER_ENABLE&&(DEBUGGER_OUTPUT_SWITCH == 0)||(PRINTF_UART_SWITCH==UART1_CHANNEL)
	.UART1_TXD_Switch = FLASH_INFO_UART1_TXD_SWITCH,
	.UART1_RXD_Switch = FLASH_INFO_UART1_RXD_SWITCH,
#endif
#if PRINTF_UART_SWITCH == UARTA_CHANNEL
	.UARTA_TXD_Switch = FLASH_INFO_UARTA_TXD_SWITCH,
	.UARTA_RXD_Switch = FLASH_INFO_UARTA_RXD_SWITCH,
#endif
#if FLASH_INFO_ENABLE_ROMPATCH
		.RomPatch_Disable = FLASH_INFO_ROMPATCH_ENABLE,
		.PATCH = {
		[0] .data.dword = _RISCV_WHILE1_ASM,//自循环
		[0].addrl = (PATCH0_ADDR >> 2) & 0xff,
		[0].addrh = (PATCH0_ADDR >> 10) & 0x1f,
		[0].last = 1,
		[1].data.dword = 0x00010001,
		[1].addrl = (PATCH1_ADDR >> 2) & 0xff,
		[1].addrh = (PATCH1_ADDR >> 10) & 0x1f,
		[1].last = 0,
	},
#else
	.RomPatch_Disable = FLASH_INFO_ROMPATCH_DISABLE,
#endif

};
