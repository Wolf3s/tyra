/*
# ______       ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "include/engine.hpp"

// ----
// Constructors/Destructors
// ----

Engine::Engine()
{
    loadExternalModules();
    setDefaultScreen();
    firePS2();
}

Engine::Engine(const ScreenSettings &t_screen)
{
    screen = t_screen;
    firePS2();
}

Engine::~Engine() {}

// ----
// Methods
// ----

void Engine::setDefaultScreen()
{
    screen.nearPlaneDist = 2.0F;
    screen.farPlaneDist = 2000.0F;
    screen.fov = 60.0F;
    screen.width = 640.0F;
    screen.height = 480.0F;
    screen.aspectRatio = screen.width / screen.height;
    screen.projectionScale = 4096.0F; // PS2 have 4k drawing area
}

void Engine::init(Game *t_game, u32 t_gifPacketSize)
{
    assertMsg(!isInitialized, "Engine was already initialized!");
    game = t_game;
    renderer = new Renderer(t_gifPacketSize, &screen);
    isInitialized = true;
    game->onInit();
    gameLoop();
}

/** Do not call this method. This is used in gameLoop() to maintain multithreading */
void Engine::wakeup(s32 t_alarmId, u16 t_time, void *t_common)
{
    (void)t_alarmId;
    (void)t_time;
    iWakeupThread(*(int *)t_common);
    ExitHandler();
}

void Engine::firePS2()
{
    srand(time(NULL));
    // fileService.startThread();
    // audio.startThread(&fileService);
    audio.init();
    audio.startThread(NULL);
    isInitialized = 0;
    mainThreadId = GetThreadId();
}

void Engine::gameLoop()
{
    for (;;)
    {
        pad.update();
        game->onUpdate();
        if (fpsDelayer++ >= 4)
        {
            fps = timer.getFPS();
            fpsDelayer = 0;
        }
        timer.prime();
        renderer->endFrame(fps);
        /** -6~ FPS */
        SetAlarm(150, &Engine::wakeup, &mainThreadId);
        SleepThread();
    }
}

void Engine::waitUntilUsbDeviceIsReady()
{
    struct stat buffer;
    int ret = -1;
    int retries = 50;

    delay(5); // some delay is required by usb mass storage driver

    while (ret != 0 && retries > 0)
    {
        ret = stat("mass:/", &buffer);
        /* Wait until the device is ready */
        nopdelay();

        retries--;
    }
}

void Engine::loadExternalModules()
{

    consoleLog("Loading external modules...\n");
    int i, ret, sometime;

    SifInitRpc(0);

    // Apply the SBV LMB patch to allow modules to be loaded from a buffer in EE RAM.
    consoleLog("Applying SBV Patches...\n");
    ret = sbv_patch_enable_lmb();
    if (ret < 0)
    {
        consoleLog("Failed to load Applying SBV Patches sbv_patch_enable_lmb");
    }
    ret = sbv_patch_disable_prefix_check();
    if (ret < 0)
    {
        consoleLog("Failed to load Applying SBV Patches sbv_patch_disable_prefix_check");
    }
    ret = sbv_patch_fileio();
    if (ret < 0)
    {
        consoleLog("Failed to load Applying SBV Patches sbv_patch_fileio");
    }

    // Load audio modules
    audio.loadModules();

    // Load USB mass modules
    consoleLog("Loading USB modules\n");

    SifExecModuleBuffer(&usbd_irx, size_usbd_irx, 0, NULL, &ret);
    if (ret < 0)
    {
        consoleLog("Failed to load module: usbd");
    }

    SifExecModuleBuffer(&usbhdfsd_irx, size_usbhdfsd_irx, 0, NULL, &ret);
    if (ret < 0)
    {
        consoleLog("Failed to load module: usbhdfsd");
    }

    waitUntilUsbDeviceIsReady();
    
    printf("modules load OK\n");
}

void Engine::delay(int count)
{
    int i;
    int ret;
    for (i = 0; i < count; i++)
    {
        ret = 0x01000000;
        while (ret--)
            asm("nop\nnop\nnop\nnop");
    }
}