#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <irrlicht.h>
#include "globals.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class Application : public IEventReceiver
{
    public:
        Application();
        ~Application();
        bool OnEvent(const SEvent& EVENT);
    private:
        // Variables
        IGUIEnvironment* gui_environment;
        IrrlichtDevice* irrlicht_device;
        ISceneManager* scene_manager;
        IVideoDriver* video_driver;
        SColor* color_background;
        IImage* image_hexmap;
        dimension2d<u32>* dimension_screen;
        ITexture* texture_hexmap;
        vector2d<s32>* origin_2d;
        IGUIFont* font_comic;
        IGUIStaticText* static_text_info;
        stringw* stringw_info;
        s32 mouse_x;
        s32 mouse_y;
        IGUIStaticText* static_text_fps;
        stringw* stringw_fps;
        s32 current_fps;
        s32 last_fps;
        IGUIStaticText* static_text_info2;
        stringw* stringw_info2;
        map< SColor, vector2d<s32> > hexagons;
        SColor current_hex;
        // Methods
        void initialize();
        void exit_Application();
        void run();
        void update_info();
        void update_fps();
        s32 get_max_of_3_numbers(s32 NUM1, s32 NUM2, s32 NUM3);
        void select_hex();
        void mark_current_hex();
};

// Constructor
Application::Application()
{
    initialize();
    run();
}

// Destructor
Application::~Application()
{
    delete dimension_screen;
    delete origin_2d;
    delete stringw_info;
    delete stringw_fps;
    irrlicht_device->drop();
}

