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
#include <limits.h>
#include <unistd.h>
#include <cstring>

#include "../utils/debug.hpp"
#include "../utils/string.hpp"

#ifdef NDEBUG
#define FS_SOURCE ""
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
    char *getBasePath();

private:
    char cwd[NAME_MAX];

    // Argv name+path & just path
    char elfName[NAME_MAX] __attribute__((aligned(16)));
    char elfPath[NAME_MAX - 14]; // It isn't 256 because elfPath will add subpaths

    void setPathInfo(char *path);
};

#endif
