cmake_minimum_required(VERSION 3.10)

project(endlessRun)

# cpp standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# binary directort
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ===============
# Project modules
# ===============
set(MENU
    src/menu/menu.cpp
    src/menu/button.cpp
)

set(SETTINGS
    src/settings/settings.cpp
)

# main exec
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/game.cpp
    ${MENU}
    ${SETTINGS}
)

# seting source dir
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# ====
# libs
# ====

set(JSON ${CMAKE_SOURCE_DIR}/deps/nlohmann)

# SFML lib path
if (WIN32)
    set(SFML_DIR ${CMAKE_SOURCE_DIR}/deps/SFML-3.0.0-win)

else()
    set(SFML_DIR ${CMAKE_SOURCE_DIR}/deps/SFML-3.0.0-linux)

endif()

# setting include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${SFML_DIR}/include # SFML
    ${SRC_DIR} # source dir
    ${JSON} # JSON parser
)

if (WIN32)
    target_link_directories(${PROJECT_NAME} PRIVATE
        ${SFML_DIR}/lib
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        # Debug
        $<$<CONFIG:Debug>:sfml-graphics-d>
        $<$<CONFIG:Debug>:sfml-window-d>
        $<$<CONFIG:Debug>:sfml-system-d>

        # Release
        $<$<CONFIG:Release>:sfml-graphics>
        $<$<CONFIG:Release>:sfml-window>
        $<$<CONFIG:Release>:sfml-system>)
else() # Linux
    target_link_directories(${PROJECT_NAME} PRIVATE
        ${SFML_DIR}/lib
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        pthread
    )
endif()

# copy assets after build to binary folder
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
