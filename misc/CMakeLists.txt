if(CONFIG_SOC_SERIES_SPK_AE201X)
# AE201：只编启动骨架（startup.S / flash_info.c）+ 最小 main。
#
# ===================== 里程碑 TODO：misc 层补齐 =====================
# 下面 x86 分支里的 flashhdr / softstrap / task_handler 三项，分别对应：
#   flashhdr      Flash 头解析（major_version / minor_version，被 board.c 的
#                 read_board_id 依赖）
#   softstrap     软跳线
#   task_handler  任务调度框架
# 三者中 flashhdr 是 boards/board.c 的硬依赖 —— 但 AE201 走的是
# boards/spksilicon/ 的独立板级实现，不依赖它，故非阻塞。
# 待 app 层接入时按其真实依赖逐个补齐。
target_sources(app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/startup.S
    ${CMAKE_CURRENT_LIST_DIR}/flash_info.c
    ${CMAKE_CURRENT_LIST_DIR}/main.c
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/flash_info.h
    )
else()
target_sources(app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/flashhdr.c
    ${CMAKE_CURRENT_LIST_DIR}/softstrap.c
    ${CMAKE_CURRENT_LIST_DIR}/task_handler.c
    ${CMAKE_CURRENT_LIST_DIR}/startup.S
    ${CMAKE_CURRENT_LIST_DIR}/flash_info.c
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/flashhdr.h
    ${CMAKE_CURRENT_LIST_DIR}/softstrap.h
    ${CMAKE_CURRENT_LIST_DIR}/task_handler.h
    ${CMAKE_CURRENT_LIST_DIR}/memops.h
    ${CMAKE_CURRENT_LIST_DIR}/flash_info.h
    )
endif()

target_include_directories(app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    )
