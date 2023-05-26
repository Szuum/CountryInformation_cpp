cmake_minimum_required(VERSION 3.22)
project(CountryInformation)

set(CMAKE_CXX_STANDARD 14)

add_executable(CountryInformation main.cpp)

find_package(FLTK REQUIRED)
include_directories(${FLTK_INCLUDE_DIRS})

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

target_link_libraries(CountryInformation PRIVATE ${FLTK_LIBRARIES})

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG db351ffbbadc6c4e9239daaa26e9aefa9f0ec82d) # The commit hash for 1.8.1. Replace with the latest from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

target_link_libraries(CountryInformation PRIVATE cpr::cpr)
