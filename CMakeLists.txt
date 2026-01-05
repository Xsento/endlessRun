cmake_minimum_required(VERSION 3.10)

project(endlessRun)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ścieżka do SFML
set(SFML_DIR ${CMAKE_SOURCE_DIR}/deps/SFML-3.0.0)

# Twój program
add_executable(${PROJECT_NAME}
    src/main.cpp
)

# Include SFML
target_include_directories(${PROJECT_NAME} PRIVATE
    ${SFML_DIR}/include
)

# Linkowanie bibliotek
if (WIN32)
    target_link_directories(${PROJECT_NAME} PRIVATE
        ${SFML_DIR}/lib
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
    )
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
