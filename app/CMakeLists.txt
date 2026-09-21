# SPDX-License-Identifier: Apache-2.0

if(NOT CONFIG_SOC_SERIES_SPK_AE201X)
# x86 EC app 层业务，main 在 app/app.c（board_devices_check → espihub_init
# → board_init → strap_init → start_all_tasks 的 EC 标准启动序）。
target_sources(app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/app.c
    )

include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/app/peripheral_management)
include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/app/power_sequencing)
include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/app/dnx)
include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/app/power_management)
include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/app/smchost)
include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/app/thermal_management)
include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/app/debug)
include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/app/kbchost)
include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/app/saf)
include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/app/soc_debug_awareness)

include(${CMAKE_CURRENT_LIST_DIR}/peripheral_management/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/power_sequencing/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/dnx/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/soc_debug_awareness/CMakeLists.txt)

include(${CMAKE_CURRENT_LIST_DIR}/smchost/CMakeLists.txt)

include(${CMAKE_CURRENT_LIST_DIR}/thermal_management/CMakeLists.txt)

include(${CMAKE_CURRENT_LIST_DIR}/debug/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/kbchost/CMakeLists.txt)


include(${CMAKE_CURRENT_LIST_DIR}/saf/CMakeLists.txt)

target_include_directories(app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    )

else()

# ===================== 里程碑 TODO：AE201 app 层接入 =====================
# 当前 AE201 不编译 app/ 下任何业务源 —— 入口是 misc/main.c 的最小骨架
# （printk + k_sleep 循环），够验证 UART console 链路，还没有 EC 业务。
#
# 待 SoC 层外设（eSPI / KBC / 电源时序 / 热管理 …）就绪后，按模块逐轮接入：
#   1. 把各模块目录的 CMakeLists.txt include 进来（对照上方 x86 分支的清单）
#   2. 目标是让 misc/main.c 换成这里的 app.c —— 即 EC 标准启动序：
#      board_devices_check → espihub_init → board_init → strap_init
#      → start_all_tasks
#   3. 届时 board_init() 由 boards/spksilicon/ae201/ae201.c 提供
#
# 现在只给 include 路径，保证接入时 board.h / board_config.h 已可见。
target_include_directories(app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    )
endif()
