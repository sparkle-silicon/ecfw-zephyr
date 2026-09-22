/*
 * Copyright (c) 2026 Sparkle Silicon Technology Corp., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *
 * AE201 启动头部布局（ROM_START snippet，SORT_KEY 0x0 排在 zephyr
 * vector_table.ld 之前）。对齐旧固件 Firmware/AE20X/ENV/link_flash.lds 的
 * .boot 段，使芯片 ROM 能按 FlashInfo 的 IVT/ReStart 字段正确跳转并搬运向量表：
 *
 *   .vtable          (vector_base)  @ 0x80000  32 中断向量 + j trap_entry (0x84B)
 *   .ae_start        (_start)       @ 0x80084  复位入口桩
 *   .FlashInfo.Fixed (Fix_flash_info)@ 0x80100  芯片 ROM 硬读 IVT(@32)/ReStart(@36)
 *   zephyr .vectors(__start)/reset/text @ 0x80200
 *
 * 注：ORIGIN(ROM) 是 zephyr common/linker.ld 里 XIP 下的 flash 区域名
 * （ROM_BASE = devicetree flash0 = 0x80000），等价旧固件的 flash 区域。
 */

/*
 * 本片段只在「镜像由 BootROM 直接引导」时才有意义，加了引导加载程序就不该再占位。
 *
 * 判据用 CONFIG_BOOTLOADER_MCUBOOT —— 该符号由 Zephyr 的 mcuboot 模块
 * (modules/Kconfig.mcuboot) 提供，语义正是「本镜像由 MCUBoot 链式加载」。
 * 模块不在时符号根本不存在，#if !defined 恒为真，构建输出与从前逐字节一致；
 * 也就是说这条判断对现有固件零影响，只在引入 MCUBoot 后才起作用。
 *
 *   未启用 MCUBoot：应用就是被 BootROM 引导的那个镜像，必须自带 FlashInfo
 *                   契约点（IVT/ReStart），布局如上方注释。
 *   启用 MCUBoot：  引导程序自己带 FlashInfo，应用跑在分区表的槽位里，
 *                   0x80100 契约点与 0x200 正文偏移都与它无关 ——
 *                   起始地址交给分区表，向量表回到 Zephyr 默认布局。
 */
#if !defined(CONFIG_BOOTLOADER_MCUBOOT)

FILL(0xFF)                  /* ← 从这行开始，所有填充都变成 0xFF */

. = ALIGN(4);
KEEP(*(.vtable))            /* vector_base @ 0x80000 */
KEEP(*(.ae_start))          /* _start @ 0x80084 */
. = ORIGIN(ROM) + 0x100;    /* 0x80100 */
KEEP(*(.FlashInfo.Fixed))   /* Fix_flash_info @ 0x80100 */
. = ORIGIN(ROM) + 0x200;    /* 0x80200 起为 zephyr 正文 */

#endif /* !CONFIG_BOOTLOADER_MCUBOOT */
