# Fundamental settings
cmake_minimum_required(VERSION 3.3)
project(TEST_PLATFORM)
enable_testing()

#=================================
#= Paths
#==================================
# Main project directiories
set(CURRENT_PATH ${CMAKE_SOURCE_DIR})
set(TOOLS_PATH ${CURRENT_PATH}/../tools)
set(CUBE_PATH ${CURRENT_PATH}/../source/NUCLEO-F44RE_Gizmo)

# Tools 
set(UNITY_PATH ${TOOLS_PATH}/unity/src)
set(UNITY_EXTRAS_PATH ${TOOLS_PATH}/unity/extras/)
set(UNITY_FIXTURE_PATH ${UNITY_EXTRAS_PATH}/fixture/src)
set(UNITY_MEMORY_PATH ${UNITY_EXTRAS_PATH}/memory/src)
set(FFF_PATH ${TOOLS_PATH}/fff)

#=================================
#= Libraries
#==================================
# Base unity
add_library(unity STATIC ${UNITY_PATH}/unity.c)
target_include_directories(unity PUBLIC ${UNITY_PATH})

# Unity memory
add_library(unity_memory STATIC ${UNITY_MEMORY_PATH}/unity_memory.c)
target_include_directories(unity PUBLIC 
    ${UNITY_MEMORY_PATH}
)
target_link_libraries(unity_memory unity)

# Unity fixture
add_library(unity_fixture STATIC ${UNITY_FIXTURE_PATH}/unity_fixture.c)
target_include_directories(unity_fixture PUBLIC 
    ${UNITY_FIXTURE_PATH}
)
target_link_libraries(unity_fixture unity_memory unity)

#=================================
#= Linking and building
#==================================
# Searching for all tests folders
file(GLOB test_list RELATIVE ${CURRENT_PATH} ${CURRENT_PATH}/test_*)

message(STATUS "==================================")
message(STATUS "List of all test modules:")
foreach(test_dir ${test_list})
    message(STATUS ${test_dir})
    add_subdirectory(${test_dir})
endforeach()
message(STATUS "==================================")

# Building all tests
#add_executable(all_tests Alltests.c)
#target_link_libraries(all_tests unity_fixture)