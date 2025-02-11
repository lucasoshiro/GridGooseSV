#include <string.h>
#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "ns3/hal_filesystem.h"
#include "ns3/lib_memory.h"

namespace libiec61850
{
struct sDirectoryHandle {
    DIR* handle;
};

FileHandle
FileSystem_openFile(char* fileName, bool readWrite)
{
    FileHandle newHandle = nullptr;

    if (readWrite)
        newHandle = static_cast<FileHandle>(fopen(fileName, "w"));
    else
        newHandle = static_cast<FileHandle>(fopen(fileName, "r"));

    return newHandle;
}

int
FileSystem_readFile(FileHandle handle, uint8_t* buffer, int maxSize)
{
    return fread(buffer, maxSize, 1, static_cast<FILE*>(handle));
}

int
FileSystem_writeFile(FileHandle handle, uint8_t* buffer, int size)
{
    return fwrite(buffer, size, 1, static_cast<FILE*>(handle));
}

void
FileSystem_closeFile(FileHandle handle)
{
    fclose(static_cast<FILE*>(handle));
}

bool
FileSystem_deleteFile(char* filename)
{
    return remove(filename) == 0;
}

bool
FileSystem_renameFile(char* oldFilename, char* newFilename)
{
    return rename(oldFilename, newFilename) == 0;
}


bool
FileSystem_getFileInfo(char* filename, uint32_t* fileSize, uint64_t* lastModificationTimestamp)
{
    struct stat fileStats;

    if (stat(filename, &fileStats) == -1)
        return false;

    if (lastModificationTimestamp != nullptr)
        *lastModificationTimestamp = static_cast<uint64_t>(fileStats.st_mtime) * 1000LL;

    if (fileSize != nullptr)
        *fileSize = fileStats.st_size;

    return true;
}

DirectoryHandle
FileSystem_openDirectory(char* directoryName)
{
    DIR* dirHandle = opendir(directoryName);

    DirectoryHandle handle = nullptr;

    if (dirHandle != nullptr) {
        handle = static_cast<DirectoryHandle>(GLOBAL_MALLOC(sizeof(struct sDirectoryHandle)));
        handle->handle = dirHandle;
    }

    return handle;
}

char*
FileSystem_readDirectory(DirectoryHandle directory, bool* isDirectory)
{
    struct dirent* dir;

    dir = readdir(directory->handle);

    if (dir != nullptr) {
        if (dir->d_name[0] == '.')
            return FileSystem_readDirectory(directory, isDirectory);
        else {
            if (isDirectory != nullptr) {
                if (dir->d_type == DT_DIR)
                    *isDirectory = true;
                else
                    *isDirectory = false;
            }

            return dir->d_name;
        }
    }
    else
        return nullptr;
}

void
FileSystem_closeDirectory(DirectoryHandle directory)
{
    closedir(directory->handle);
    GLOBAL_FREEMEM(directory);
}
}