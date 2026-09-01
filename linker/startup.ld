/*
 * Copyright (c) 2026 Sparkle Silicon Technology Corp., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *
 * AE103 启动头部布局（ROM_START snippet，SORT_KEY 0x0 排在 zephyr
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
FILL(0xFF)                  /* ← 从这行开始，所有填充都变成 0xFF */

. = ALIGN(4);
KEEP(*(.vtable))            /* vector_base @ 0x80000 */
KEEP(*(.ae_start))          /* _start @ 0x80084 */
. = ORIGIN(ROM) + 0x100;    /* 0x80100 */
KEEP(*(.FlashInfo.Fixed))   /* Fix_flash_info @ 0x80100 */
. = ORIGIN(ROM) + 0x200;    /* 0x80200 起为 zephyr 正文 */
