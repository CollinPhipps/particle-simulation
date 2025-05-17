cmake_minimum_required(VERSION 3.16)
project(ParticleSimulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/SFML)

set(SOURCES
    src/main.cpp
    src/Ball.cpp
    src/ParticleSystem.cpp
)

add_executable(ParticleSimulation ${SOURCES})

target_include_directories(ParticleSimulation
    PRIVATE
        include
        external/SFML/include
)

target_link_libraries(ParticleSimulation
    PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
)