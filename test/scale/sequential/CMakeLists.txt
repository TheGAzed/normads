add_library(helper ../helper/helper.c)
target_include_directories(helper PUBLIC ..)

add_executable(scale_sequential_unit main.c
        stack/scale_stack_unit.c
)

target_include_directories(scale_sequential_unit PUBLIC .)

target_link_libraries(scale_sequential_unit PRIVATE greatest ${PROJECT_NAME} helper)
add_test(NAME SCALE_SEQUENTIAL_UNIT_TEST COMMAND scale_sequential_unit)
