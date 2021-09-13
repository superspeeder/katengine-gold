project(stb)

find_path(STB_INCLUDE_DIRS "stb.h")

add_library(stb src/stb_image.cpp
                src/stb_truetype.cpp
                src/stb_image_write.cpp
                src/stb_image_resize.cpp
                src/stb_rect_pack.cpp
                src/stb_ds.cpp
                src/stb_sprintf.cpp
                src/stb_easy_font.cpp
                src/stb_herringbone_wang_tile.cpp
                src/stb_c_lexer.cpp
                src/stb_divide.cpp
                src/stb_leakcheck.cpp
                src/stb_include.cpp)

target_include_directories(stb PUBLIC ${STB_INCLUDE_DIRS})
add_library(stb::stb ALIAS stb)