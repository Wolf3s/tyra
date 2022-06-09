/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Wellington Carvalho <wellcoj@gmail.com>
*/

#ifndef _TYRA_FILE_MANAGER_
#define _TYRA_FILE_MANAGER_

#include <tamtypes.h>
#include <cstdio>
#include <kernel.h>
#include "../utils/debug.hpp"
#include "../utils/string.hpp"

#ifdef NDEBUG
#define FS_SOURCE "mass:"
#else
#define FS_SOURCE "host:"
#endif

class FileManager
{

public:
    FileManager();
    ~FileManager();

    /**
     * Read file from source. If NDEBUG read from 'mass:' alse read from 'host:'
     * @returns FILE *
     */
    FILE *openFile(const char *t_subfolder, const char *t_name, const char *t_extension);
    FILE *openFile(const char *t_dir, const char *t_file);
    FILE *openFile(const char *t_path);
};

#endif
