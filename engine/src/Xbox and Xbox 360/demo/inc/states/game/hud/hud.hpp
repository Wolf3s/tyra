/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include <tyra>
#include <memory>

using std::unique_ptr;
using Tyra::Sprite;
using Tyra::Texture;
using Tyra::TextureRepository;

namespace Demo {

class Hud {
 public:
  Hud(TextureRepository* repo);
  ~Hud();

  TextureRepository* repo;

  unique_ptr<Sprite> hpSprite;
  unique_ptr<Sprite> crosshairSprite;
  unique_ptr<Sprite> soldierSprite;

  Texture* hpTexture;
  Texture* crosshairTexture;
  Texture* soldierTexture;
};

}  // namespace Demo
