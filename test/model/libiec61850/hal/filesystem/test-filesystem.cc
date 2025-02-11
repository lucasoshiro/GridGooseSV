//
// Created by lso on 11/02/25.
//

#include "test-filesystem.h"
#include "ns3/hal_filesystem.h"

#include <stdio.h>

std::string getMockDir()
{
    std::string filePath = __FILE__;
    std::string dirName = filePath.substr(0, filePath.find_last_of('/'));
    return dirName + "/../../../../mock/";
}

std::string mockFilePath(std::string mockFile)
{
    return getMockDir() + mockFile;
}

void
TestFilesystem::DoRun()
{
    char fileFoobar[1024];
    u_int8_t buffer[8];
    strcpy(fileFoobar, mockFilePath("foobar.txt").c_str());

    auto file = libiec61850::FileSystem_openFile(fileFoobar, false);

    libiec61850::FileSystem_readFile(file, buffer, 6);
    buffer[6] = '\0';
    libiec61850::FileSystem_closeFile(file);

    NS_TEST_ASSERT_MSG_EQ(
        std::string(reinterpret_cast<char*>(buffer)),
        "foobar",
        "Expected buffer to be foobar"
        );
}