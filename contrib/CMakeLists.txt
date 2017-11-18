file(GLOB_RECURSE LC_SRC ${PROJECT_SOURCE_DIR} *.c)

add_avr_library(contrib ${LC_SRC})
