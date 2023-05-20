    cmake_minimum_required(VERSION 3.26)  # Minimum required CMake version

    project(Magian)  # Project name
    
    # Rest of your CMake commands and configurations
    if (WIN32)
        set(RESOURCE_FILES ${CMAKE_SOURCE_DIR}//images//icons//flame_light.ico)
        include_directories(${CMAKE_SOURCE_DIR})  # Add the directory where the header file is located
        add_executable(${PROJECT_NAME} WIN32 ${RESOURCE_FILES} staging.cpp) # will output /build/magian.exe
        target_link_libraries(${PROJECT_NAME} PRIVATE winmm) # for playing windows audio
    else()
        # Linux-specific configuration
        set(LINUX_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/magian/linux.toolchain.cmake")
        if (EXISTS ${LINUX_TOOLCHAIN_FILE})
            include(${LINUX_TOOLCHAIN_FILE})
        else()
            message(FATAL_ERROR "Could not find the Linux toolchain file: ${LINUX_TOOLCHAIN_FILE}")
        endif()
        include_directories(${CMAKE_SOURCE_DIR})  # Add the directory where the header file is located
        add_executable(${PROJECT_NAME} staging.cpp)
    endif()
    


    