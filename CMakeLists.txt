cmake_minimum_required(VERSION 3.10)
project(MyProject VERSION 0.3.2 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wpedantic -pedantic-errors -Wuninitialized -Wzero-as-null-pointer-constant)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/std:c++17)
endif()

add_definitions(-DQT_DEPRECATED_WARNINGS)
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)
add_definitions(-DVERSION="${PROJECT_VERSION}")

find_package(Qt5 COMPONENTS Core Widgets Sql REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/src/interface/ui)

# file(GLOB_RECURSE SOURCE_FILES "src/**/*" "resources/**/*")

# # add_executable(MyProject ${SOURCE_FILES})
# add_executable(${PROJECT_NAME})
# target_sources(${PROJECT_NAME} PRIVATE src/main.cpp)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.h")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(MyProject Qt5::Core Qt5::Widgets Qt5::Sql)

install(TARGETS MyProject)
