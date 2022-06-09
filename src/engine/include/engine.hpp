/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_ENGINE_
#define _TYRA_ENGINE_

#include <tamtypes.h>
#include <sys/stat.h>
#include <loadfile.h>
#include <cstdlib>
#include <time.h>
#include <stdio.h>
#include <kernel.h>
#include <sifrpc.h>
#include <sbv_patches.h>

#include "game.hpp"
#include "models/screen_settings.hpp"
#include "models/math/matrix.hpp"
#include "modules/renderer.hpp"
#include "modules/timer.hpp"
#include "modules/pad.hpp"
#include "modules/audio.hpp"
#include "modules/file_service.hpp"
#include "utils/debug.hpp"

extern u8 usbd_irx[];
extern int size_usbd_irx;

extern u8  usbhdfsd_irx;
extern int size_usbhdfsd_irx;

class Engine
{

public:
    Engine(const ScreenSettings &screen);
    Engine();
    ~Engine();

    void init(Game *t_game, u32 t_gifPacketSize);
    void setDefaultScreen();
    void waitUntilUsbDeviceIsReady();
    Renderer *renderer;
    // FileService fileService;
    Audio audio;
    ScreenSettings screen;
    Pad pad;
    float fps;

private:
    void firePS2();
    void delay(int count);
    u8 fpsDelayer;
    Timer timer;
    u8 isInitialized;
    void gameLoop();
    Game *game;
    s32 mainThreadId;
    static void wakeup(s32 t_alarmId, u16 t_time, void *t_common);
    void loadExternalModules();
};

#endif
