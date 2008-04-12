#ifndef CORE_H
#define CORE_H

#include <irrlicht.h>
#include <CEGUI.h>
#include <irrlichtrenderer.h>

class Core: public irr::IEventReceiver {
        public:
                Core ();
                ~Core ();
                bool OnEvent (irr::SEvent e);
                void Run ();

        private:
                irr::IrrlichtDevice *dev;
                irr::video::IVideoDriver *drv;
                irr::scene::ISceneManager *smgr;
                CEGUI::IrrlichtRenderer *rend;
                irr::u32 last_time;
                bool quit;
};

#endif // CORE_H