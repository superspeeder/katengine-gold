cmake_minimum_required(VERSION 3.10)

project(katengine-gold)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


find_package(glad REQUIRED)
find_package(spdlog REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(cpr REQUIRED)

add_subdirectory(stb)
add_subdirectory(editor)
add_subdirectory(engine)
add_subdirectory(sandbox)

