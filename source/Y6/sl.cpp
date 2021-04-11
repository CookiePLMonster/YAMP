#include "sl.h"

#include <algorithm>

namespace sl {

context_t* sm_context;
handle_t* (*handle_create_internal)(handle_t* obj, void* ptr, uint32_t type);

void mutex_construct(mutex_t& mutex)
{
    InitializeCriticalSectionAndSpinCount(&mutex.m_cs, 4096);
}

void mutex_destruct(mutex_t& mutex)
{
    DeleteCriticalSection(&mutex.m_cs);
}

void spinlock_lock(spinlock_t& spinlock)
{
    uint32_t itersToPause = 4;
    while (true)
    {
        uint32_t iters = 0;
        while (spinlock.m_lock_status != 0)
        {
            if (++iters > 0x10000)
            {
                iters = 0;
                SleepEx(0, TRUE);
            }
            _mm_pause();
        }
        if (InterlockedCompareExchange(&spinlock.m_lock_status, 1, 0) == 0)
        {
            break;
        }
        for (uint32_t pause = 0; pause != itersToPause; pause++)
        {
            _mm_pause();
        }
        itersToPause = std::max(itersToPause * 2, 512u);

    }
    _mm_mfence();
}

void spinlock_unlock(spinlock_t& spinlock)
{
    _mm_mfence();
    spinlock.m_lock_status = 0;
}

handle_t handle_create(void* ptr, uint32_t type)
{
    handle_t result{};
    handle_create_internal(&result, ptr, type);
    return result;
}

void file_handle_lock::_afterConstruct()
{
    sl::file_handle_event* event1 = new sl::file_handle_event {};
    sl::file_handle_event* event2 = new sl::file_handle_event {};
    event1->_afterConstruct();
    event2->_afterConstruct();

    eventHandle1 = sl::handle_create(event1, 3);
    eventHandle2 = sl::handle_create(event2, 3);
    
    sl::mutex_construct(critSec1);
    sl::mutex_construct(critSec2);
}

void file_handle_event::_afterConstruct()
{
    eventHandle = CreateEventW(nullptr, TRUE, TRUE, nullptr);
}

void file_handle_internal_t::_afterConstruct()
{
    sl::file_handle_event* event = new sl::file_handle_event {};
	event->_afterConstruct();

    m_async_event = sl::handle_create(event, 3);
}

}