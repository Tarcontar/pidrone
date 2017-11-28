set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm-eabi)

set(triple arm-none-eabi)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER_TARGET ${triple})
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_TARGET ${triple})

set(SHARED_FLAGS "-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16")
set(CMAKE_C_FLAGS "${SHARED_FLAGS} -lm -lc -Os -g -Wall")
set(CMAKE_CXX_FLAGS "${SHARED_FLAGS} -lm -fno-builtin -Wal")
set(CMAKE_ASM_FLAGS "${SHARED_FLAGS} -g -Wa,--no-warn -x assembler-with-cpp")