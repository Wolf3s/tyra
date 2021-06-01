/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "rocket_league.hpp"

// ----
// Constructors/Destructors
// ----

RocketLeague::RocketLeague(Engine *t_engine)
    : engine(t_engine), camera(&t_engine->screen)
{
}

RocketLeague::~RocketLeague()
{
    delete player;
    delete raceTrack;
}

// ----
// Methods
// ----

void RocketLeague::onInit()
{
    texRepo = engine->renderer->getTextureRepository();
    setBgColorAndAmbientColor();
    engine->renderer->setCameraDefinitions(&camera.view, &camera.unitCirclePosition, camera.planes);
    engine->audio.loadSong("sounds/long-night-1a.wav");
    player = new Player(texRepo);
    raceTrack = new RaceTrack(texRepo);
    beachBall = new BeachBall(texRepo);
    ammoBox = new AmmoBox(texRepo);
    policeCar = new PoliceCar(texRepo);
    skybox = new Skybox(texRepo);
    engine->audio.playSong();
    engine->audio.setSongVolume(50);
}

void RocketLeague::onUpdate()
{
    player->update(engine->pad, camera);
    camera.update(engine->pad, player->mesh);

    if (engine->pad.isCrossClicked)
        beachBall->push(player->mesh.position);

    beachBall->update();
    skybox->update(player->mesh.position);

    engine->renderer->draw(skybox->mesh);
    engine->renderer->draw(raceTrack->mesh);
    engine->renderer->draw(player->mesh);
    engine->renderer->draw(policeCar->mesh);
    engine->renderer->draw(ammoBox->mesh);
    engine->renderer->draw(beachBall->mesh);
}

void RocketLeague::setBgColorAndAmbientColor()
{
    color_t bgColor;
    bgColor.r = 0x20;
    bgColor.g = 0x20;
    bgColor.b = 0x20;
    engine->renderer->setWorldColor(bgColor);
    Vector3 ambient = Vector3(0.003F, 0.003F, 0.003F);
    engine->renderer->setAmbientLight(ambient);
}