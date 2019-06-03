set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm-eabi)

set(TRIPLE arm-none-eabi)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_C_COMPILER ${TRIPLE}-gcc)
set(CMAKE_CXX_COMPILER ${TRIPLE}-g++)
set(CMAKE_ASM_COMPILER ${TRIPLE}-as)
set(CMAKE_OBJCOPY ${TRIPLE}-objcopy)
set(CMAKE_OBJDUMP ${TRIPLE}-objdump)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT}")

set(SHARED_FLAGS "-specs=nano.specs -specs=nosys.specs -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -fshort-wchar")
set(CMAKE_C_FLAGS "${SHARED_FLAGS} -Os -g -Wall -s")
set(CMAKE_CXX_FLAGS "${SHARED_FLAGS} -Os -fno-builtin -Wall -s")
set(CMAKE_ASM_FLAGS "${SHARED_FLAGS} -g -Wa,--no-warn -x assembler-with-cpp")

SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--no-wchar-size-warning -Wl,--gc-sections -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -nostartfiles -L${LINKER_SCRIPT_DIR} -T ${CMAKE_CURRENT_LIST_DIR}/linker.ld")
