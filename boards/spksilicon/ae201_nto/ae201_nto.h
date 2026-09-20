/*
 * Copyright (c) 2026 Sparkle Silicon Technology Corp., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file ae201_nto.h
 * @brief AE201 NTO 板级标识与板级专属常量。
 *
 * 与 x86 EC 侧的 boards/nuvoton、boards/microchip 是**并列关系**：
 * 各自对应一组互斥的 SoC 条件，彼此不共享头文件。公共的板级接口
 * （board_init / board_suspend / board_resume）声明在 boards/board_config.h，
 * 本头文件只承载 AE201 NTO 这一块板卡自己的东西。
 */

#ifndef __AE201_NTO_H__
#define __AE201_NTO_H__

/* ===================== 板级标识 ===================== */

/** 板卡名（与 Kconfig.defconfig 的 config BOARD default 保持一致）。 */
#define AE201_NTO_BOARD_NAME "ae201_nto"

/** 板卡型号描述（与 Kconfig.board 的 bool 提示串保持一致）。 */
#define AE201_NTO_BOARD_MODEL "SPK32 AE201 NTO"

/*
 * 板级引脚表 / 板级 ID 常量在此追加。
 *
 * 接入状态：app 层外设就绪后再补。x86 侧对应物是
 * boards/nuvoton/npcx4m8f_mecc/npcx4m8f_aic_on_ptl.c 的 gpio_ec_config
 * 数组（mecc_npcx9_cfg / _sus / _res），AE201 侧待外设接入时按同样形态建表。
 */

#endif /* __AE201_NTO_H__ */
