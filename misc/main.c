/*
 * Copyright (c) 2026 Sparkle Silicon Technology Corp., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *
 * SPK AE103 启动入口。当前是「最小验证骨架」，对应 EC 标准启动流程的
 * 最前段：UART console 早期就位（本文件最上面那句 printk 即验证点），
 * 之后才轮到中断配置、内存初始化、外设初始化、主循环 —— 与裸机固件
 * KERNEL_MAIN.c 的 main() 顺序一致（config_irqc → 开中断 → init_memory
 * → 外设 init → main_loop）。
 *
 * 正式的 EC 业务 main（x86 EC 的 app/app.c：board_devices_check →
 * espihub_init → board_init → strap_init → start_all_tasks）待 SPK
 * 板层 / app 层适配后再接入，届时替换本文件。
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

void main(void)
{
	/* UART console 验证：若 printk 能经 UART0 吐出，说明 serial console
	 * 链路（驱动 → chosen console → CONFIG_UART_CONSOLE）已通。 */
	printk("EC FW Zephyr boot: %s\n", CONFIG_BOARD);

	while (1) {
		k_sleep(K_SECONDS(1));
	}
}
