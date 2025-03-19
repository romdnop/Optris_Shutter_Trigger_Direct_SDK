cmake_minimum_required(VERSION 3.10)

# Set the project name (used as the executable name)
project(xi410_get_image)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the source files
set(SOURCE_FILES user/src/irimagerShowOOP.cpp)

# Include directories
include_directories(
    user/inc   # Headers from user/inc
    sdk        # Headers from sdk
)

# Create the executable with the project name
add_executable(${PROJECT_NAME} ${SOURCE_FILES})