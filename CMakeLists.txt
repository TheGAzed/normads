cmake_minimum_required(VERSION 3.22.1)

project(NORMADS VERSION 1 LANGUAGES C)

if (MSVC)
    add_compile_options(/W2 /WX /Zi)
else ()
    add_compile_options(-std=c99 -O0 -g -Wall -Wextra -Wconversion -Werror)
endif ()

add_subdirectory(external)

add_library(${PROJECT_NAME} MODULE)
add_subdirectory(source)
target_include_directories(${PROJECT_NAME} PUBLIC include)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)

include(CTest)
enable_testing()

#add_subdirectory(test)

if (EXISTS "${CMAKE_SOURCE_DIR}/experiment")
#    add_subdirectory(experiment)
endif ()