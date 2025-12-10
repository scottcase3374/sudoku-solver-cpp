cmake_minimum_required(VERSION 3.28)
project(sudokusolver)

set(CMAKE_CXX_STANDARD 23)

find_package(Boost REQUIRED COMPONENTS filesystem timer)
add_executable(sudokusolver main.cpp
        view.hpp
        cell.hpp
        cell.cpp
        coordinator.hpp
        game.hpp
        loader.hpp
        loader.cpp
        coordinator.cpp
        view.cpp
        status.hpp
        strategy.hpp
        strategy.cpp
        game.cpp
)

target_link_libraries(sudokusolver
        ${CMAKE_CXX_STANDARD_LIBRARIES}
        Boost::filesystem
        Boost::timer)