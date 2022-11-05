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
#include <vector>
#include "states/game/terrain/heightmap.hpp"
#include "states/game/player/player_shoot_action.hpp"
#include "./enemy.hpp"

using Tyra::DynamicMesh;
using Tyra::Engine;
using Tyra::Texture;
using Tyra::TextureRepository;
using Tyra::Vec4;

namespace Demo {

class EnemyManager {
 public:
  EnemyManager(Engine* engine, const Heightmap& heightmap);
  ~EnemyManager();

  void update(const Heightmap& heightmap, const Vec4& playerPosition,
              const PlayerShootAction& shootAction);

  std::vector<RendererDynamicPair*> getPairs() const;

 private:
  TextureRepository* textureRepo;
  std::vector<Enemy*> enemies;
  DynamicMesh* motherMesh;
  Texture* bodyTexture;
  Texture* clothTexture;
};

}  // namespace Demo
