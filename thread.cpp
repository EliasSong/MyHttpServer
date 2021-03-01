#include "thread.h"
#include "util.h"
#include "log.h"
namespace PangTao
{
    static thread_local Thread *t_thread = nullptr;
    static thread_local std::string t_thread_name = "UNKNOW";

    Semaphore::Semaphore(uint32_t count)
    {
        if (sem_init(&m_semaphore, 0, count))
        {
            throw std::logic_error("sem init failed");
        }
    }
    Semaphore::~Semaphore()
    {
        sem_destroy(&m_semaphore);
    }
    void Semaphore::wait()
    {
        if (sem_wait(&m_semaphore))
        {
            throw std::logic_error("sem wait failed");
        }
    }
    void Semaphore::notify()
    {
        if (sem_post(&m_semaphore))
        {
            throw std::logic_error("sem post failed");
        }
    }

    Thread *Thread::GetThis()
    {
        return t_thread;
    }
    const std::string &Thread::GetName()
    {
        return t_thread_name;
    }

    void Thread::SetName(const std::string &name)
    {
        if (t_thread)
        {
            t_thread->m_name = name;
        }
        t_thread_name = name;
    }

    Thread::Thread(std::function<void()> cb, const std::string &name) : m_cb(cb), m_name(name)
    {
        if (name.empty())
        {
            m_name = "UNKNOW";
        }
        int rt = pthread_create(&m_thread, nullptr, &Thread::run, this);
        if (rt)
        {
            PANGTAO_LOG_ERROR(PANGTAO_ROOT_LOGGER, "pthread_create thread failed. name = " + name);
            throw std::logic_error("pthread_create thread failed.");
        }
        m_semaphore.wait();
    }
    Thread::~Thread()
    {
        if (m_thread)
        {
            pthread_detach(m_thread);
        }
    }
    void Thread::join()
    {
        if (m_thread)
        {
            int rt = pthread_join(m_thread, nullptr);
            if (rt)
            {
                PANGTAO_LOG_ERROR(PANGTAO_ROOT_LOGGER, "pthread_join thread failed. name = " + m_name);
                throw std::logic_error("pthread_join thread failed.");
            }
            m_thread = 0;
        }
    }
    void *Thread::run(void *arg)
    {
        Thread *thread = (Thread *)arg;
        t_thread = thread;
        thread->m_id = PangTao::GetThreadId();
        t_thread_name = thread->m_name;
        pthread_setname_np(pthread_self(), thread->m_name.substr(0, 15).c_str());
        std::function<void()> cb;
        cb.swap(thread->m_cb);
        thread->m_semaphore.notify();
        cb();
        return 0;
    }

} // namespace PangTao
