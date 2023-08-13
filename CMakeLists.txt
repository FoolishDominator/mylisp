cmake_minimum_required(VERSION 3.10)

project(mylisp)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pipe -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -std=c++11")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -Wall -g")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall")

# 编译mylisp依赖的静态库
file(GLOB_RECURSE LISP_DEPEND_SRC
        ${PROJECT_SOURCE_DIR}/src/*.c
        )
add_library(lisp_depend STATIC ${LISP_DEPEND_SRC})

# 编译可执行文件
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/mylisp.c)
target_link_libraries(${PROJECT_NAME} lisp_depend)

# add_subdirectory(src)

# add_executable(${PROJECT_NAME} mylisp.c)

# target_link_libraries(${PROJECT_NAME}
#     lispdependency)