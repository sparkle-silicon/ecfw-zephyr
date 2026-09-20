# SPDX-License-Identifier: Apache-2.0

if(NOT CONFIG_SOC_SERIES_SPK_AE201X)
# x86 EC board 层实现（board_init/board_devices_check 等），依赖 i2c_hub /
# gpio_ec / flashhdr 等 x86 侧组件。
# AE201 不走这里，而是下方 elseif(CONFIG_SOC_SERIES_SPK_AE201X) 的独立实现
# （boards/spksilicon/）—— 两者互斥，是刻意的并列关系，不是「暂未接入」。
target_sources(app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/board.c
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/board.h
    )
endif()

if(CONFIG_SOC_FAMILY_MEC)

target_sources_ifdef(CONFIG_BOARD_MEC172XLJ_MTL_TEMPLATE app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/microchip/mtl_template_mec172xlj.c
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/microchip/mtl_template_mec172xlj.h
    )

target_sources_ifdef(CONFIG_BOARD_MEC172X_MTL_S app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/microchip/mtl_s_mec172x.c
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/microchip/mtl_s_mec172x.h
    )

target_sources_ifdef(CONFIG_BOARD_MEC172X_PTL_UH app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/microchip/ptl_uh_mec172x.c
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/microchip/ptl_uh_mec172x.h
    )

target_include_directories(app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/microchip
    )

elseif(CONFIG_SOC_SERIES_NPCX4)

include(${CMAKE_CURRENT_LIST_DIR}/nuvoton/npcx4m8f_mecc/CMakeLists.txt)

target_include_directories(app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/nuvoton
    ${CMAKE_CURRENT_LIST_DIR}/nuvoton/npcx4m8f_mecc    )

elseif(CONFIG_SOC_SERIES_SPK_AE201X)

# AE201：板级实现在 spksilicon/ 子目录，形态对齐 nuvoton/npcx4m8f_mecc。
# ⚠️ 本 elseif 必须排在下面 else() 之前 —— else() 是该链的兜底分支，
#    放反了 AE201 会落进 else() 而拿不到源文件，且不会有任何报错。
include(${CMAKE_CURRENT_LIST_DIR}/spksilicon/ae201_nto/CMakeLists.txt)

target_include_directories(app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/spksilicon
    ${CMAKE_CURRENT_LIST_DIR}/spksilicon/ae201_nto
    )

else()

# SPK 等其它 SoC：board 层公共头（board.h / board_config.h）同样要可见，
# 只是无 MEC/NPCX 专属子目录。原文件仅覆盖 MEC/NPCX4 两个分支，SPK 接入时补此分支。
target_include_directories(app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    )

endif()