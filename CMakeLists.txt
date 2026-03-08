cmake_minimum_required(VERSION 3.22)

project(LightingSystem)

# Project Rules: Baseline C++20, C++23 encouraged.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# Core Library
# ============================================================================
# Tách phần logic cốt lõi ra thành một thư viện để dễ tái sử dụng
add_library(LightingCore STATIC
    src/LightSchedule.cpp
    src/DimmingEngine.cpp
    src/LightingStrategy.cpp
    src/helper/DateTime.cpp
)

target_include_directories(LightingCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ============================================================================
# Main Application
# ============================================================================
add_executable(LightingSystem src/Main.cpp)
target_link_libraries(LightingSystem PRIVATE LightingCore)

# ============================================================================
# Tests / Tools
# ============================================================================
# Tạo thư mục con trong build cho các file test/tool
set(TEST_OUTPUT_DIR ${CMAKE_BINARY_DIR}/tests)

add_executable(TestSchedule tests/TestSchedule.cpp)
target_link_libraries(TestSchedule PRIVATE LightingCore)

# Đặt output của TestSchedule vào folder build/tests
set_target_properties(TestSchedule PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

# ============================================================================
# Compiler Options
# ============================================================================
if(MSVC)
    target_compile_options(LightingCore PRIVATE /W4)
    target_compile_options(LightingSystem PRIVATE /W4)
else()
    target_compile_options(LightingCore PRIVATE -Wall -Wextra)
    target_compile_options(LightingSystem PRIVATE -Wall -Wextra)
endif()
