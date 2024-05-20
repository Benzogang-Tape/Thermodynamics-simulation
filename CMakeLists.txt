cmake_minimum_required(VERSION 3.26)

set(PROJECT_NAME Thermodynamics_simulation)
project(${PROJECT_NAME})

include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
FETCHCONTENT_DECLARE(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.5.1)
FETCHCONTENT_MAKEAVAILABLE(SFML)

set(CMAKE_CXX_STANDARD 20)

set(${PROJECT_NAME}_HEADERS
    Reactor.hpp
    Molecules.hpp
    Vec.hpp
    Buttons.hpp
    ReactorBtns.hpp
    Plot.hpp
)

set(${PROJECT_NAME}_SOURCES
    Reactor.cpp
    Molecules.cpp
    Vec.cpp
    Buttons.cpp
    ReactorBtns.cpp
    Plot.cpp
    main.cpp
)

set(${PROJECT_NAME}_SOURCE_LIST
    ${${PROJECT_NAME}_SOURCES}
    ${${PROJECT_NAME}_HEADERS}
)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCE_LIST})

#set(SFML_DIR "D:\\sdk\\SFML-2.6.1\\lib\\cmake\\SFML")

target_link_libraries(${PROJECT_NAME} sfml-graphics)

#set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

#find_package(SFML 2.6 REQUIRED graphics network audio window system)
