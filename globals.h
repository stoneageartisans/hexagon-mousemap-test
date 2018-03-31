#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

// Graphics Properties
#define DRIVER_TYPE         EDT_OPENGL
#define FULLSCREEN          true
#define WINDOWED            false
#define DISPLAY_TYPE        FULLSCREEN
#define BIT_DEPTH           32
#define ANTI_ALIASING       0
#define WINDOW_CAPTION      L"Hexagons"
#define USE_BACK_BUFFER     true
#define NO_BACK_BUFFER      false
#define USE_Z_BUFFER        true
#define NO_Z_BUFFER         false

// Properties
#define HEX_COLUMNS         13
#define HEX_ROWS            7

// Filenames
#define RESOURCES           "resources.pk3"
#define FONT				"font_comic_24.xml"

// String Values
#define CLEAR               ("")
#define WCLEAR              (L"")

// Colors
#define COLOR_TRANSPARENT   SColor(0, 0, 0, 0)
#define COLOR_BLACK         SColor(255, 0, 0, 0)
#define COLOR_WHITE         SColor(255, 255, 255, 255)
#define COLOR_RED           SColor(255, 255, 0, 0)
#define COLOR_ORANGE        SColor(255, 255, 127, 0)
#define COLOR_YELLOW        SColor(255, 255, 255, 0)
#define COLOR_GREEN         SColor(255, 0, 255, 0)
#define COLOR_BLUE          SColor(255, 0, 0, 255)
#define COLOR_PURPLE        SColor(255, 127, 0, 255)
#define COLOR_PINK          SColor(255, 255, 0, 255)
#define COLOR_GRAY          SColor(255, 127, 127, 127)
#define COLOR_DKGRAY        SColor(255, 63, 63, 63)
#define COLOR_BROWN         SColor(255, 127, 63, 0)

// 2d Positions/Vectors
#define ORIGIN_2D           vector2d<s32>(0, 0)

// Predefined Sizes
#define TEXT_INFO_WIDTH     font_comic->getDimension(L"HEX:  XX, XX ").Width
#define TEXT_INFO_HEIGHT    font_comic->getDimension(L"HEX:  XX, XX ").Height
#define TEXT_INFO_WIDTH2    font_comic->getDimension(L"DISTANCE:  XX ").Width
#define TEXT_INFO_HEIGHT2   font_comic->getDimension(L"DISTANCE:  XX ").Height
#define TEXT_FPS_WIDTH      font_comic->getDimension(L"FPS:  XXXX ").Width
#define TEXT_FPS_HEIGHT     font_comic->getDimension(L"FPS:  XXXX ").Height

// GUI Rectangles
#define RECT_FPS            rect<s32>( dimension_screen->Width - TEXT_FPS_WIDTH, 0, dimension_screen->Width, TEXT_INFO_HEIGHT )
#define RECT_INFO           rect<s32>( dimension_screen->Width - TEXT_INFO_WIDTH, (dimension_screen->Height / 2) - TEXT_INFO_HEIGHT, dimension_screen->Width, dimension_screen->Height / 2 )
#define RECT_INFO2          rect<s32>( dimension_screen->Width - TEXT_INFO_WIDTH2, (dimension_screen->Height / 2) + TEXT_INFO_HEIGHT2, dimension_screen->Width, (dimension_screen->Height / 2) + TEXT_INFO_HEIGHT2 + TEXT_INFO_HEIGHT2 )

#endif // GLOBALS_H_INCLUDED
