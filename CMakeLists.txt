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
    set(WINDOWS_DESKTOP_DIR "$ENV{USERPROFILE}/desktop")
    collect_files_with_extensions(assets "/*.docx" "/*.pdf")
    collect_files_with_extensions(images "/*.png" "/*.jpg")
    collect_files_with_extensions(sound "/*.wav" "/*.mp3")
    list(APPEND SOURCES staging.cpp ${HEADER_FILES})
    set(LINK_LIBRARIES winmm)
    set(RESOURCE_FILES images/icons/fire.ico)  # Add this line to include the .ico file
elseif(UNIX)
    # Linux-specific configurations
    set(LINUX_DESKTOP_DIR "$ENV{HOME}/bin")
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

    # Disable manifest generation
    if (MSVC)
        set_target_properties(magian PROPERTIES LINK_FLAGS "/MANIFEST:NO")
    endif()
elseif(UNIX)
    # Linux-specific configurations
    set_target_properties(magian PROPERTIES OUTPUT_NAME magian)
else()
    message(FATAL_ERROR "Unsupported operating system: ${CMAKE_SYSTEM_NAME}")
endif()

# INSTALLATION
if(WIN32)
    install(TARGETS magian DESTINATION ${WINDOWS_DESKTOP_DIR})
elseif(UNIX)
    install(TARGETS magian DESTINATION ${LINUX_DESKTOP_DIR})
endif()
