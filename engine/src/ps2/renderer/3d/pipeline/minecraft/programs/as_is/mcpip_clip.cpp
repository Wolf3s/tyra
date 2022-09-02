/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/3d/pipeline/minecraft/programs/as_is/mcpip_clip.hpp"

namespace Tyra {

McpipClip::McpipClip() {
  staticPacket = packet2_create(8, P2_TYPE_NORMAL, P2_MODE_CHAIN, true);
  algoSettings.lerpColors = false;
  algoSettings.lerpTexCoords = true;
  algoSettings.lerpNormals = false;
}

McpipClip::~McpipClip() { packet2_free(staticPacket); }

void McpipClip::init(RendererCore* core, McpipBlockData* t_singleBlockData,
                     McpipBlockData* t_multiBlockData, prim_t* t_prim,
                     lod_t* t_lod) {
  prim = t_prim;
  lod = t_lod;
  singleBlockData = t_singleBlockData;
  multiBlockData = t_multiBlockData;
  rendererCore = core;
  algorithm.init(rendererCore->getSettings());
  setDBufferSize();
  initStaticPacket();
}

unsigned int McpipClip::uploadVU1Program(McpipProgramsRepository* repo,
                                         const unsigned int& addr) {
  auto* program = repo->getProgram(McpipProgramName::McPipAsIs);
  return rendererCore->renderer3D.uploadVU1Program(program, addr);
}

void McpipClip::configureVU1AndSendStaticData() {
  unsigned short start = VU1_MCPIP_AS_IS_STATIC_LAST_DATA_ADDR + 1;
  rendererCore->renderer3D.setVU1DoubleBuffers(start, vu1DBufferSize);
  sendVU1StaticData();
}

void McpipClip::initStaticPacket() {
  packet2_utils_vu_open_unpack(staticPacket, VU1_MCPIP_AS_IS_STATIC_LOD, false);
  {
    packet2_utils_gs_add_lod(staticPacket, lod);
    packet2_utils_gif_add_set(staticPacket, 1);
  }
  packet2_utils_vu_close_unpack(staticPacket);

  packet2_utils_vu_add_end_tag(staticPacket);
}

void McpipClip::addData(McpipBlock* block, const bool& isMulti,
                        RendererCoreTextureBuffers* texBuffers,
                        packet2_t* packet, const unsigned char& context) {
  std::vector<PlanesClipVertex> clippedVertices;
  auto mvp = rendererCore->renderer3D.getViewProj() * (*block->model);

  const auto* blockData = isMulti ? multiBlockData : singleBlockData;

  for (unsigned int i = 0; i < blockData->count / 3; i++) {
    for (unsigned char j = 0; j < 3; j++) {
      inputVerts[j] = mvp * blockData->vertices[i * 3 + j];
      inputTriangle[j] = {&inputVerts[j], nullptr,
                          &blockData->textureCoords[i * 3 + j], nullptr};
    }

    unsigned char clippedSize =
        algorithm.clip(clippedTriangle, inputTriangle, algoSettings);

    if (clippedSize == 0) continue;

    auto va = clippedTriangle[0];
    for (unsigned char j = 1; j <= clippedSize - 2; j++) {
      auto vb = clippedTriangle[j];
      auto vc = clippedTriangle[(j + 1) % clippedSize];
      clippedVertices.push_back(va);
      clippedVertices.push_back(vb);
      clippedVertices.push_back(vc);
    }
  }

  addCorrections(&clippedVertices, block);
  moveDataToBuffer(&clippedVertices, context);
  addDataToPacket(packet, context, block, clippedVertices.size(), texBuffers);
}

void McpipClip::addCorrections(std::vector<PlanesClipVertex>* vertices,
                               McpipBlock* block) {
  for (unsigned int i = 0; i < vertices->size(); i++) {
    (*vertices)[i].position /= (*vertices)[i].position.w;  // Perspective divide
    (*vertices)[i].st += *block->textureOffset;            // Texture offset
  }
}

void McpipClip::moveDataToBuffer(std::vector<PlanesClipVertex>* vertices,
                                 const unsigned char& context) {
  for (unsigned int i = 0; i < vertices->size(); i++) {
    vertexBuffers[context][i].set(vertices->at(i).position);
    texCoordBuffers[context][i].set(vertices->at(i).st);
  }
}

void McpipClip::addDataToPacket(packet2_t* packet, const unsigned char& context,
                                McpipBlock* block, const int& count,
                                RendererCoreTextureBuffers* texBuffers) {
  packet2_reset(packet, false);

  rendererCore->texture.updateClutBuffer(texBuffers->clut);

  packet2_utils_vu_open_unpack(packet, VU1_MCPIP_AS_IS_DYNAMIC_SCALE, true);
  {
    packet2_add_float(packet, 2048.0F);  // scale
    packet2_add_float(packet, 2048.0F);  // scale
    packet2_add_float(packet,
                      static_cast<float>(0xFFFFFF) / 32.0F);  // scale
    packet2_add_s32(packet, count);                           // vertex count

    packet2_utils_gs_add_prim_giftag(packet, prim, count,
                                     ((u64)GIF_REG_ST) << 0 |
                                         ((u64)GIF_REG_RGBAQ) << 4 |
                                         ((u64)GIF_REG_XYZ2) << 8,
                                     3, 0);

    packet2_utils_gs_add_texbuff_clut(packet, texBuffers->core,
                                      &rendererCore->texture.clut);

    Packet2TyraUtils::addColor(packet, *block->color);
  }
  packet2_utils_vu_close_unpack(packet);

  unsigned int addr = VU1_MCPIP_AS_IS_DYNAMIC_VERTEX_DATA_ADDR;

  packet2_utils_vu_add_unpack_data(packet, addr, vertexBuffers[context], count,
                                   true);
  addr += count;

  packet2_utils_vu_add_unpack_data(packet, addr, texCoordBuffers[context],
                                   count, true);
}

void McpipClip::setDBufferSize() {
  vu1DBufferSize = 1000;                                    // VU1 mem size
  vu1DBufferSize -= VU1_MCPIP_AS_IS_STATIC_LAST_DATA_ADDR;  // static  data
  vu1DBufferSize -= 1;
  vu1DBufferSize /= 2;  // xtop double buffer
}

void McpipClip::sendVU1StaticData() {
  dma_channel_wait(DMA_CHANNEL_VIF1, 0);
  dma_channel_send_packet2(staticPacket, DMA_CHANNEL_VIF1, true);
}

}  // namespace Tyra
