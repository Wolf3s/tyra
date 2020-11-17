/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../include/modules/vu1.hpp"

#include "../include/utils/debug.hpp"
#include <dma.h>
#include <string.h>
#include <kernel.h>

// ----
// Constructors/Destructors
// ----

VU1::VU1()
{
    PRINT_LOG("Initializing VU1");
    currPacket = 0;
    switchPacket = 0;
    spackets[0] = spacket_create(3072, SPACKET_NORMAL);
    spackets[1] = spacket_create(3072, SPACKET_NORMAL);
    PRINT_LOG("VU1 initialized!");
}

VU1::~VU1() {}

// ----
// Methods
// -

/** Create dynamic list */
void VU1::createList()
{
    switchPacket = !switchPacket;
    memset((char *)&buildList, 0, sizeof(buildList));

    if (switchPacket)
        currPacket = spackets[0];
    else
        currPacket = spackets[1];
    spacket_reset(currPacket, false);
    buildList.kickBuffer = currPacket;
}

/** Add reference list and send via VIF1 
 * Not using TOPS register
 * Similar to addReferenceList() 
 */
void VU1::sendSingleRefList(int t_destAddress, void *t_data, int t_quadSize)
{
    spacket_t *spacket = spacket_create(2, SPACKET_NORMAL);
    spacket_ref(spacket, t_data, t_quadSize, 0, 0, 0, true);
    spacket_vif_stcycl(spacket, 0, 0x0101, 0);
    spacket_open_unpack(spacket, VIF_UNPACK_V4_32, t_destAddress, 0, 0, 0, 0);
    spacket_close_unpack(spacket, t_quadSize);
    spacket_chain_open_end(spacket, 0, 0, true);
    spacket_vif_nop(spacket, 0);
    spacket_vif_nop(spacket, 0);
    spacket_chain_close_tag(spacket);
    spacket_send_chain(spacket, DMA_CHANNEL_VIF1, true, true, true);
}

/** Add list beginning, set's double buffer if not set  */
void VU1::addListBeginning()
{
    if (buildList.isBuilding == 1)
        PRINT_ERR("Please end current list list before adding new one!");

    if (isDoubleBufferSet == 0)
        addDoubleBufferSetting();
    else
        addFlush();

    buildList.dmaSizeAll += buildList.dmaSize;
    buildList.dmaSize = 0;
    buildList.offset = currPacket;
    spacket_chain_open_cnt(currPacket, 0, 0, 0, true);
    spacket_vif_stcycl(currPacket, 0, 0x0101, 0);
    spacket_open_unpack(currPacket, V4_32, 0, 1, 0, 0, 0);
    buildList.isBuilding = 1;
}

/** Add list ending and fix unpack size */
void VU1::addListEnding()
{
    if (buildList.isBuilding == 0)
    {
        PRINT_ERR("Please add list beginning first. Nothing to end!");
        return;
    }
    spacket_align_to_qw(currPacket);
    spacket_chain_close_tag(currPacket);
    spacket_close_unpack(currPacket, ((u32)currPacket->next - (u32)currPacket->unpackOpenedAt) >> 4);
    buildList.isBuilding = 0;
}

/** Add list which will load data from given pointer
 * A lot faster than standard list.
 * @param offset offset before data in quadwords
 * @param data data pointer
 * @param size in quadwords
 * @param useTops when true, data will be loaded at the beginning of buffer (BASE+OFFSET)
 */
void VU1::addReferenceList(u32 t_offset, void *t_data, u32 t_size, u8 t_useTops)
{
    checkDataAlignment(t_data);
    if (buildList.isBuilding == 1)
        PRINT_ERR("Please end current list list before adding new one!");
    spacket_ref(currPacket, t_data, t_size, 0, 0, 0, 1);
    spacket_vif_stcycl(currPacket, 0, 0x0101, 0);
    spacket_open_unpack(currPacket, V4_32, 0, t_useTops, 0, 0, 0);
    spacket_close_unpack(currPacket, t_useTops ? ((u32)currPacket->next - (u32)currPacket->unpackOpenedAt) >> 4 : t_offset >> 4);
    buildList.dmaSize += t_size * 8;
    buildList.dmaSizeAll += buildList.dmaSize;
}

