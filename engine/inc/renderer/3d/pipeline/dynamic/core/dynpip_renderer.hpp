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
#include "renderer/core/renderer_core.hpp"
#include "./dynpip_programs_repository.hpp"

namespace Tyra {

class DynPipRenderer {
 public:
  DynPipRenderer();
  ~DynPipRenderer();

  void init(RendererCore* t_core, DynPipProgramsRepository* t_programRepo,
            prim_t* prim, lod_t* lod);

  void reinitVU1();

  void sendObjectData(DynPipBag* bag, M4x4* mvp,
                      RendererCoreTextureBuffers* texBuffers) const;

  void render(DynPipBag** bags, const u32& count);

  void clearLastProgramName();

  const u16& getBufferSize() { return bufferSize; }

  void allocateOnUse(const u32& t_packetSize);
  void deallocateOnUse();

 private:
  void sendStaticData() const;
  void setProgramsCache();
  void uploadPrograms();
  void setDoubleBuffer();

  void addBufferDataToPacket(DynPipBag** bags, const u32& count);
  void sendPacket();

  prim_t* prim;
  lod_t* lod;
  u16 packetSize;

  packet2_t* packets[2];
  packet2_t* programsPacket;
  packet2_t* currentPacket;
  packet2_t* staticDataPacket;
  packet2_t* objectDataPacket;

  DynPipProgramsRepository* programsRepo;
  RendererCore* rendererCore;
  Path1* path1;

  DynPipProgramName lastProgramName;

  u16 bufferSize;
  u8 context;
};

}  // namespace Tyra
