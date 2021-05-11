cmake_minimum_required(VERSION 3.17)
project(video_player)

set(WITH_OPENCV ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_PREFIX_PATH "C://Qt//5.15.2//mingw81_64//")
set(OpenCV_DIR "C:\\opencv\\install")
set(Lua_DIR lib/lua)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets Qt5::OpenGL)
set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets Opengl)

add_executable(${PROJECT_NAME}
        main.cpp
        VideoPlayer.cpp VideoPlayer.h
        glwidget.h glwidget.cpp
        SettingsWindow.cc SettingsWindow.h
        Switch.h Switch.cpp
        Style.h
        Effect.h
        EffectsImplementations.h
        VideoPlayback.cpp VideoPlayback.h ScriptHandler.cpp ScriptHandler.h)


if (NOT CMAKE_PREFIX_PATH)
    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
endif ()

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})

if (${WITH_OPENCV})
    find_package(OpenCV REQUIRED)
    include_directories(${OpenCV_INCLUDE_DIRS})
    message(STATUS ${OpenCV_LIBS})
    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

    foreach (OPENCV_LIB ${OpenCV_LIBS})
        add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD                     # Adds a post-build event to equalizer
                COMMAND ${CMAKE_COMMAND} -E copy_if_different   # which executes "cmake - E copy_if_different..."
                "${OpenCV_DIR}/x64/mingw/bin/lib${OPENCV_LIB}${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH}.dll"    # <--this is in-file
                $<TARGET_FILE_DIR:${PROJECT_NAME}>                   # <--this is out-file path
        )
    endforeach ()
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${OpenCV_DIR}/x64/mingw/bin/opencv_videoio_ffmpeg452_64.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )

endif ()

if (WIN32)
    set(DEBUG_SUFFIX)
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        # set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB ${REQUIRED_LIBS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QT_INSTALL_PATH}/bin/Qt${QT_VERSION}${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()

add_subdirectory(${Lua_DIR})
message(${Lua_DIR}/src/lua.hpp)
include_directories(${Lua_DIR}/src/)
target_link_libraries(${PROJECT_NAME} liblua)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/files/empty.png"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>")