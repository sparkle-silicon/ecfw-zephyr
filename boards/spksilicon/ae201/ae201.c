/*
 * Copyright (c) 2026 Sparkle Silicon Technology Corp., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file ae201.c
 * @brief AE201 板级实现（board_init / board_suspend / board_resume）。
 *
 * 【与 x86 EC 侧 board.c 的关系 —— 并列，不是继承】
 *
 *   boards/board.c        ← 依赖 i2c_hub / gpio_ec / flashhdr / errcodes
 *   boards/spksilicon/    ← 本文件，自包含
 *
 *   boards/CMakeLists.txt 用互斥条件把两者分开（board.c 只在
 *   NOT CONFIG_SOC_SERIES_SPK_AE201X 时编译）。因此本文件**不能**
 *   引用 i2c_hub_read / gpio_ec_config / major_version 一类的 x86 侧符号，
 *   它们在本配置下根本不存在 —— 编译期就会失败。
 *
 * 【接入状态】
 *
 *   本文件当前**无调用者**：AE201 的 main 在 misc/main.c，尚未调用
 *   board_init()；x86 侧调用点 app/app.c:52 在 AE201 下整层不编译。
 *   所以本轮的作用是解掉 board_init/suspend/resume「已声明未定义」的债
 *   （声明见 boards/board_config.h:55/64/73），并按标准范式把接入点摆好，
 *   待 app 层外设就绪后按里程碑接入。
 *
 *   预期副作用为零：app 是静态库，未被引用的 .o 不会被链入最终镜像，
 *   故固件二进制应逐字节不变（构建时间戳除外）—— 这本身就是验证点。
 */

#include <errno.h>
#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>

#include "board.h"
#include "board_config.h"
#include "ae201.h"

LOG_MODULE_REGISTER(board, CONFIG_BOARD_LOG_LEVEL);

/*
 * ============================ 板级设备就绪检查 ============================
 *
 * 策略：**严格** —— 对齐 x86 侧 board.c:46-78 的行为。
 * 任一设备 not ready 即 return -ENODEV，拒绝启动。
 *
 * 为什么选严格（2026-09-20 定夺）：
 *   失败在启动时暴露，而不是推迟到使用点。宽松策略虽然 bring-up 阶段
 *   不容易被无关外设卡住，但会掩盖真实的初始化失败 —— 一个没就绪的外设
 *   要等到业务真正用它时才炸，那时的现场信息远不如启动时干净。
 *
 * ⚠️ 检查范围（2026-09-20 实测，勿凭直觉推断）：
 *   这里的「设备」指 z_device_get_all_static() 返回的静态设备，即 DT 中
 *   status="okay" **且驱动已编译**的节点。status="disabled" 的节点根本
 *   不会生成 device，也就不进遍历 —— 不会因为「DT 里有 disabled 节点」
 *   而误伤。
 *
 *   当前 AE201 共 7 个：gpioa~gpioe(5) + uart0 + timer0。
 *     - uart0 是标签名，实际节点为 serial@5800（dtsi 中标签序为
 *       uarta/uartb/uart0/uart1，前两个才是 16x 过采样的 A/B 口）
 *     - 未生成 device 的：flash0 / sram0（无驱动实例化）、
 *       uarta / uartb / uart1（status="disabled"）
 *
 *   所以真正会触发拒绝启动的是「节点 okay 但 init 失败」—— 那本就是该报
 *   的错。需要注意的只是新外设若其 init 有前置依赖（时钟 / 电源域），
 *   要确认在 board_init() 调用点之前已满足。
 *
 * 与 x86 版的差异：x86 版遍历结束后会调用 board_dts_pin_muxing() 应用
 *   zephyr,user 节点的 pinctrl；AE201 当前 DTS 无该节点，故省略。
 *   待需要板级 pinctrl 时按 boards/board.c:34-44 的形态补上。
 */
static int ae201_board_devices_check(void)
{
	const struct device *dev;
	const struct device *devlist_end;
	size_t ndevs;

	ndevs = z_device_get_all_static(&dev);

	if ((ndevs == 0) || (dev == NULL))
	{
		LOG_ERR("No devices initialized!");
		return -ENODEV;
	}

	devlist_end = dev + ndevs;

	while (dev < devlist_end)
	{
		if ((dev->name != NULL) && (strlen(dev->name) != 0))
		{
			if (z_device_is_ready(dev))
			{
				LOG_DBG("%s ready", dev->name);
			}
			else
			{
				LOG_WRN("%s not ready. Check dts", dev->name);
				printk("%s not ready. Check dts\n", dev->name);
				return -ENODEV;
			}
		}
		else
		{
			LOG_WRN("Device with no name");
		}

		dev++;
	}

	return 0;
}

int board_init(void)
{
	int ret;

	LOG_INF("%s board init", AE201_BOARD_MODEL);

	ret = ae201_board_devices_check();
	if (ret)
	{
		LOG_ERR("Board device check failed: %d", ret);
		return ret;
	}

	/*
	 * TODO(里程碑)：app 层外设就绪后在此追加板级初始化。
	 *   对照 x86 侧 npcx4m8f_aic_on_ptl.c 的 board_init() 顺序：
	 *     - 板级 GPIO 引脚表配置（对应 gpio_configure_array 的位置）
	 *     - 板级 ID 读取（对应 read_board_id / update_platform_sku_type）
	 *   引脚表建在 ae201.h，形态对齐 mecc_npcx9_cfg。
	 */

	return 0;
}

int board_suspend(void)
{
	/*
	 * TODO(里程碑)：AE201 低功耗路径尚未实现。
	 *   接入时在此配置 S3/S4/S5 下的引脚状态，形态对齐 x86 侧
	 *   npcx4m8f_aic_on_ptl.c 的 mecc_npcx9_cfg_sus 引脚表。
	 *   注意 AE201 的 iRAM0 是独立电源域，低功耗时可关（见内存布局备忘）。
	 */

	return 0;
}

int board_resume(void)
{
	/*
	 * TODO(里程碑)：与 board_suspend() 成对，形态对齐
	 *   x86 侧 npcx4m8f_aic_on_ptl.c 的 mecc_npcx9_cfg_res 引脚表。
	 */

	return 0;
}
