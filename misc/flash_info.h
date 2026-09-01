/*
 * Copyright (c) 2026 Sparkle Silicon Technology Corp., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *
 * AE103 芯片 FlashInfo（sFixedFlashInfo）定义。移植自旧固件
 * Firmware/AE20X/INCLUDE/AE_FLASHINFO.H，与芯片 ROM 引导的固定布局对齐。
 *
 * 芯片内部 ROM 复位后读外部 flash 0x80100 处的 FlashInfo.Fixed，用其中的
 * IVT 字段（偏移 32）搬运向量表、ReStart 字段（偏移 36）跳转固件入口。
 * 因此结构体的字段顺序/类型/位域是 ROM 的契约，不可改动；pack(1) 保证
 * IVT/ReStart 的绝对偏移精确。
 */

#ifndef __FLASH_INFO_H__
#define __FLASH_INFO_H__

#include <stdint.h>

#define FLASH_INFO_ENABLE_BACKUP 0
#define FLASH_INFO_ENABLE_EXTERNAL_FLASH 1
#define FLASH_INFO_ENABLE_PWM 1
#define FLASH_INFO_PWM_SWITCH_CHANNEL 0
#define FLASH_INFO_ENABLE_JTAG 0
#define FLASH_INFO_ENABLE_ROMPATCH 0
#define FIRMWARE_KBYTE_SIZE 512
#define PRINTF_UART_SWITCH 0
#define PRINTF_UART_BAUD 115200
#define FLASH_INFO_MAIN_FREQ_DEFAULT        4  /* 默认主频分频系数 */
#define FLASH_INFO_UART0_TXD_SWITCH 0//UART0_TXD_SEL
#define FLASH_INFO_UART1_TXD_SWITCH 0//UART1_TXD_SEL
#define FLASH_INFO_UART1_RXD_SWITCH 0//UART1_RXD_SEL
#define FLASH_INFO_UARTA_TXD_SWITCH 0//UARTA_TXD_SEL
#define FLASH_INFO_UARTA_RXD_SWITCH 0//UARTA_RXD_SEL

/* flash 基址（旧固件 AE20X/INCLUDE/AE_REG.H: FLASH_BASE_ADDR）。 */
#define FLASH_BASE_ADDR 0x80000UL

#define FIRMWARE_KBYTE_SIZE_MAX 512
#define FIRMWARE_KBYTE_SIZE_MIN 1

#if (FIRMWARE_KBYTE_SIZE>FIRMWARE_KBYTE_SIZE_MAX)
#undef FIRMWARE_KBYTE_SIZE
#define FIRMWARE_KBYTE_SIZE FIRMWARE_KBYTE_SIZE_MAX
#elif (FIRMWARE_KBYTE_SIZE<FIRMWARE_KBYTE_SIZE_MIN)
#undef FIRMWARE_KBYTE_SIZE
#define FIRMWARE_KBYTE_SIZE FIRMWARE_KBYTE_SIZE_MIN
#endif
#define FIRMWARE_BYTE_SIZE   ((FIRMWARE_KBYTE_SIZE) * 1024)
#define FIRMWARE_4KBYTE_SIZE   ((FIRMWARE_KBYTE_SIZE+3) >>2)
#if ((FIRMWARE_4KBYTE_SIZE)<((FIRMWARE_KBYTE_SIZE_MIN+3)>>2))
#define FLASH_INFO_FIRMWARE_4KSECTOR ((FIRMWARE_KBYTE_SIZE_MIN+3)>>2)
#elif ((FIRMWARE_4KBYTE_SIZE)>((FIRMWARE_KBYTE_SIZE_MAX+3)>>2))
#define FLASH_INFO_FIRMWARE_4KSECTOR ((FIRMWARE_KBYTE_SIZE_MAX+3)>>2)
#else
#define FLASH_INFO_FIRMWARE_4KSECTOR (FIRMWARE_4KBYTE_SIZE-1)
#endif
#define FLASH_INFO_EXTERNAL_FLASH_ENABLE     1
#define FLASH_INFO_EXTERNAL_FLASH_DISABLE    0

#define FLASH_INFO_LAST8M_DISABLE            1
#define FLASH_INFO_LAST8M_ENABLE             0
#define FLASH_INFO_FIND_16M                  FLASH_INFO_LAST8M_DISABLE
#define FLASH_INFO_0X0_START_FIND            FLASH_INFO_LAST8M_DISABLE
#define FLASH_INFO_FIND_LAST8M               FLASH_INFO_LAST8M_ENABLE
#define FLASH_INFO_0X80000_START_FIND        FLASH_INFO_LAST8M_ENABLE

/* 低地址查找范围 */
#define FLASH_INFO_LOWADDR_MIRROR_LAST       0
#define FLASH_INFO_LOWADDR_INTERVAL_64K      1
#define FLASH_INFO_LOWADDR_RANGE_16K         2
#define FLASH_INFO_LOWADDR_RANGE_4K          3

