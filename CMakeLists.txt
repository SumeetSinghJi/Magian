cmake_minimum_required(VERSION 3.19)
project(sandbox)

# ADDING SOURCE FILES
set(SOURCES staging.cpp)

# ADDING HEADERS
file(GLOB_RECURSE HEADER_FILES headers/*.h)
list(APPEND SOURCES ${HEADER_FILES})

# ADDING ASSETS
function(collect_files_with_extensions dir extensions)
    file(GLOB_RECURSE FILES "${dir}/*${extensions}")
    list(APPEND SOURCES ${FILES})
endfunction()
# (folder location and extensions)
collect_files_with_extensions(assets "/*.docx" "/*.pdf")
collect_files_with_extensions(images "/*.png" "/*.jpg")
collect_files_with_extensions(sound "/*.wav" "/*.mp3")

# COMPILING
add_executable(sandbox ${SOURCES})

# OS Specific
if(WIN32)
    set_target_properties(sandbox PROPERTIES OUTPUT_NAME sandbox_win)
    # set(WINDOWS_DESKTOP_DIR "$ENV{USERPROFILE}/desktop")
    install(TARGETS sandbox DESTINATION ${WINDOWS_DESKTOP_DIR})
elseif(UNIX)
    set_target_properties(sandbox PROPERTIES OUTPUT_NAME sandbox_linux)
    # set(LINUX_DESKTOP_DIR "$ENV{HOME}/bin")
    install(TARGETS sandbox DESTINATION ${LINUX_DESKTOP_DIR})
else()
    message(FATAL_ERROR "Unsupported operating system: ${CMAKE_SYSTEM_NAME}")
endif()