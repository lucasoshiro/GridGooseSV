#include "ns3/core-module.h"
#include "ns3/hal_thread.h"

constexpr size_t semaphore_size = 32;
typedef std::counting_semaphore<semaphore_size> std_semaphore;

namespace libiec61850
{

struct sThread {
    ThreadExecutionFunction function;
    void* parameter;
    std::thread* thread;
    bool state;
    bool autodestroy;
};

static void threadJob(Thread thread)
{
    thread->function(thread->parameter);
}

static void threadJobDestroyAutomatic(Thread thread)
{
    thread->function(thread->parameter);
    delete thread;
}

PAL_API Thread
Thread_create(ThreadExecutionFunction function, void* parameter, bool autodestroy)
{
    auto thread = new struct sThread;

    if (thread != nullptr)
    {
        thread->parameter = parameter;
        thread->function = function;
        thread->state = false;
        thread->autodestroy = autodestroy;
        thread->thread = nullptr;
    }

    return thread;
}

PAL_API void
Thread_start(Thread thread)
{

    if (thread->autodestroy)
    {
        thread->thread = new std::thread(threadJobDestroyAutomatic, thread);
        thread->thread->detach();
    }
    else
    {
        thread->thread = new std::thread(threadJob, thread);
    }

    thread->state = true;
}

PAL_API void
Thread_destroy(Thread thread)
{
    if (thread->state)
    {
        thread->thread->join();
    }

    delete thread->thread;
    delete thread;
}

PAL_API void
Thread_sleep(int millies)
{
    /* TODO: how do we implement an sleep in terms of simulation? Perhaps we
     * can use the Yoneda lemma to transform a sleep into a schedule?
     */
    (void) millies;
}

PAL_API Semaphore
Semaphore_create(int initialValue)
{
    return new std::counting_semaphore<semaphore_size>(initialValue);
}

PAL_API void
Semaphore_wait(Semaphore self)
{
    static_cast<std_semaphore *>(self)->try_acquire();
}

PAL_API void
Semaphore_post(Semaphore self)
{
    static_cast<std_semaphore *>(self)->release();
}

PAL_API void
Semaphore_destroy(Semaphore self)
{
    delete static_cast<std_semaphore *>(self);
}

}