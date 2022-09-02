/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/dynamic/core/bag/dynpip_lighting_bag.hpp"

namespace Tyra {

DynPipLightingBag::DynPipLightingBag() {
  lightMatrix = nullptr;
  normalsFrom = nullptr;
  normalsTo = nullptr;
  dirLights = nullptr;
}

DynPipLightingBag::~DynPipLightingBag() {}

void DynPipLightingBag::freeNormals() {
  if (normalsFrom) delete[] normalsFrom;
  if (normalsTo) delete[] normalsTo;
}

}  // namespace Tyra
