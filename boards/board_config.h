/*
 * Copyright (c) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __BOARD_COMMON_H__
#define __BOARD_COMMON_H__

/**
 * @brief This global variable helps to configure variable gpios.
 */
extern uint8_t boot_mode_maf;

#if defined(CONFIG_SOC_FAMILY_MEC)

#ifdef CONFIG_BOARD_MEC1501MODULAR_ASSY6885
#include "mec15xx_aic_rvp_selection.h"
#elif CONFIG_BOARD_MEC172XMODULAR_ASSY6930
#include "mecc172x_aic_rvp_selection.h"
#elif defined(CONFIG_BOARD_MEC1501_EUCLID)
#include "euclid_mec1501.h"
#elif defined(CONFIG_BOARD_MEC1501_ADL)
#include "adl_mec1501.h"
#elif defined(CONFIG_BOARD_MEC1501_ADL_P)
#include "adl_p_mec1501.h"
#elif defined(CONFIG_BOARD_MEC1501_MTL_P)
#include "mtl_p_mec1501.h"
#elif defined(CONFIG_BOARD_MEC172XLJ_MTL_TEMPLATE)
#include "mtl_template_mec172xlj.h"
#elif defined(CONFIG_BOARD_MEC172X_MTL_S)
#include "mtl_s_mec172x.h"
#elif defined(CONFIG_BOARD_MEC172X_PTL_UH)
#include "ptl_uh_mec172x.h"
#else
#error "Platform not supported"
#endif /* CONFIG_BOARD_MEC1501MODULAR_ASSY6885 */

#elif defined(CONFIG_SOC_SERIES_NPCX4)
#if defined(CONFIG_BOARD_NPCX4M8F_PTL)
#include "npcx4m8f_aic_rvp_selection.h"
#endif

#endif /* CONFIG_SOC_FAMILY_MEC */

/*
 * 热管理实现在 app/thermal_management/，属 x86 EC app 层业务。
 * AE201 下 app 层整层不编译（见 app/CMakeLists.txt 的 SoC 条件），其
 * include 路径也不存在，故此处必须同步排除 —— 否则任何 include 本头文件
 * 的板级源文件（如 boards/spksilicon/ae201_nto/ae201_nto.c）都会因找不到
 * thermalmgmt.h 而编译失败。
 *
 * 注意区分：这不是「include 路径漏加」，而是「AE201 下这些 API 本就没有
 * 实现」—— 补回 include 路径只会把编译错误推迟成链接错误。
 *
 * 背景：CONFIG_THERMAL_MANAGEMENT=y 来自 prj.conf（x86 EC 业务需求），
 * 该文件无条件加载，AE201 一并继承。本处的 SoC 条件只挡 include，
 * 不改配置值 —— 配置层的 SoC 条件化留待后续单独一轮处理。
 */
#if defined(CONFIG_THERMAL_MANAGEMENT) && !defined(CONFIG_SOC_SERIES_SPK_AE201X)
#include "thermalmgmt.h"
#include "board_thermal.h"
#endif
/**
 * @brief Perform platform configuration depending on the board.
 *
 * @retval 0 If successful, otherwise negative error code.
 */
int board_init(void);

/**
 * @brief Perform platform configuration during suspend depending on the board.
 *
 * Note: Allows to optimize power consumption while the system is in S3/S4/S5.
 *
 * @retval 0 If successful, otherwise negative error code.
 */
int board_suspend(void);

/**
 * @brief Perform platform configuration during resume depending on the board.
 *
 * Note: Allows to restore pin functionality when the system exits S3/S4/S5.
 *
 * @retval 0 If successful, otherwise negative error code.
 */
int board_resume(void);

#endif /* __BOARD_COMMON_H__ */
