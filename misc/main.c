/*
 * Copyright (c) 2026 Sparkle Silicon Technology Corp., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *
 * SPK AE103 最小验证入口：仅让内核 + 启动骨架跑起来，用于核对
 * vector_base/_start/Fix_flash_info 的链接地址。正式的 EC 业务 main
 * （x86 EC 的 app.c，espi/board/task 等）待 SPK 板层/app 层适配后再接入。
 */

#include <zephyr/kernel.h>

void main(void)
{
	while (1) {
		k_sleep(K_SECONDS(1));
	}
}
