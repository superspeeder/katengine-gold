project(editor)

file(GLOB_RECURSE EDITOR_SOURCE CONFIGURE_DEPENDS src/**.cpp)

add_executable(editor ${EDITOR_SOURCE})

target_include_directories(editor PUBLIC include/)

target_link_libraries(editor kat::engine)
