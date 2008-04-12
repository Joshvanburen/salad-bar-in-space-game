#include "core.h"

using namespace CEGUI;
using namespace irr;
using namespace video;
using namespace scene;

Core::Core () {
        dev = createDevice (EDT_OPENGL, core::dimension2d<s32> (640, 480), 32, false,
                        true, false, this);
        drv = dev->getVideoDriver ();
        smgr = dev->getSceneManager ();
        dev->getCursorControl ()->setVisible (false);
        // Without setting the following flags all works good as well
        // It's just to be 100% sure
        drv->setTextureCreationFlag (ETCF_ALWAYS_32_BIT, true);
        drv->setTextureCreationFlag (ETCF_ALWAYS_16_BIT, false);
        drv->setTextureCreationFlag (ETCF_CREATE_MIP_MAPS, false);
        drv->setTextureCreationFlag (ETCF_OPTIMIZED_FOR_QUALITY, true);
        // Create Irrlicht renderer for CEGUI
        // Bool tells whether to use irrlicht resource provider or default one
        // We use default one, I couldn't see any difference using both of them
        rend = new IrrlichtRenderer (dev, false);
        // Create CEGUI, we specify only renderer, we take all the rest parameters
        // by default (you can see them all in CEGUI API)
        new     System (rend);
        // Get default resource provider
        DefaultResourceProvider* rp = static_cast<DefaultResourceProvider*> (
                        System::getSingleton ().getResourceProvider ());
        // Set defaults for resource groups
        // This will allow us to use resource groups instead of writing full path
        // to resources every time we need it
        rp->setResourceGroupDirectory ("schemes", "../datafiles/schemes/");
        rp->setResourceGroupDirectory ("imagesets", "../datafiles/imagesets/");
        rp->setResourceGroupDirectory ("fonts", "../datafiles/fonts/");
        rp->setResourceGroupDirectory ("layouts", "../datafiles/layouts/");
        rp->setResourceGroupDirectory ("looknfeel", "../datafiles/looknfeel/");
        //rp->setResourceGroupDirectory ("lua_scripts", "../datafiles/lua_scripts/");

    // CEGUI relies on various systems being set up, so this is what we do
    // here first.
    //
    // Note that it is possible, and even usual, for most of these steps to
    // be done automatically via a "scheme" definition, or even from the
    // cegui.conf configuration file, however for completeness, and as an
    // example, virtually everything is being done manually in this example
    // code.

    // Imagesets are a collection of named areas within a texture or image
    // file.  Each area becomes an Image, and has a unique name by which it
    // can be referenced.  This sample is using the TaharezLook widgets, and
    // these rely upon the TaharezLook Imageset; so we must load this in
    // before doing anything else.  Note that the Imageset would normally be
    // specified as part of a scheme file, although as this example is
    // demonstrating, it is not a requirement.
    //
    // Load TaharezLook imageset by making use of the ImagesetManager singleton.
        Imageset *imgs = ImagesetManager::getSingleton ().createImageset (
                        "TaharezLook.imageset", "imagesets");
        // The next thing we do is set a default mouse cursor image.  This is
    // not strictly essential, although it is nice to always have a visible
    // cursor if a window or widget does not explicitly set one of its own.
    //
    // The TaharezLook Imageset contains an Image named "MouseArrow" which is
    // the ideal thing to have as a default mouse cursor image.
        System::getSingleton ().setDefaultMouseCursor (&imgs->getImage ("MouseArrow"));
        // Now we have the gui imagery side of things set up, we should load in a font.
    // You should always load in at least one font, this is to ensure that there
    // is a default available for any gui element which needs to draw text.
    // The first font you load is automatically set as the initial default font,
    // although you can change the default later on if so desired.  Again, it is
    // possible to list fonts to be automatically loaded as part of a scheme, so
    // this step may not usually be performed explicitly.
    //
    // Fonts are loaded via the FontManager singleton.
        FontManager::getSingleton ().createFont ("DejaVuSans-10.font", "fonts");
    // The widgets that we will be using for this sample are the TaharezLook widgets,
    // and to enable us to use this 'skin' we must load the xml specification - which
    // within cegui is known as a "looknfeel" file.
    //
    // We load the looknfeel via the WidgetLookManager singleton.
        WidgetLookManager::getSingleton ().parseLookNFeelSpecification (
                        "TaharezLook.looknfeel", "looknfeel");
    // The final step of basic initialisation that is usually peformed is
    // registering some widgets with the system via a scheme file.  The scheme
    // basically states the name of a dynamically loaded module that contains the
    // widget classes that we wish to use.  As stated previously, a scheme can actually
    // conatin much more information, though for the sake of this first example, we
    // load a scheme which only contains what is required to register some widgets.
    //
    // Use the SchemeManager singleton to load in a scheme that registers widgets
    // for TaharezLook.
        SchemeManager::getSingleton ().loadScheme ("TaharezLookWidgets.scheme", "schemes");
    // Now the system is initialised, we can actually create some UI elements, for
    // this first example, a full-screen 'root' window is set as the active GUI
    // sheet, and then a simple frame window will be created and attached to it.

    // All windows and widgets are created via the WindowManager singleton.
        WindowManager &wmgr = WindowManager::getSingleton ();
    // Here we create a "DeafultWindow".  This is a native type, that is, it does
    // not have to be loaded via a scheme, it is always available.  One common use
    // for the DefaultWindow is as a generic container for other windows.  Its
    // size defaults to 1.0f x 1.0f using the Relative metrics mode, which means
    // when it is set as the root GUI sheet window, it will cover the entire display.
    // The DefaultWindow does not perform any rendering of its own, so it's invisible.
    //
    // Create a DefaultWindow called 'Root'.
        DefaultWindow *root = (DefaultWindow*)wmgr.createWindow ("DefaultWindow", "Root");
    // Set the GUI root window (also known as the GUI "sheet"), so the gui we set up
    // will be visible.
        System::getSingleton ().setGUISheet (root);
    // A FrameWindow is a window with a frame and a titlebar which may be moved around
    // and resized.
    //
    // Create a FrameWindow in the TaharezLook style, and name it 'Demo Window'
        FrameWindow *wnd = (FrameWindow*)wmgr.createWindow ("TaharezLook/FrameWindow",
                        "Demo Window");
    // Here we attach the newly created FrameWindow to the previously created
    // DefaultWindow which we will be using as the root of the displayed gui.
        root->addChildWindow (wnd);
    // Windows are in Relative metrics mode by default.  This means that we can
    // specify sizes and positions without having to know the exact pixel size
    // of the elements in advance.  The relative metrics mode co-ordinates are
    // relative to the parent of the window where the co-ordinates are being set.
    // This means that if 0.5f is specified as the width for a window, that window
    // will be half as its parent window.
    //
    // Here we set the FrameWindow so that it is half the size of the display,
    // and centered within the display.
        wnd->setPosition (UVector2 (cegui_reldim (0.25f), cegui_reldim (0.25f)));
        wnd->setSize (UVector2 (cegui_reldim (0.5f), cegui_reldim (0.5f)));
    // Now we set the maximum and minum sizes for the new window.  These are
    // specified using relative co-ordinates, but the important thing to note
    // is that these settings are always relative to the display rather than the
    // parent window.
    //
    // Here we set a maximum size for the FrameWindow which is equal to the size
    // of the display, and a minimum size of one tenth of the display.
        wnd->setMaxSize (UVector2 (cegui_reldim (1.0f), cegui_reldim (1.0f)));
        wnd->setMinSize (UVector2 (cegui_reldim (0.1f), cegui_reldim (0.1f)));
    // As a final step in the initialisation of our sample window, we set the window's
    // text to "Hello, world!", so that this text will appear as the caption in the
    // FrameWindow's titlebar.
        wnd->setText ("Hello, world!");

        smgr->addCameraSceneNode (0);
        //drv->setAmbientLight (SColor (255, 255, 255, 255));
        last_time = dev->getTimer ()->getRealTime ();
        quit = false;
}

