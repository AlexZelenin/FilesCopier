#include "copymanager.h"

#include <future>
#include <fstream>

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif


CopyManager::CopyManager()
{
}

CopyManager::CopyManager(const std::list<CMGlobal::FileInfo>& file_list)
    : m_fileList(file_list),
      m_interrupted{false},
      m_canRead{false},
      m_complete{false},
      m_progress{0}
{
}

CopyManager::~CopyManager()
{
    m_th1.join();
    m_th2.join();
}

void CopyManager::runCopy()
{
    m_th1 = std::thread([this](){ readDir(); });
    m_th2 = std::thread([this](){ writeDir(); });
}

void CopyManager::interrupt()
{
    m_interrupted = !m_interrupted;
}

bool CopyManager::isInterrupted() const noexcept
{
    return m_interrupted;
}

int CopyManager::progress() const noexcept
{
    return m_progress.load(std::memory_order_relaxed);
}

std::queue<std::string>& CopyManager::getErrorQueue()
{
    return m_error_queue;
}

void CopyManager::readDir()
{
    for (const auto& file : m_fileList) {

        if (isInterrupted()) {
            break;
        }

        if (file.type & CMGlobal::FileInfo::Directory) continue;

        std::string path = file.pathFrom;
        path.append("/").append(file.filename);

        std::ifstream file_i(path, std::ios::in | std::ios::binary);

        if (!file_i) {
            std::string errorMessage = std::string(": error read file ") + file.filename;
            m_error_queue.push(errorMessage);
        }

        CMGlobal::FileInfo file_info = file;
        file_info.buffer = new char[file_info.file_size * sizeof(std::size_t)];
        file_i.read(file_info.buffer, file.file_size);

        m_queue.push(file_info);

        file_i.close();
        m_canRead = true;
        m_cv.notify_one();

    }

    std::unique_lock<std::mutex> lock(m_mutex);
    m_complete = true;
    m_cv.notify_one();
}

void CopyManager::writeDir()
{
    while(!isInterrupted()) {

        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock, [this](){
            while(!m_canRead) { return false; }
            return true;
        });

        if (m_complete && m_queue.empty()) return;

        while (!m_queue.empty()) {

            CMGlobal::FileInfo file_info = m_queue.front();
            m_queue.pop();

            std::string path = file_info.pathTo;
            path.append("/").append(file_info.filename);

            std::ofstream file_o(path, std::ios::out | std::ios::binary);

            if (!file_o) {
                std::string errorMessage = std::string(": error write file ") + file_info.filename;
                m_error_queue.push(errorMessage);
            }

            file_o.write(file_info.buffer, file_info.file_size);

            delete[] file_info.buffer;
            file_o.close();

            m_progress.fetch_add(1, std::memory_order_relaxed);
        }
    }
}
