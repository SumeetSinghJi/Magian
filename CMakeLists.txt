cmake_minimum_required(VERSION 3.19)
project(Magian)

# ADDING HEADERS
file(GLOB_RECURSE HEADER_FILES headers/*.h)

# ADDING ASSETS
function(collect_files_with_extensions dir extensions)
    file(GLOB_RECURSE FILES "${dir}/*${extensions}")
    list(APPEND SOURCES ${FILES})
endfunction()

# OS Specific
if(WIN32)
    # Windows-specific configurations
    set(WINDOWS_DESKTOP_DIR "$ENV{USERPROFILE}/Desktop")
    collect_files_with_extensions(assets "/*.docx" "/*.pdf")
    collect_files_with_extensions(images "/*.png" "/*.jpg")
    collect_files_with_extensions(sound/music "/*.wav" "/*.mp3")
    list(APPEND SOURCES staging.cpp ${HEADER_FILES})
    set(LINK_LIBRARIES winmm)
    set(RESOURCE_FILES images/icons/fire.ico)  # Add this line to include the .ico file
elseif(UNIX)
    # UNIX-specific configurations
    if(APPLE)
        # macOS-specific configurations
        set(UNIX_DESKTOP_DIR "$ENV{HOME}/Desktop")
    else()
        # Linux-specific configurations
        set(UNIX_DESKTOP_DIR "$ENV{HOME}/bin")
    endif()
    collect_files_with_extensions(assets "/*.docx" "/*.pdf")
    collect_files_with_extensions(images "/*.png" "/*.jpg")
    collect_files_with_extensions(sound "/*.wav" "/*.mp3")
    list(APPEND SOURCES staging.cpp ${HEADER_FILES})
else()
    message(FATAL_ERROR "Unsupported operating system: ${CMAKE_SYSTEM_NAME}")
endif()

# Add the include directories
include_directories(headers)

# COMPILING
add_executable(magian ${SOURCES} ${RESOURCE_FILES})  # Include the RESOURCE_FILES variable

# Link libraries
target_link_libraries(magian PRIVATE ${LINK_LIBRARIES})

# Platform-specific configurations
if(WIN32)
    # Windows-specific configurations
    set_target_properties(magian PROPERTIES OUTPUT_NAME magian)

    # Create a resource file with the icon
    set(RESOURCE_FILE ${CMAKE_CURRENT_BINARY_DIR}/magian.rc)
    file(WRITE ${RESOURCE_FILE} "1 ICON \"${CMAKE_CURRENT_SOURCE_DIR}/images/icons/fire.ico\"")

    # Add the resource file to the target
    target_sources(magian PRIVATE ${RESOURCE_FILE})

    # Configure the manifest file with the icon reference
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/magian.manifest.in
        ${CMAKE_CURRENT_BINARY_DIR}/magian.manifest
        @ONLY
    )
    
    # Add the manifest file as a source to the target
    target_sources(magian PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/magian.manifest)

    # INSTALLATION
    add_custom_command(TARGET magian POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:magian>" "${WINDOWS_DESKTOP_DIR}/magian.exe"
        COMMENT "Copying executable to desktop directory"
    )
elseif(UNIX)
    # UNIX-specific configurations
    set_target_properties(magian PROPERTIES OUTPUT_NAME magian)

    # INSTALLATION
    add_custom_command(TARGET magian POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:magian>" "${UNIX_DESKTOP_DIR}/magian"
        COMMENT "Copying executable to desktop directory"
    )
endif()
