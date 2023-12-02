project(core VERSION 0.1 LANGUAGES CXX)

file(GLOB PROJECT_SOURCES "*.h" "*.cpp")

add_library(core STATIC ${PROJECT_SOURCES}
    elevation.h
    vegetation.h
    dielectric_permittivity.h)
