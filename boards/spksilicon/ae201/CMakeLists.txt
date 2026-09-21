# SPDX-License-Identifier: Apache-2.0
#
# AE201 板级实现接入点（由 boards/CMakeLists.txt 的
# elseif(CONFIG_SOC_SERIES_SPK_AE201X) 分支 include 进来）。
# 形态对齐 nuvoton/npcx4m8f_mecc/CMakeLists.txt。

target_sources_ifdef(CONFIG_BOARD_SPK32_AE201 app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/ae201.c
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/ae201.h
    )
