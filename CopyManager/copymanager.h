#ifndef COPYMANAGER_H
#define COPYMANAGER_H

#include "CopyManager_global.h"

#if defined (_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <string>
#include <list>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>


class COPYMANAGER_EXPORT CopyManager
{
public:
    CopyManager();
    CopyManager(const std::list<CMGlobal::FileInfo>& file_list);
    ~CopyManager();

    void runCopy();
    void interrupt();
    bool isInterrupted() const noexcept;

    int progress() const noexcept;
    bool isComplete() const noexcept;
    std::queue<std::string>& getErrorQueue();

private:
    void readDir();
    void writeDir();

private:
    std::mutex m_mutex;
    std::mutex m_lockerprint;
    std::list<CMGlobal::FileInfo> m_fileList;
    std::queue<CMGlobal::FileInfo> m_queue;
    std::condition_variable m_cv;

    std::thread m_th1;
    std::thread m_th2;

    std::atomic_bool m_interrupted;
    std::atomic_bool m_canRead;
    std::atomic_bool m_complete;

    std::atomic_int m_progress;

    std::queue<std::string> m_error_queue;
};


#if defined (_MSC_VER)
#pragma warning(pop)
#endif

#endif // COPYMANAGER_H
