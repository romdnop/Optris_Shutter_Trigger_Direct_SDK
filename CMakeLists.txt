cmake_minimum_required(VERSION 3.10)

# Set the project name (used as the executable name)
project(xi410_get_image)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable compiler-specific options for MSVC
if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)  # Avoid unsafe function warnings
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)  # Place .exe in bin/
endif()

add_definitions(-DUNICODE -D_UNICODE)


# Add source files
set(SOURCE_FILES 
    user/src/irimagerShowOOP.cpp 
    user/src/IRImagerHandler.cpp
	user/src/VideoDisplay.cpp
)

# Include directories
include_directories(
    user/inc   # Headers from user/inc
    sdk        # Headers from sdk
	sdk/pif_config
	sdk/extras
)


# Determine whether we are building for 32-bit or 64-bit
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SDK_LIB_DIR "sdk/x64")
else()
    set(SDK_LIB_DIR "sdk/Win32")
endif()


# Link the SDK library files
link_directories(${SDK_LIB_DIR})

# Find all .lib files in the sdk folder
file(GLOB SDK_LIBS ${SDK_LIB_DIR}/*.lib)


# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)



# Create the executable with the project name
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link the found .lib files to the executable
target_link_libraries(${PROJECT_NAME} ${SDK_LIBS})


# Ensure the DLL is copied next to the executable
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "../${SDK_LIB_DIR}/libirimager.dll"  # Source DLL
		"../Formats.def"
		"../generic.xml"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"  # Destination: Same folder as the executable
)