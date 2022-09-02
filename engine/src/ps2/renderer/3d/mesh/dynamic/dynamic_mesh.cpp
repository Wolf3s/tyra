
/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "math/m4x4.hpp"
#include "renderer/3d/mesh/dynamic/dynamic_mesh.hpp"

namespace Tyra {

DynamicMesh::DynamicMesh(const MeshBuilderData* data) : Mesh(data) {
  if (data->materials[0]->frames.size() == 1) {
    TYRA_WARN(
        "Frames count should be greater than 1 for DynamicMesh! Maybe you "
        "should use StaticMesh?");
  }

  for (unsigned int i = 0; i < data->materials[0]->frames.size(); i++) {
    frames.push_back(new MeshFrame(*data, i));
  }

  animation.resetAll(frames);
}

DynamicMesh::DynamicMesh(const DynamicMesh& mesh) : Mesh(mesh) {
  for (unsigned int i = 0; i < mesh.frames.size(); i++) {
    frames.push_back(new MeshFrame(*mesh.frames[i]));
  }

  animation.resetAll(frames);
}

DynamicMesh::~DynamicMesh() {
  for (unsigned int i = 0; i < frames.size(); i++) {
    delete frames[i];
  }
}

}  // namespace Tyra
