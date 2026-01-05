cmake_minimum_required(VERSION 3.10)

project(endlessRun)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# SFML lib path
set(SFML_DIR ${CMAKE_SOURCE_DIR}/deps/SFML-3.0.0)

# exec
add_executable(${PROJECT_NAME}
    src/main.cpp
)

# Include SFML
target_include_directories(${PROJECT_NAME} PRIVATE
    ${SFML_DIR}/include
)

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