/* 高地址查找空间 */
#define FLASH_INFO_HIGHADDR_SPACE_BIT23        7
#define FLASH_INFO_HIGHADDR_SPACE_BIT22        6
#define FLASH_INFO_HIGHADDR_SPACE_BIT21        5
#define FLASH_INFO_HIGHADDR_SPACE_BIT20        4
#define FLASH_INFO_HIGHADDR_SPACE_BIT19        3
#define FLASH_INFO_HIGHADDR_SPACE_BIT18        2
#define FLASH_INFO_HIGHADDR_SPACE_BIT17        1
#define FLASH_INFO_HIGHADDR_SPACE_BIT16        0

#define FLASH_INFO_SPI_QUAD                  0
#define FLASH_INFO_SPI_DUAL                  1

#define FLASH_INFO_WP_GPIOD08                0
#define FLASH_INFO_WP_GPIOB17                1

// #define FLASH_INFO_CSN_GPIOB22               0
// #define FLASH_INFO_CSN_GPIOB18               1

#define FLASH_INFO_MIRROR_ALWAYS             0
#define FLASH_INFO_MIRROR_WHEN_DIFFERENT     1

#define FLASH_INFO_MIRROR_DIRECT             0
#define FLASH_INFO_MIRROR_CACHE              1

#define FLASH_INFO_PWM_DISABLE               0
#define FLASH_INFO_PWM_ENABLE                1

#define FLASH_INFO_PWM_SWITCH FLASH_INFO_PWM_SWITCH_CHANNEL

#define FLASH_INFO_BACKUP_ENABLE             1
#define FLASH_INFO_BACKUP_DISABLE            0

#define FLASH_INFO_ROMPATCH_ENABLE           0
#define FLASH_INFO_ROMPATCH_DISABLE          1

#define FLASH_INFO_EXIT_REBOOT               1
#define FLASH_INFO_EXIT_SLEEP                0

#define FLASH_INFO_LPC_ENABLE                1
#define FLASH_INFO_ESPI_ENABLE               0

#define FLASH_INFO_JTAG_ENABLE               1
#define FLASH_INFO_JTAG_DISABLE              0

#define FLASH_INFO_JTAG_SWITCH_PIOE          1
#define FLASH_INFO_JTAG_SWITCH_PIOB          0

#define FLASH_INFO_LOG_ENABLE                1
#define FLASH_INFO_LOG_DISABLE               0

#define FLASH_INFO_LOG_VERBOSE               0
#define FLASH_INFO_LOG_BRIEF                 1

#define FLASH_INFO_DEBUGGER_ENABLE           1
#define FLASH_INFO_DEBUGGER_DISABLE          0

#define FLASH_INFO_DEBUGGER_UART_ENABLE      1
#define FLASH_INFO_DEBUGGER_UART_DISABLE     0

#define FLASH_INFO_DEBUGGER_SMBUS_ENABLE     1
#define FLASH_INFO_DEBUGGER_SMBUS_DISABLE    0

#define FLASH_INFO_DLS_5BITS                 0
#define FLASH_INFO_DLS_6BITS                 1
#define FLASH_INFO_DLS_7BITS                 2
#define FLASH_INFO_DLS_8BITS                 3

#define FLASH_INFO_STOP_1BIT                 0
#define FLASH_INFO_STOP_2BITS                1

#define FLASH_INFO_PARITY_DISABLE            0
#define FLASH_INFO_PARITY_ENABLE             1

#define FLASH_INFO_PARITY_EVEN               0
#define FLASH_INFO_PARITY_ODD                1

#define LOG_PRINTF_BAUD PRINTF_UART_BAUD
#define FLASH_INFO_BAUD_RATE(baud) (((baud)<=3200)?0:\
                                (((baud)>=409600)?0x7F:\
                                ((((baud)+1600) / 3200) - 1)))

#define FLASH_INFO_LOG_PRINTF_BAUD FLASH_INFO_BAUD_RATE(LOG_PRINTF_BAUD)
#define FLASH_INFO_DEBUGGER_UART_BAUD FLASH_INFO_BAUD_RATE(UART1_BAUD)

/* 向量表 / 复位入口符号（misc/startup.S 定义，链接在 0x80000 / 0x80084）。 */
extern uint32_t vector_base;
extern uint32_t _start;

/* FlashInfo 字段：相对 FLASH_BASE_ADDR 的偏移（编译期由链接符号算出）。 */

/* ROM PATCH 描述符（旧固件 spatch_typdef，pack(1) 下 4 字节）。 */
// #pragma pack(1)
typedef struct _patch_typdef{
	uint32_t data;
	uint8_t  addrl;
	uint8_t  addrh : 5;
	uint8_t  last : 1;
} spatch_typdef;

