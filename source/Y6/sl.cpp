#include "sl.h"

#include <algorithm>

namespace sl {

context_t* sm_context;
handle_t* (*handle_create_internal)(handle_t* obj, void* ptr, uint32_t type);
void (*archive_lock_wlock)(handle_t handle);
void (*archive_lock_wunlock)(handle_t handle);

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

void rwspinlock_wlock(rwspinlock_t& spinlock)
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
        if (InterlockedCompareExchange(&spinlock.m_lock_status, 0x80000000, 0) == 0)
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

void rwspinlock_wunlock(rwspinlock_t& spinlock)
{
    // TODO: This might be wrong
    _mm_mfence();
    spinlock.m_lock_status = 0;
}

void rwspinlock_rlock(rwspinlock_t& spinlock)
{
    uint32_t itersToPause = 4;
    while (true)
    {
        int32_t curStatus = spinlock.m_lock_status;
        uint32_t iters = 0;
        while ((spinlock.m_lock_status & 0x800000) != 0)
        {
            if (++iters > 0x10000)
            {
                iters = 0;
                SleepEx(0, TRUE);
            }
            _mm_pause();
            curStatus = spinlock.m_lock_status;
        }
        if (InterlockedCompareExchange(&spinlock.m_lock_status, curStatus + 1, curStatus) == curStatus)
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

void rwspinlock_runlock(rwspinlock_t& spinlock)
{
    InterlockedExchangeAdd(&spinlock.m_lock_status, 0xFFFFFFFF);
}

handle_t semaphore_create(uint32_t initialCount)
{
    handle_t result;
    HANDLE semaphore = CreateSemaphoreW(nullptr, initialCount, LONG_MAX, nullptr);
    if (semaphore != INVALID_HANDLE_VALUE)
    {
        semaphore_internal_t* sema = new semaphore_internal_t;
        sema->h_semaphore = semaphore;
        result = handle_create(sema, 2);
    }
    return result;
}

namespace {

const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
    DWORD dwType; // Must be 0x1000.
    LPCSTR szName; // Pointer to name (in user addr space).
    DWORD dwThreadID; // Thread ID (-1=caller thread).
    DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)
void SetThreadName(DWORD dwThreadID, const char* threadName) {
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = threadName;
    info.dwThreadID = dwThreadID;
    info.dwFlags = 0;
#pragma warning(push)
#pragma warning(disable: 6320 6322)
    __try{
        RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
    }
    __except (EXCEPTION_EXECUTE_HANDLER){
    }
#pragma warning(pop)
}
}

static DWORD WINAPI stub_thread_start_routine(LPVOID arg)
{
    // TODO: The original game code is full of XMM stores here
    // Saving XMM state somehow?
    thread_internal_t* thread = reinterpret_cast<thread_internal_t*>(arg);
    SetThreadName(thread->thread_id, thread->sz_name);

    // TODO: thread_sid_register
    DWORD result = thread->p_routine(thread->arg);
    // TODO: thread_sid_unregister

    return result;
}

// TODO: Originally this function had more parameters
handle_t thread_create(uint32_t (*p_routine)(uint64_t), uint64_t arg, const char* name)
{  
    thread_internal_t* thread = new thread_internal_t;
    thread->p_routine = p_routine;
    thread->arg = arg;
    strcpy_s(thread->sz_name, name);
    handle_t result = handle_create(thread, 1);

    DWORD threadId;
    thread->h_thread = CreateThread(nullptr, 0x10000, stub_thread_start_routine, thread, CREATE_SUSPENDED, &threadId);
    if (thread->h_thread != nullptr)
    {
        thread->thread_id = threadId;
        // TODO: Affinity and priority
        ResumeThread(thread->h_thread);
    }
    return result;
}

handle_t handle_create(void* ptr, uint32_t type)
{
    handle_t result{};
    handle_create_internal(&result, ptr, type);
    return result;
}

void archive_lock::_afterConstruct()
{
    sl::file_handle_event* event1 = new sl::file_handle_event {};
    sl::file_handle_event* event2 = new sl::file_handle_event {};
    event1->_afterConstruct();
    event2->_afterConstruct();

    eventHandle1 = sl::handle_create(event1, 3);
    eventHandle2 = sl::handle_create(event2, 3);
}

void file_handle_event::_afterConstruct()
{
    eventHandle = CreateEventW(nullptr, TRUE, TRUE, nullptr);
}

void file_handle_internal_t::end_async_request()
{
    m_req_item_index = 0;
    m_flags &= 0xFFFFFFFB;

    file_handle_event* event = handle_instance<file_handle_event>(m_async_event, 3);
    SetEvent(event->eventHandle);
}

void file_handle_internal_t::callback(FILE_ASYNC_METHOD type, uint32_t status)
{
    auto func = mp_callback_func;
    auto handle = m_handle;
    auto param = mp_callback_param;
    if (m_callback_method == type)
    {
        if (func != nullptr)
        {
            sl::rwspinlock_rlock(m_locked);
            uint32_t flags = m_flags >> 28;
            sl::rwspinlock_runlock(m_locked);
            while (flags & 1)
            {
                SleepEx(0, TRUE);
                sl::rwspinlock_rlock(m_locked);
                flags = m_flags >> 28;
                sl::rwspinlock_runlock(m_locked);
            }
        }
    }
    else
    {
        func = nullptr;
    }

    sl::rwspinlock_wlock(m_locked);
    m_last_async_status = status;
    if ((m_flags & 0x1A) != 0)
    {
        func = nullptr;
        m_flags &= 0xFBFFFFFF;
    }
    else if (func != nullptr)
    {
        m_flags |= 0x50000000;
        if (type >= FILE_ASYNC_METHOD_PRELOAD)
            m_callback_execute_thread = GetCurrentThreadId();
    }
    end_async_request();
    sl::rwspinlock_wunlock(m_locked);
    if (func != nullptr)
    {
        if (type == FILE_ASYNC_METHOD_READ || type == FILE_ASYNC_METHOD_WRITE)
        {
            assert(!"t_lockfree_stack unimplemented!");
        }
        else
        {
            func(m_handle, status, mp_callback_param);
            sl::rwspinlock_wlock(m_locked);
            m_callback_execute_thread = 0;
            const unsigned int destroyFlag = m_flags >> 27;
            m_flags &= 0x83FFFFFF;
            sl::rwspinlock_wunlock(m_locked);
            if (destroyFlag & 1)
                sl::file_handle_destroy(this);
        }
    }
}

void file_handle_internal_t::_afterConstruct()
{
    sl::file_handle_event* event = new sl::file_handle_event {};
	event->_afterConstruct();

    m_async_event = sl::handle_create(event, 3);
}

sl::mutex_t::mutex_t()
{
    mutex_construct(*this);
}

sl::mutex_t::~mutex_t()
{
    mutex_destruct(*this);
}

void mutex_t::lock()
{
    EnterCriticalSection(&m_cs);
}

void mutex_t::unlock()
{
    LeaveCriticalSection(&m_cs);
}

}