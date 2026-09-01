if(CONFIG_SOC_SERIES_SPK_AE103X)
# SPK AE103 最小验证：只编启动骨架（startup.S/flash_info.c）+ minimal main。
# x86 EC 的 misc 业务（flashhdr/softstrap/task_handler）待板层适配后再接。
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
