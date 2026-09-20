# SPDX-License-Identifier: Apache-2.0

# SPK AE201：本目录均为 x86 EC 外设驱动（eSPI/PS2/KSCAN/PECI/ADC/风扇/LED 等），
# AE201 侧由 SoC 层驱动承接（west/zephyr_fork/soc/riscv/spksilicon/ae201/），
# 故下述源文件在 AE201 构建中不参与编译。
#
# 但 include 路径必须无条件生效（见文件末尾）：板级公共代码 boards/board.c 会引用
# 这里的头文件（i2c_hub.h / gpio_ec.h 等），若把 include 路径一并圈进条件块，
# AE201 下这些头文件将从搜索路径中消失，导致板层无法编译。
if(NOT CONFIG_SOC_SERIES_SPK_AE201X)

if (CONFIG_PS2_KEYBOARD OR CONFIG_PS2_MOUSE)
target_sources(app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/ps2kbaux.c
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/ps2kbaux.h
    )
endif()

target_sources_ifdef(CONFIG_KSCAN_EC app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/kbs_matrix.c
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/kbs_matrix.h
    ${CMAKE_CURRENT_LIST_DIR}/kbs_keymap.h
    )

if (CONFIG_KSCAN_EC OR CONFIG_PS2_KEYBOARD)
    if (CONFIG_EC_GTECH_KEYBOARD)
        target_sources(app
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/gtech_keymap.c
        )
    endif()
    if (CONFIG_EC_FUJITSU_KEYBOARD)
        target_sources(app
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fujitsu_keymap.c
        )
    endif()
endif()

target_sources_ifdef(CONFIG_POSTCODE_MANAGEMENT app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/max6958.c
    ${CMAKE_CURRENT_LIST_DIR}/port80display.c
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/max6958.h
    ${CMAKE_CURRENT_LIST_DIR}/port80display.h
    )

target_sources_ifdef(CONFIG_THERMAL_MANAGEMENT app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/adc_sensors.c
    ${CMAKE_CURRENT_LIST_DIR}/peci_hub.c
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/adc_sensors.h
    ${CMAKE_CURRENT_LIST_DIR}/peci_hub.h
    )

target_sources(app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/eeprom.c
    ${CMAKE_CURRENT_LIST_DIR}/i2c_hub.c
    ${CMAKE_CURRENT_LIST_DIR}/espi_hub.c
    ${CMAKE_CURRENT_LIST_DIR}/espioob_mngr.c
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/eeprom.h
    ${CMAKE_CURRENT_LIST_DIR}/i2c_hub.h
    ${CMAKE_CURRENT_LIST_DIR}/espi_hub.h
    ${CMAKE_CURRENT_LIST_DIR}/espioob_mngr.h
    ${CMAKE_CURRENT_LIST_DIR}/gpio_ec.h
    ${CMAKE_CURRENT_LIST_DIR}/fan.h
    ${CMAKE_CURRENT_LIST_DIR}/led.h
    ${CMAKE_CURRENT_LIST_DIR}/vci.h
    ${CMAKE_CURRENT_LIST_DIR}/acpi.h
    )

target_sources_ifdef(CONFIG_SOC_FAMILY_MEC app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/led.c
    PUBLIC
    )

target_sources_ifdef(CONFIG_SOC_SERIES_MEC1501X app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/gpio_mec.c
    ${CMAKE_CURRENT_LIST_DIR}/fan_mec15xx.c
    ${CMAKE_CURRENT_LIST_DIR}/vci_mec15xx.c
    ${CMAKE_CURRENT_LIST_DIR}/acpi_mec15xx.c
    PUBLIC
    )

target_sources_ifdef(CONFIG_SOC_SERIES_MEC172X app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/gpio_mec.c
    ${CMAKE_CURRENT_LIST_DIR}/fan_mec15xx.c
    # Require Soc-HW access via device tree
    ${CMAKE_CURRENT_LIST_DIR}/vci_mec172x.c
    ${CMAKE_CURRENT_LIST_DIR}/acpi_mec172x.c
    PUBLIC
    )

target_sources_ifdef(CONFIG_SOC_NPCX4M8F app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/gpio_npcx.c
    ${CMAKE_CURRENT_LIST_DIR}/fan_npcx.c
    ${CMAKE_CURRENT_LIST_DIR}/led.c
    ${CMAKE_CURRENT_LIST_DIR}/acpi_npcx.c
    PUBLIC
    )

endif() # NOT CONFIG_SOC_SERIES_SPK_AE201X

# SPDX AE201：include 路径无条件生效（刻意放在条件块之外）。
# board 层公共代码引用的头文件（i2c_hub.h / gpio_ec.h / eeprom.h 等）需始终可见，
# 与源文件是否参与编译无关。
target_include_directories(app
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    )
