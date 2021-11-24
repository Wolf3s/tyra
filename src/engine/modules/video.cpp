/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# André Guilherme <andregui17@outlook.com>
# Based on Eugene Plotnikov work SIMPLE MEDIA SYSTEM
*/

#include "../include/video/mpeg.hpp"
#include "modules/file_service.hpp"
#include "../include/utils/string.hpp"
#include "../include/utils/debug.hpp"
#include <loadfile.h>
#include <cstdlib>
#include <string.h>
#include <stdio.h>

FILE *avi;
FILE *mpg;
FILE *m2v;
FILE *m1v;
u32 videoLoaded;
u8 audioLoaded;
u8 fullFilename;
//Initialize video
void Video::MPEG_Initialize(char *path)
{
  if (videoLoaded && audioLoaded)
  {
    MPEGSequenceInfo *m_Width;
    MPEGSequenceInfo *m_Height;
    MPEGSequenceInfo *m_frame_cnt;
    MPEGSequenceInfo *m_VideoFmt;

    char *fullFilename = String::createConcatenated("host:", path);
    avi = fopen(fullFilename, "loaded");
    mpg = fopen(fullFilename, "loaded");
    m1v = fopen(fullFilename, "loaded");
    m2v = fopen(fullFilename, "loaded");
    videoLoaded = true;
    consoleLog("Loaded video");
   switch ('video')
   {
      case 'avi':
      {
        if (MPEG_VIDEO_FORMAT_NTSC)
        {          
          avi;
          m_VideoFmt->m_Profile;
          m_Width->m_Height;
          m_frame_cnt;
        }

        if (MPEG_VIDEO_FORMAT_PAL)
        {
           avi;
           m_Width->m_Height;
           m_frame_cnt->m_ChromaFmt;
        }
      }
      break;
      
      case 'mpg':
      {
         if (MPEG_VIDEO_FORMAT_NTSC)
         {
            mpg;
            m_Width->m_Height;
            m_frame_cnt;
         }

         if (MPEG_VIDEO_FORMAT_PAL)
         {
           mpg;
           m_Width->m_Height;
           m_frame_cnt;
         }
         break;   
      }
      case 'm2v':
      {
         if (MPEG_VIDEO_FORMAT_NTSC)
         {
            m2v;
            m_Width->m_Height;
            m_frame_cnt;
         }

         if (MPEG_VIDEO_FORMAT_PAL)
         {
           m2v;
           m_Width->m_Height;
           m_frame_cnt;
         }
         break;
      }
      case 'm1v':
      {
          if (MPEG_VIDEO_FORMAT_NTSC)
         {
            m1v;
            m_Width->m_Height;
            m_frame_cnt;
         }

         if (MPEG_VIDEO_FORMAT_PAL)
         {
           m1v;
           m_Width->m_Height;
           m_frame_cnt;
         }
         break;
      }
      
      default:
      {
        assertMsg(avi != NULL, "Failed to open avi file");
        assertMsg(mpg != NULL, "Failed to open mpg file");
        assertMsg(m1v != NULL, "Failed to open m1v file");
        assertMsg(m2v != NULL, "Failed to open m2v file");
      } 
    }
  }
}

void Video::MPEG_set_volume(int volume){};

//mpeg suspend
void Video::MPEG_Suspend()
{
  videoLoaded = false;
  fclose(mpg);
  fclose(avi);
  fclose(m1v);
  fclose(m2v);
}

//resume video
void Video::MPEG_Resume()
{
  String::getLength;
  MPEG_Resume();
}

//restart
void Video::MPEG_Reset()
{
 constexpr u32 reset = true;
}