/** Start VU1 program */
void VU1::addStartProgram()
{
    spacket_chain_open_cnt(currPacket, 0, 0, 0, 1);
    spacket_vif_mscal(currPacket, 0, 0);
    spacket_vif_flush(currPacket, 0);
    spacket_chain_close_tag(currPacket);
}

/** Continue VU1 program from "--cont" line */
void VU1::addContinueProgram()
{
    spacket_chain_open_cnt(currPacket, 0, 0, 0, 1);
    spacket_vif_mscnt(currPacket, 0);
    spacket_vif_flush(currPacket, 0);
    spacket_chain_close_tag(currPacket);
}

/** Add end tag and send packet via VIF1 */
void VU1::sendList()
{
    spacket_chain_open_end(currPacket, 0, 0, 1);
    spacket_vif_nop(currPacket, 0);
    spacket_vif_nop(currPacket, 0);
    spacket_chain_close_tag(currPacket);
    printf("QWS:%d\n", spacket_get_qw_count(currPacket));
    spacket_send_chain(currPacket, DMA_CHANNEL_VIF1, true, true, true);
}

void VU1::addDoubleBufferSetting()
{
    spacket_chain_open_cnt(currPacket, 0, 0, 0, 1);
    spacket_vif_base(currPacket, 8, 0);
    spacket_vif_offset(currPacket, 496, 0);
    isDoubleBufferSet = 1;
    spacket_chain_close_tag(currPacket);
}

void VU1::addFlush()
{
    spacket_chain_open_cnt(currPacket, 0, 0, 0, 1);
    spacket_vif_nop(currPacket, 0);
    spacket_vif_flush(currPacket, 0);
    spacket_chain_close_tag(currPacket);
}

void VU1::checkDataAlignment(void *t_data)
{
    if (((u32)t_data & 0xF))
        PRINT_ERR("data is not 16 byte aligned!");
}

// ----
// List adding
// ----

void VU1::add128(u64 v1, u64 v2)
{
    checkList();
    spacket_add_u64(currPacket, v1);
    spacket_add_u64(currPacket, v2);
    buildList.dmaSize += 16;
}

void VU1::add64(u64 v)
{
    checkList();
    spacket_add_u64(currPacket, v);
    buildList.dmaSize += 8;
}

void VU1::add32(u32 v)
{
    checkList();
    spacket_add_u32(currPacket, v);
    buildList.dmaSize += 4;
}

void VU1::addFloat(float v)
{
    checkList();
    spacket_add_float(currPacket, v);
    buildList.dmaSize += 4;
}

void VU1::checkList()
{
    if (buildList.isBuilding == 0)
        PRINT_ERR("Please add list beginning before adding data!");
    // if (buildList.dmaSizeAll > VIF_BUFFER_SIZE)
    //     PRINT_ERR("Buffer size exceed!");
}

// ----
// Static
// ----

/** TODO */
void VU1::uploadProgram(int t_dest, u32 *t_start, u32 *t_end)
{
    // get the size of the code as we can only send 256 instructions in each MPGtag
    int count = VU1::countProgramSize(t_start, t_end);
    spacket_t *spacket = spacket_create(20, SPACKET_NORMAL);

    while (count > 0)
    {
        u32 currentCount = count > 256 ? 256 : count;
        spacket_ref(spacket, t_start, currentCount / 2, 0, 0, 0, 1);
        spacket_vif_nop(spacket, 0);
        spacket_vif_mpg(spacket, currentCount & 0xFF, t_dest, 0);
        t_start += currentCount * 2;
        count -= currentCount;
        t_dest += currentCount;
    }
    spacket_chain_open_end(spacket, 0, 0, true);
    spacket_vif_nop(spacket, 0);
    spacket_vif_nop(spacket, 0);
    spacket_chain_close_tag(spacket);
    spacket_send_chain(spacket, DMA_CHANNEL_VIF1, true, true, true);
}

u32 VU1::countProgramSize(u32 *t_start, u32 *t_end)
{
    u32 size = (t_end - t_start) / 2;
    if (size & 1)
        size++;
    return size;
}
