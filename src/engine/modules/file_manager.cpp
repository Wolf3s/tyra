#include "../include/modules/file_manager.hpp"

FileManager::FileManager()
{
    getcwd(cwd, sizeof(cwd));
    printf("Booting from %s\n", cwd);
    setPathInfo(cwd);
}

FileManager::~FileManager()
{
}

FILE *FileManager::openFile(const char *t_subfolder, const char *t_name, const char *t_extension)
{
    char *path_part1 = String::createConcatenated(t_subfolder, t_name);
    char *path = String::createConcatenated(path_part1, t_extension);
    delete[] path_part1;

    return openFile(path);
}

FILE *FileManager::openFile(const char *t_dir, const char *t_file)
{
    char *path = String::createConcatenated(t_dir, t_file);
    return openFile(path);
}

FILE *FileManager::openFile(const char *t_path)
{
    char *path = String::createConcatenated(this->getBasePath(), t_path);

    printf("Opening file: %s\n", path);
    FILE *file = fopen(path, "rb");

    if (file == NULL)
    {
        char *errorMsg = String::createConcatenated("Failed to load the file: ", path);
        assertMsg(file != NULL, errorMsg);
    }

    return file;
}

void FileManager::setPathInfo(char *path)
{
    char *ptr;

    strcpy(this->elfName, path);
    strcpy(this->elfPath, path);

    ptr = strrchr(this->elfPath, '/');
    if (ptr == NULL)
    {
        ptr = strrchr(this->elfPath, '\\');
        if (ptr == NULL)
        {
            ptr = strrchr(this->elfPath, ':');
            if (ptr == NULL)
            {
                printf("Did not find path (%s)!\n", path);
                SleepThread();
            }
        }
    }

    ptr++;
    *ptr = '\0';

    printf("ELF name is %s\n", this->elfName);
    printf("path is %s\n", this->elfPath);
}

char *FileManager::getBasePath()
{
    return this->cwd;
}