// Public Methods
bool Application::OnEvent(const SEvent& EVENT)
{
    bool return_value = false;
    switch(EVENT.EventType)
    {
		case EET_KEY_INPUT_EVENT:
            switch(EVENT.KeyInput.Key)
            {
                case KEY_ESCAPE:
                    if(!EVENT.KeyInput.PressedDown)
                    {
                        exit_Application();
                        return_value = true;
                    }
                    break;
                default:
                    break;
            }
            break;
        case EET_MOUSE_INPUT_EVENT:
            switch(EVENT.MouseInput.Event)
            {
                case EMIE_MOUSE_MOVED:
                    mouse_x = EVENT.MouseInput.X;
                    mouse_y = EVENT.MouseInput.Y;
                    return_value = true;
                    break;
                case EMIE_LMOUSE_LEFT_UP:
                    mouse_x = EVENT.MouseInput.X;
                    mouse_y = EVENT.MouseInput.Y;
                    select_hex();
                    return_value = true;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return return_value;
}

// Private Methods
void Application::exit_Application()
{
    irrlicht_device->setEventReceiver(0);
    irrlicht_device->closeDevice();
    irrlicht_device->run();
}

void Application::initialize()
{
	// Define custom parameters
	SIrrlichtCreationParameters* parameters = new SIrrlichtCreationParameters();
	parameters->DriverType = DRIVER_TYPE;
	parameters->Bits = BIT_DEPTH;
	parameters->AntiAlias = ANTI_ALIASING;
	parameters->Fullscreen = DISPLAY_TYPE;
	IrrlichtDevice* temp_device = createDevice(EDT_NULL);
    switch(DISPLAY_TYPE)
    {
        case FULLSCREEN:
            dimension_screen = new dimension2d<u32>(temp_device->getVideoModeList()->getDesktopResolution().Width,
                                                    temp_device->getVideoModeList()->getDesktopResolution().Height
                                                    );
            break;
        case WINDOWED:
            dimension_screen = new dimension2d<u32>(( (f32) temp_device->getVideoModeList()->getDesktopResolution().Width ) / 1.5,
                                                    ( (f32) temp_device->getVideoModeList()->getDesktopResolution().Height ) / 1.5
                                                    );
            break;
        default:
            dimension_screen = new dimension2d<u32>(1024, 768);
            break;
    }
    temp_device->drop();
    parameters->WindowSize = *dimension_screen;

    // Initialize irrlicht
    irrlicht_device = createDeviceEx(*parameters);
    irrlicht_device->setEventReceiver(this);
    irrlicht_device->setWindowCaption(WINDOW_CAPTION);
    video_driver = irrlicht_device->getVideoDriver();
    scene_manager = irrlicht_device->getSceneManager();
    gui_environment = irrlicht_device->getGUIEnvironment();

    // Initialize some values
    color_background = new COLOR_BLACK;
    origin_2d = new ORIGIN_2D;
    irrlicht_device->getFileSystem()->addFileArchive(RESOURCES);
    font_comic = gui_environment->getFont(FONT);
    gui_environment->getSkin()->setFont(font_comic);
    static_text_info = gui_environment->addStaticText(L"HEX:",
                                                      RECT_INFO,
                                                      false,
                                                      false
                                                      );
    static_text_info->setOverrideColor(COLOR_WHITE);
    static_text_info->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_CENTER);
    stringw_info = new stringw();
    static_text_info2 = gui_environment->addStaticText(L"DISTANCE:",
                                                       RECT_INFO2,
                                                       false,
                                                       false
                                                       );
    static_text_info2->setOverrideColor(COLOR_WHITE);
    static_text_info2->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_CENTER);
    stringw_info2 = new stringw();
    current_hex = COLOR_BLACK;

    // Create FPS display
    static_text_fps = gui_environment->addStaticText(L"",
                                                     RECT_FPS,
                                                     false,
                                                     false
                                                     );
    static_text_fps->setOverrideColor(COLOR_RED);
    static_text_fps->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_CENTER);
    current_fps = 0;
    last_fps = -1;
    stringw_fps = new stringw();

    // Create hexmap
    s32 hex_radius_vertical = dimension_screen->Height / 12;
    s32 hex_height = hex_radius_vertical * 2;
    s32 hex_radius_horizontal = hex_radius_vertical * 0.866;
    s32 hex_width = hex_radius_horizontal * 2;
    vector2d<s32> hex_start = vector2d<s32>(hex_radius_horizontal, hex_radius_vertical);
    vector2d<s32> hex_center;
    S3DVertex vertices[7];
    u32 indices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 6,
        0, 6, 1
    };
    u32 R = 0;
    u32 G = 0;
    u32 B = 0;
    texture_hexmap = video_driver->addRenderTargetTexture(*dimension_screen, "hexmap");
    video_driver->setRenderTarget(texture_hexmap);  // set drawing to the texture
    for(int y = 0; y < HEX_ROWS; y ++)
    {
        G = (y + 1) * 10;
        hex_center.Y = hex_start.Y + ( (hex_height / 4) * 3 * y );
        for(int x = 0; x < HEX_COLUMNS; x ++)
        {
            hex_center.X = hex_start.X + (hex_width * x);
            R = (x + 1) * 10;
            SColor hex_color = SColor(255, R, G, B);
            vertices[0] = S3DVertex(hex_center.X,
                                    hex_center.Y,
                                    0,
                                    1,
                                    1,
                                    1,
                                    hex_color,
                                    0,
                                    0
                                    );
            vertices[1] = S3DVertex(hex_center.X,
                                    hex_center.Y - hex_radius_vertical,
                                    0,
                                    1,
                                    1,
                                    1,
                                    hex_color,
                                    0,
                                    0
                                    );
            vertices[2] = S3DVertex(hex_center.X + hex_radius_horizontal,
                                    hex_center.Y - (hex_radius_vertical / 2),
                                    0,
                                    1,
                                    1,
                                    1,
                                    hex_color,
                                    0,
                                    0
                                    );
            vertices[3] = S3DVertex(hex_center.X + hex_radius_horizontal,
                                    hex_center.Y + (hex_radius_vertical / 2),
                                    0,
                                    1,
                                    1,
                                    1,
                                    hex_color,
                                    0,
                                    0
                                    );
            vertices[4] = S3DVertex(hex_center.X,
                                    hex_center.Y + hex_radius_vertical,
                                    0,
                                    1,
                                    1,
                                    1,
                                    hex_color,
                                    0,
                                    0
                                    );
            vertices[5] = S3DVertex(hex_center.X - hex_radius_horizontal,
                                    hex_center.Y + (hex_radius_vertical / 2),
                                    0,
                                    1,
                                    1,
                                    1,
                                    hex_color,
                                    0,
                                    0
                                    );
            vertices[6] = S3DVertex(hex_center.X - hex_radius_horizontal,
                                    hex_center.Y - (hex_radius_vertical / 2),
                                    0,
                                    1,
                                    1,
                                    1,
                                    hex_color,
                                    0,
                                    0
                                    );
            if(x == 0)
            {
                if(y == 0)
                {
                    video_driver->draw2DVertexPrimitiveList(vertices, 7, indices, 6, EVT_STANDARD, EPT_TRIANGLES, EIT_32BIT);
                    hexagons.insert(hex_color, hex_center);
                }
            }
            if(x == 1)
            {
                if(y < 3)
                {
                    video_driver->draw2DVertexPrimitiveList(vertices, 7, indices, 6, EVT_STANDARD, EPT_TRIANGLES, EIT_32BIT);
                    hexagons.insert(hex_color, hex_center);
                }
            }
            if(x == 2)
            {
                if(y < 5)
                {
                    video_driver->draw2DVertexPrimitiveList(vertices, 7, indices, 6, EVT_STANDARD, EPT_TRIANGLES, EIT_32BIT);
                    hexagons.insert(hex_color, hex_center);
                }
            }
            if( (x > 2) && (x < 10) )
            {
                video_driver->draw2DVertexPrimitiveList(vertices, 7, indices, 6, EVT_STANDARD, EPT_TRIANGLES, EIT_32BIT);
                hexagons.insert(hex_color, hex_center);
            }
            if(x == 10)
            {
                if(y > 1)
                {
                    video_driver->draw2DVertexPrimitiveList(vertices, 7, indices, 6, EVT_STANDARD, EPT_TRIANGLES, EIT_32BIT);
                    hexagons.insert(hex_color, hex_center);
                }
            }
            if(x == 11)
            {
                if(y > 3)
                {
                    video_driver->draw2DVertexPrimitiveList(vertices, 7, indices, 6, EVT_STANDARD, EPT_TRIANGLES, EIT_32BIT);
                    hexagons.insert(hex_color, hex_center);
                }
            }
            if(x == 12)
            {
                if(y == 6)
                {
                    video_driver->draw2DVertexPrimitiveList(vertices, 7, indices, 6, EVT_STANDARD, EPT_TRIANGLES, EIT_32BIT);
                    hexagons.insert(hex_color, hex_center);
                }
            }
        }
        hex_start.X = hex_start.X - hex_radius_horizontal;
    }
    video_driver->setRenderTarget(ERT_FRAME_BUFFER);  // return drawing to the screen
    image_hexmap = video_driver->createImage(texture_hexmap, *origin_2d, *dimension_screen);  // create the hexmap in memory for checking pixels
}

