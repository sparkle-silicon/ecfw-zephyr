# SPDX-License-Identifier: Apache-2.0

if(NOT CONFIG_SOC_SERIES_SPK_AE103X)
# x86 EC board 层实现（board_init/board_devices_check 等），SPK 最小验证暂不接入。
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

else()

# SPK 等其它 SoC：board 层公共头（board.h / board_config.h）同样要可见，
# 只是无 MEC/NPCX 专属子目录。原文件仅覆盖 MEC/NPCX4 两个分支，SPK 接入时补此分支。
target_include_directories(app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    )

endif()