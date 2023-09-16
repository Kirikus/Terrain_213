project(tests VERSION 0.1 LANGUAGES CXX)

file(GLOB PROJECT_SOURCES "*.h" "*.cpp" "*.ui")

add_executable(tests ${PROJECT_SOURCES})

target_link_libraries(tests PRIVATE core)

