cmake_minimum_required(VERSION 3.10)

# Set the project name (used as the executable name)
project(xi410_tcp_daemon)

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
	user/src/irDirectDaemon.cpp
)

# Include directories
include_directories(
    user/inc   # Headers from user/inc
    ../sdk        # Headers from sdk
	../sdk/pif_config
	../sdk/extras
)


# Detect OS and configure accordingly
if(WIN32)
    message(STATUS "Configuring for Windows")
	
	# Determine whether we are building for 32-bit or 64-bit
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(SDK_LIB_DIR "../sdk/x64")
	else()
		set(SDK_LIB_DIR "../sdk/Win32")
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
			"../config/Formats.def"
			"../config/generic.xml"
			"$<TARGET_FILE_DIR:${PROJECT_NAME}>"  # Destination: Same folder as the executable
	)

	# Set the command-line argument for Visual Studio Debugging
	set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_COMMAND_ARGUMENTS "-c generic.xml")
elseif(UNIX)
    message(STATUS "Configuring for Linux")

    # Create the executable
    add_executable(${PROJECT_NAME} ${SOURCE_FILES})

    # Link against libirimager.so in /usr/lib
    target_link_libraries(${PROJECT_NAME} 
		/usr/lib/libirimager.so
		/usr/lib/libirdirectsdk.so
		-lstdc++fs  # Ensure correct C++ standard library
        -static-libstdc++  # Try static linking if needed
	)

    # Ensure necessary files are copied next to the executable
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "../config/Formats.def" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "../config/generic.xml" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    )

    # Set debugging arguments
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_COMMAND_ARGUMENTS "-c \"$<TARGET_FILE_DIR:${PROJECT_NAME}>/generic.xml\"")
endif()