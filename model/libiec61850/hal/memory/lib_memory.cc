#include "ns3/lib_memory.h"

namespace libiec61850
{

static MemoryExceptionHandler exceptionHandler = nullptr;
static void* exceptionHandlerParameter = nullptr;

static void
noMemoryAvailableHandler(void)
{
    if (exceptionHandler != nullptr)
        exceptionHandler(exceptionHandlerParameter);
}

void
Memory_installExceptionHandler(MemoryExceptionHandler handler, void* parameter)
{
    exceptionHandler = handler;
    exceptionHandlerParameter = parameter;
}

void*
Memory_malloc(size_t size)
{
    void* memory = malloc(size);

    if (memory == nullptr)
        noMemoryAvailableHandler();

    return memory;
}


void*
Memory_calloc(size_t nmemb, size_t size)
{
    void* memory = calloc(nmemb, size);

    if (memory == nullptr)
        noMemoryAvailableHandler();

    return memory;
}


void *
Memory_realloc(void *ptr, size_t size)
{
    void* memory = realloc(ptr, size);

    if (memory == nullptr)
        noMemoryAvailableHandler();

    return memory;
}

void
Memory_free(void* memb)
{
    free(memb);
}

}