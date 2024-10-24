get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Define test build
add_executable(${DIR_NAME} 
    ${DIR_NAME}.c 
    ${DIR_NAME}_runner.c
)

# Link needed libraries
target_link_libraries(${DIR_NAME} unity_fixture)
target_include_directories(${DIR_NAME} PUBLIC ${FFF_PATH})

# Adding test
add_test(${DIR_NAME}Test ${DIR_NAME})
