cmake_minimum_required(VERSION 3.14)
project(ipapi-cpp)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch the latest Git tag as the project version
find_package(Git REQUIRED)
execute_process(
    COMMAND git describe --tags --abbrev=0
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_TAG
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(GIT_TAG)
    string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)" VERSION_MATCH ${GIT_TAG})
    if(VERSION_MATCH)
        set(PROJECT_VERSION ${GIT_TAG})
    else()
        set(PROJECT_VERSION "0.0.0") # Default version if no valid tag is found
    endif()
else()
    set(PROJECT_VERSION "0.0.0") # Default version if Git is unavailable
endif()

# Display the version
message(STATUS "Project version: ${PROJECT_VERSION}")

# Configure the version header
configure_file(
    ${CMAKE_SOURCE_DIR}/include/version.h.in
    ${CMAKE_BINARY_DIR}/version.h
)
include_directories(${CMAKE_BINARY_DIR})

# Include GoogleTest via FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Find and link libcurl
find_package(CURL REQUIRED)

# Add your ipapi-cpp library (example)
add_library(ipapi-cpp STATIC
  src/ipapi.cpp  # Update with your source files
)

# Add include and source files
target_sources(ipapi-cpp
    PRIVATE
        src/ipapi.cpp
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Specify the include directory
target_include_directories(ipapi-cpp
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Link curl to ipapi-cpp
target_link_libraries(ipapi-cpp
  PRIVATE
    CURL::libcurl  # Link the curl library
)

# Enable testing
enable_testing()

# Add your test executable
add_executable(ipapi-test
  test/ipapi_test.cpp  # Update with your test files
)

# Link the test executable with GoogleTest, ipapi-cpp library, and curl
target_link_libraries(ipapi-test
  gtest gtest_main
  ipapi-cpp
  CURL::libcurl  # Link curl to your test executable
)

# Automatically discover tests
include(GoogleTest)
gtest_discover_tests(ipapi-test)
