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
    # sound library set
    set(LINK_LIBRARIES winmm)
    # icon file set
    set(RESOURCE_FILES images/icons/fire.ico)  # Add this line to include the .ico file
elseif(APPLE)
    # macOS-specific configurations
    set(UNIX_DESKTOP_DIR "$ENV{HOME}/Desktop")
    collect_files_with_extensions(assets "/*.docx" "/*.pdf")
    collect_files_with_extensions(images "/*.png" "/*.jpg")
    collect_files_with_extensions(sound "/*.wav" "/*.mp3")
    list(APPEND SOURCES staging.cpp ${HEADER_FILES})
    find_library(APPLE_MEDIA_PLAYER_FRAMEWORK AppleMediaPlayers)
    # sound library set
    set(LINK_LIBRARIES ${APPLE_MEDIA_PLAYER_FRAMEWORK})
    # icon file set
    set(ICON_FILE ${CMAKE_CURRENT_SOURCE_DIR}/images/icons/fire.icns)
elseif(UNIX)
    # Linux-specific configurations
    set(UNIX_DESKTOP_DIR "$ENV{HOME}/bin")
    collect_files_with_extensions(assets "/*.docx" "/*.pdf")
    collect_files_with_extensions(images "/*.png" "/*.jpg")
    collect_files_with_extensions(sound "/*.wav" "/*.mp3")
    list(APPEND SOURCES staging.cpp ${HEADER_FILES})
    # sound library set
    set(LINK_LIBRARIES asound)
    # icon file set
    set(ICON_FILE ${CMAKE_CURRENT_SOURCE_DIR}/images/icons/fire.png)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/magian.desktop.in
        ${CMAKE_CURRENT_BINARY_DIR}/magian.desktop
        @ONLY
    )
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
elseif(APPLE)
    # macOS-specific configurations
    set_target_properties(magian PROPERTIES MACOSX_BUNDLE TRUE)
    set_target_properties(magian PROPERTIES OUTPUT_NAME "Magian")

    # Create the Info.plist file
    set(INFO_PLIST_FILE ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in
        ${INFO_PLIST_FILE}
        @ONLY
    )
    set_target_properties(magian PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${INFO_PLIST_FILE})

    # Copy the icon file to the bundle
    set(ICON_DESTINATION_DIR ${CMAKE_CURRENT_BINARY_DIR}/Magian.app/Contents/Resources)
    file(COPY ${ICON_FILE} DESTINATION ${ICON_DESTINATION_DIR})
    
    # INSTALLATION
    add_custom_command(TARGET magian POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "$<TARGET_BUNDLE_DIR:magian>" "${UNIX_DESKTOP_DIR}/Magian.app"
        COMMENT "Copying app bundle to desktop directory"
    )
else()
    # Linux-specific configurations
    set_target_properties(magian PROPERTIES OUTPUT_NAME magian)

    # Copy the icon file to the installation directory
    install(FILES ${ICON_FILE} DESTINATION ${UNIX_DESKTOP_DIR})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/magian.desktop DESTINATION ${UNIX_DESKTOP_DIR})
    
    # INSTALLATION
    add_custom_command(TARGET magian POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:magian>" "${UNIX_DESKTOP_DIR}/magian"
        COMMENT "Copying executable to desktop directory"
    )
endif()