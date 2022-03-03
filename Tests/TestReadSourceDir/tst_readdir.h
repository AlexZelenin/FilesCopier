#ifndef TST_READDIR_H
#define TST_READDIR_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "copymanager.h"

using namespace testing;

TEST(ReadSourceDir, readDir)
{
    std::list<CMGlobal::FileInfo> list_files;
    CMGlobal::FileInfo file_info;
    file_info.filename = std::string("Common.pri");
    file_info.file_size = 125;
    file_info.pathFrom = std::string("testdirs/from/");
    file_info.pathTo = std::string("testdirs/to/");
    file_info.type = CMGlobal::FileInfo::File;
    list_files.push_back(file_info);
    CopyManager *copyManager = new CopyManager(list_files);

    copyManager->runCopy();

    int i = 0;
    while(!copyManager->progress()) {

    }

    if (!copyManager->getErrorQueue().empty()) {
        EXPECT_TRUE(false) << copyManager->getErrorQueue().front();
        copyManager->getErrorQueue().pop();
    } else {
        i = copyManager->progress();
    }
    copyManager->wait();
    delete copyManager;

    EXPECT_EQ(i, 1);
}

#endif // TST_READDIR_H
