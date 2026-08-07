cmake_minimum_required(VERSION 3.15)
project(wikicpp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(WIKICPP_BUILD_EXAMPLES "Build example programs" ON)
option(WIKICPP_BUILD_TESTS "Build unit tests" OFF)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

add_library(wikicpp INTERFACE)
target_include_directories(wikicpp INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(wikicpp INTERFACE CURL::libcurl nlohmann_json::nlohmann_json)
target_compile_definitions(wikicpp INTERFACE WIKICPP_HEADER_ONLY)

if(WIKICPP_BUILD_EXAMPLES)
    add_executable(search_example examples/search_example.cpp)
    target_link_libraries(search_example wikicpp)
endif()

install(DIRECTORY include/ DESTINATION include)
install(TARGETS wikicpp EXPORT wikicppTargets)
install(EXPORT wikicppTargets DESTINATION lib/cmake/wikicpp)
