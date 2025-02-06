#include "ns3/hal_thread.h"
#include "ns3/core-module.h"

const size_t semaphore_size = 32;
typedef std::counting_semaphore<semaphore_size> std_semaphore;

namespace libiec61850
{

/**
 * \brief Create a new Thread instance
 *
 * \param function the entry point of the thread
 * \param parameter a parameter that is passed to the threads start function
 * \param autodestroy the thread is automatically destroyed if the ThreadExecutionFunction has finished.
 *
 * \return the newly created Thread instance
 */
PAL_API Thread
Thread_create(ThreadExecutionFunction function, void* parameter, bool autodestroy);

/**
 * \brief Start a Thread.
 *
 * This function invokes the start function of the thread. The thread terminates when
 * the start function returns.
 *
 * \param thread the Thread instance to start
 */
PAL_API void
Thread_start(Thread thread);

/**
 * \brief Destroy a Thread and free all related resources.
 *
 * \param thread the Thread instance to destroy
 */
PAL_API void
Thread_destroy(Thread thread);

/**
 * \brief Suspend execution of the Thread for the specified number of milliseconds
 */
PAL_API void
Thread_sleep(int millies);

PAL_API Semaphore
Semaphore_create(int initialValue)
{
    return new std::counting_semaphore<semaphore_size>(initialValue);
}

/* Wait until semaphore value is greater than zero. Then decrease the semaphore value. */
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