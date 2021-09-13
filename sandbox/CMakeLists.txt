project(sandbox)

file(GLOB_RECURSE SANDBOX_SOURCE CONFIGURE_DEPENDS src/**.cpp)

add_executable(sandbox ${SANDBOX_SOURCE})

target_include_directories(sandbox PUBLIC include/)

target_link_libraries(sandbox kat::engine)