void Application::run()
{
    while( irrlicht_device->run() )
    {
        video_driver->beginScene(USE_BACK_BUFFER, USE_Z_BUFFER, *color_background);
        video_driver->draw2DImage(texture_hexmap, *origin_2d);
        mark_current_hex();
        scene_manager->drawAll();
        gui_environment->drawAll();
        video_driver->endScene();
        irrlicht_device->yield();
        update_info();
        update_fps();
    }
}

void Application::update_info()
{
    u32 x = ( image_hexmap->getPixel( (u32) mouse_x, (u32) mouse_y ).getRed() ) / 10;
    u32 y = ( image_hexmap->getPixel( (u32) mouse_x, (u32) mouse_y ).getGreen() ) / 10;
    *stringw_info = L"HEX:  ";
    *stringw_info += x;
    stringw_info->append(L", ");
    *stringw_info += y;
    static_text_info->setText( stringw_info->c_str() );

    u32 diff_x = x - (current_hex.getRed() / 10);
    u32 diff_y = y - (current_hex.getGreen() / 10);
    *stringw_info2 = L"DISTANCE:  ";
    *stringw_info2 += get_max_of_3_numbers(abs(diff_x),
                                           abs(diff_y),
                                           abs(diff_y - diff_x)
                                           );
    /*if(diff_y > 1)
    {
        *stringw_info2 += get_max_of_3_numbers(diff_x,
                                               diff_y,
                                               (diff_x + diff_y) - (diff_y / 2)
                                               );
    }
    else
    {
        *stringw_info2 += get_max_of_3_numbers(diff_x,
                                               diff_y,
                                               diff_x + diff_y
                                               );
    }*/
    static_text_info2->setText( stringw_info2->c_str() );
}

s32 Application::get_max_of_3_numbers(s32 NUM1, s32 NUM2, s32 NUM3)
{
    if(NUM1 > NUM2)
    {
        if(NUM1 > NUM3)
        {
            return NUM1;
        }
        else
        {
            return NUM3;
        }
    }
    else
    {
        if(NUM2 > NUM3)
        {
            return NUM2;
        }
        else
        {
            return NUM3;
        }
    }
}

void Application::update_fps()
{
    current_fps = video_driver->getFPS();
    if(last_fps != current_fps)
    {
        *stringw_fps =  L"FPS:  ";
        *stringw_fps += current_fps;
        static_text_fps->setText( stringw_fps->c_str() );
        last_fps = current_fps;
    }
}

void Application::select_hex()
{
    SColor temp_color = image_hexmap->getPixel( (u32) mouse_x, (u32) mouse_y );
    if( hexagons.find(temp_color) != 0 )
    {
        current_hex = temp_color;
    }
}

void Application::mark_current_hex()
{
    if( hexagons.find(current_hex) != 0 )
    {
        video_driver->draw2DPolygon(hexagons[current_hex],
                                    32,
                                    COLOR_WHITE,
                                    3
                                    );
    }
}

#endif // APPLICATION_H_INCLUDED