Core::~Core () {
        // Remove CEGUI
        delete System::getSingletonPtr ();
        // Remove CEGUI Irrlicht renderer
        delete rend;
        dev->drop ();
}

bool Core::OnEvent (SEvent e) {
        // Quit on escape
        if (e.EventType == EET_KEY_INPUT_EVENT)
                if (e.KeyInput.Key == KEY_ESCAPE) {
                        quit = true;
                }
        // Otherwise send events to CEGUI
        if (rend) rend->OnEvent (e);
        return false;
}

void Core::Run () {
        int last_fps = -1;
        while (dev->run () && !quit) {
                if (dev->isWindowActive ()) {
                        u32 curr_time = dev->getTimer ()->getRealTime ();
                        System::getSingleton ().injectTimePulse (
                                        static_cast<float> (curr_time - last_time) / 1000.0f);
                        last_time = curr_time;
                        drv->beginScene (true, true, SColor (150, 50, 50, 50));
                        smgr->drawAll ();
                        System::getSingleton ().renderGUI ();
                        drv->endScene ();

                        int fps = drv->getFPS ();
                        if (last_fps != fps) {
                                core::stringw str = L"Irrlicht 1.2 & CEGUI 0.6   FPS: ";
                                str += fps;
                                dev->setWindowCaption (str.c_str ());
                                last_fps = fps;
                        }
                }
        }
}
