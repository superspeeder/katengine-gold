project(engine)

file(GLOB_RECURSE ENGINE_SOURCE CONFIGURE_DEPENDS src/**.cpp)

add_library(engine ${ENGINE_SOURCE})
add_library(kat::engine ALIAS engine)

target_include_directories(engine PUBLIC include/)
target_link_libraries(engine PUBLIC glm glad::glad glfw imgui::imgui Vulkan::Vulkan stb::stb spdlog::spdlog rapidjson)