#pragma pack(push, 1)
typedef struct _FixedFlashInfo{
	/* 0 */
	uint8_t Firmware_ID[11];        /* 识别码 */
#define FLASH_INFO_FIRMWARE_ID  "SPK32AE201\0"//"SPK32AE201\0"//CORE_NAME
	uint8_t Compiler_Version[21];   /* 版本号 */
#define FLASH_INFO_COMPILER_VERSION   __DATE__ " "__TIME__"\0"//CODE_VERSION
	/* 32 (0x20) */
	uint32_t IVT;                   /* 中断向量表起始（相对 Mirror_Addr） */
#define FLASH_INFO_IVT     ((uint32_t)&vector_base - FLASH_BASE_ADDR)   /* = 0x00000 */
	/* 36 (0x24) */
	uint32_t ReStart;               /* 复位运行起始地址 */
#define FLASH_INFO_RESTART ((uint32_t)&_start/*__start*/     - FLASH_BASE_ADDR)   /* = 0x00084 */
	/* 40 (0x28) */
	uint32_t BackUp_FlashAddr;      /* 备份区相对偏移 */
#define FLASH_INFO_BACKUP_FLASHADDR   FIRMWARE_BYTE_SIZE
	/* 44 (0x2C) */
	uint32_t Dynamic_FlashInfo;     /* 尾部 FLASH 动态信息相对偏移 */
	/* 48 (0x30) */
	union{
		uint32_t Sysctl_Reserved;
		struct{
			uint32_t Enable_Used : 1;  /* 0 禁外部FLASH / 1 允许 */
			uint32_t Last8M_Disable : 1;  /* 0 后8M / 1 全16M */
			uint32_t LowAddr_Offset : 2;  /* 低4位镜像查找范围 */
			uint32_t HighAddr_Space : 3;  /* 外部FLASH查找最高位 */
			uint32_t Must_Mirror_Disable : 1;  /* 0 必镜像 / 1 不同才镜像 */
			uint32_t NoCare : 8;
			uint32_t PWM_Enable : 1;  /* 镜像期间 PWM 闪烁 */
			uint32_t PWMn_Switch : 3;
			uint32_t SPI_Switch : 1;  /* 0 quad / 1 dual */
			uint32_t WP_Switch : 1;  /* 0 GPIOD08 / 1 GPIOB17 */
			uint32_t CSN_Switch : 1;  /* 无效，默认0 */
			uint32_t Firmware_4KSector : 7;  /* 镜像按 4K 单位拷贝（实际+1） */
			uint32_t Mirror2RunCache : 1;
			uint32_t Reserverd : 1;
		};
	} External_Flash_Control;
	/* 52 (0x34) */
	uint32_t MainFrequency : 6;  /* 主频分频系数 */
	uint32_t BackUp_Enable : 1;
	uint32_t RomPatch_Disable : 1;
	uint32_t Exit_ReBoot_Switch : 1;  /* 0 休眠 / 1 自动重启 */
	uint32_t LPC_Enable : 1;  /* 0 ESPI / 1 LPC */
	uint32_t JTAG_Enable : 1;
	uint32_t JTAG_Switch : 1;
	uint32_t LOG_PRINTF_Enable : 1;
	uint32_t DEBUGGER_Enable : 1;
	uint32_t LOG_PRINTF_UARTn_Switch : 2;
	uint32_t LOG_PRINTF_LEVEL : 1;
	uint32_t LOG_PRINTF_BAUD_RATE : 7;
	uint32_t DEBUGGER_UART_Enable : 1;
	uint32_t DEBUGGER_UART_BAUD_RATE : 7;
	uint32_t DEBUGGER_SMBUS_Enable : 1;
	uint32_t DEBUGGER_SMBUS_SLAVE_Addr : 7;
	uint32_t LOG_PRINTF_DLS : 2;
	uint32_t LOG_PRINTF_STOP : 1;
	uint32_t LOG_PRINTF_PE : 1;
	uint32_t LOG_PRINTF_EPE : 1;
	uint32_t DEBUGGER_UART_DLS : 2;
	uint32_t DEBUGGER_UART_STOP : 1;
	uint32_t DEBUGGER_UART_PE : 1;
	uint32_t DEBUGGER_UART_EPE : 1;
	uint32_t DEBUGGER_SMBUS_Switch : 3;
	uint32_t DEBUGGER_SMBUS_SPEED_Switch : 1;
	uint32_t DEBUGGER_SMBUS_CLK_Switch : 2;
	uint32_t DEBUGGER_SMBUS_DAT_Switch : 2;
	uint32_t UART0_TXD_Switch : 2;
	uint32_t UART1_TXD_Switch : 1;
	uint32_t UART1_RXD_Switch : 1;
	uint32_t UARTA_TXD_Switch : 1;
	uint32_t UARTA_RXD_Switch : 1;
	/* 60 (0x3C) */
	spatch_typdef PATCH[8];
} sFixedFlashInfo;

typedef struct _rom_data//关于FLASH的一些信息
{
	uint32_t Flash_ID;//外部FLASH ID如果外部FLASH ID发生变动，需要除了版本号以外校验签名是否一致
	uint32_t Firmware_Address : 24;//外部flash中固件的相对位置位置(上次MIRROR位置,绝对地址)
} sExternalFlash_Info;

#pragma pack(pop)

extern sFixedFlashInfo Fix_flash_info;

#endif /* __FLASH_INFO_H__ */
