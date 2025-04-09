cmake_minimum_required(VERSION 3.10)
project(Xi410_Direct_Solution)

# Define project paths
set(PROJECTS xi410_tcp_daemon xi410_tcp_trigger xi410_usb_trigger)
set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build)

# Ensure the main build directory exists
file(MAKE_DIRECTORY ${BUILD_DIR})

# Process each project
foreach(PROJ ${PROJECTS})
    message(STATUS "Configuring ${PROJ}...")

    # Define build directories for each project
    set(PROJ_SRC_DIR ${CMAKE_SOURCE_DIR}/${PROJ})
    set(PROJ_BUILD_DIR ${PROJ_SRC_DIR}/build)

    # Create build directory
    file(MAKE_DIRECTORY ${PROJ_BUILD_DIR})

    # Run CMake for the project
    execute_process(
        COMMAND ${CMAKE_COMMAND} -S ${PROJ_SRC_DIR} -B ${PROJ_BUILD_DIR}
        RESULT_VARIABLE CMAKE_RESULT
    )
    if(NOT CMAKE_RESULT EQUAL 0)
        message(FATAL_ERROR "CMake configuration failed for ${PROJ}")
    endif()

    # Build the project
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${PROJ_BUILD_DIR} --config Release
        RESULT_VARIABLE BUILD_RESULT
    )
    if(NOT BUILD_RESULT EQUAL 0)
        message(FATAL_ERROR "Build failed for ${PROJ}")
    endif()

    # Copy release binaries & contents to the main build directory
    set(PROJ_RELEASE_DIR ${PROJ_BUILD_DIR}/bin/Release)
    set(DEST_DIR ${BUILD_DIR}/${PROJ})

    file(MAKE_DIRECTORY ${DEST_DIR})
    
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJ_RELEASE_DIR} ${DEST_DIR}
    )
    
    message(STATUS "Copied ${PROJ} release files to ${DEST_DIR}")
endforeach()

message(STATUS "All projects built successfully!")
