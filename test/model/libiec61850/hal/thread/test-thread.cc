//
// Created by lso on 11/02/25.
//

#include "test-thread.h"

#include "ns3/hal_thread.h"

#include <semaphore>

struct _params
{
    int *p;
    libiec61850::Semaphore sem;
};

static void* thread_func(void* arg)
{
    const auto params = static_cast<struct _params*>(arg);

    libiec61850::Semaphore_wait(params->sem);
    const int n = *(params->p);
    *(params->p) = n + 7;
    libiec61850::Semaphore_post(params->sem);

    return nullptr;
}

void
TestThread::DoRun()
{
    int n = 0;
    libiec61850::Semaphore sem = libiec61850::Semaphore_create(0);

    struct _params params = {.p = &n, .sem = sem};

    libiec61850::Thread threads[3];

    for (int i = 0; i < 3; ++i)
    {
        threads[i] = libiec61850::Thread_create(thread_func, &params, false);;
    }

    NS_TEST_ASSERT_MSG_EQ(n, 0, "Expected n to be 0");

    libiec61850::Thread_start(threads[0]);
    libiec61850::Thread_destroy(threads[0]);

    NS_TEST_ASSERT_MSG_EQ(n, 7, "Expected n to be 7");

    libiec61850::Thread_start(threads[1]);
    libiec61850::Thread_start(threads[2]);

    libiec61850::Thread_destroy(threads[1]);
    libiec61850::Thread_destroy(threads[2]);

    NS_TEST_ASSERT_MSG_EQ(n, 21, "Expected n to be 21");

    libiec61850::Semaphore_destroy(sem);
}