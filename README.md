# haxima2

Work-in-progress CRPG.

# Requirements

* SDL2 [https://www.libsdl.org/download-2.0.php]
* freetype2 [https://www.freetype.org/download.html]
* SDL_ttf 2.0 [https://www.libsdl.org/projects/SDL_ttf/]

To install requirements on Ubuntu:

    sudo apt install libsdl2-dev libsdl2-2.0-0 -y;
    sudo apt install libfreetype6-dev libsdl2-ttf-dev libsdl2-ttf-2.0-0 -y;


# Build

    ./autogen.sh
    make
    make install


# Run

    haxima2


# Dev

There is an .indent.pro file included will enforce the project's coding style:

    indent src/*.[ch]
