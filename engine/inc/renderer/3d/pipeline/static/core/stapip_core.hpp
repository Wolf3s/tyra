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

#include <tamtypes.h>
#include "./bag/stapip_bag.hpp"
#include "./bag/packaging/stapip_bag_packages_bbox.hpp"
#include "./bag/packaging/stapip_bag_package.hpp"
#include "./bag/packaging/stapip_bag_packager.hpp"
#include "./stapip_qbuffer_renderer.hpp"
#include "./stapip_bag_bboxes_cacher.hpp"
#include "renderer/3d/pipeline/shared/pipeline_frustum_culling.hpp"

namespace Tyra {

class StaPipCore {
 public:
  StaPipCore();
  ~StaPipCore();

  void init(RendererCore* t_core);

  void onFrameEnd();

  /** Render 3D via "bags" */
  void render(StaPipBag* bag);

  /** Get max vert count of VU1 qbuffer (for optimizations) */
  u32 getMaxVertCountByParams(const bool& isSingleColor,
                              const bool& isLightingEnabled,
                              const bool& isTextureEnabled);

  /** Get max vert count of VU1 qbuffer (for optimizations) */
  u32 getMaxVertCountByBag(const StaPipBag* bag);

  /**
   * - Uploads standard VU1 programs.
   * - Sends static "Tyra Renderer3D" VU1 data.
   * - Sets double buffers exactly for "Tyra Renderer3D"
   * Should be called if VU1 was used by your non standard programs.
   */
  void reinitVU1Programs();

  void allocateOnUse() { qbufferRenderer.allocateOnUse(); }
  void deallocateOnUse() { qbufferRenderer.deallocateOnUse(); }

 private:
  void setPrim();
  void setLod();

  prim_t prim;
  lod_t lod;

  u32 maxVertCount;
  RendererCore* rendererCore;
  StapipBagBBoxesCacher cacher;

  void setMaxVertCount(const u32& count);
  StaPipBagPackager packager;
  StaPipQBufferRenderer qbufferRenderer;
  void renderPkgs(StaPipBagPackage* packages, const bool& doClip, u16 count);
  void renderSubpkgs(StaPipBagPackage* packages, u16 count);
};

}  // namespace Tyra
