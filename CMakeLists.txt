cmake_minimum_required(VERSION 3.16)
project(2026GKD_week4_part2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(2026GKD_week4_part2
    main.cpp
    TaskManager.cpp
    TaskBase.cpp
)

target_include_directories(2026GKD_week4_part2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(2026GKD_week4_part2 PRIVATE Threads::Threads)
