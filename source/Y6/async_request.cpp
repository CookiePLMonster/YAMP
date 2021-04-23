#include "async_request.h"

#include <cstdio>
#include <utility>

uint32_t csl_file_async_request::sm_serial = 0;

csl_file_async_request::csl_file_async_request(isl_file_access** pp_interface, uint32_t max_req_item)
	: mpp_file_access(pp_interface)
{
	char threadName[128];
	sprintf_s(threadName, "file_async(%d)", sm_serial++);
	m_request = sl::semaphore_create(0);
	mp_req_item_buf = new req_item_t[max_req_item];
	for (uint32_t i = 0; i < max_req_item; i++)
	{
		m_free_queue.enqueue(&mp_req_item_buf[max_req_item - i - 1]);
	}

	m_h_thread = sl::thread_create(stub_thread_routine, reinterpret_cast<uint64_t>(this), threadName);
}

uint32_t csl_file_async_request::thread_routine()
{
	// TODO: The game uses sl::pcounter_count and sl::pcounter_milli_sec for these but for now storing frequency locally is easier
	int64_t ticksPerSec;
	{
		LARGE_INTEGER li;
		QueryPerformanceFrequency(&li);
		ticksPerSec = li.QuadPart;
	}
	int64_t lastTick = 0;

	// Unknown meaning
	bool skipSemaWait = false;
	bool lastSkipSemaWait = false;

	bool loadingWaitStarted = false;
	while (true)
	{
		if (skipSemaWait)
		{
			lastSkipSemaWait = false;
		}
		else
		{
			sl::semaphore_internal_t* sema = sl::semaphore_handle_instance(m_request);
			WaitForSingleObject(reinterpret_cast<HANDLE>(sema->h_semaphore), INFINITE);
		}

		// TODO: This is probably supposed to be scoped
		m_mutex_request.lock();

		// Try to get the request item
		req_item_t* item = nullptr;
		for (size_t i = 0; i < 6; i++)
		{
			item = m_busy_queue[i].dequeue();
			if (item != nullptr) break;

			item = m_request_queue[i].dequeue();
			if (item != nullptr) break;
		}

		if (m_is_finish_req != 0 && item == nullptr) break; // Terminate thread

		assert(item != nullptr);

		// TODO: Scoped?
		sl::spinlock_lock(item->m_locked);
		// TODO: Beautify, it's probably a "processed" flag
		const unsigned int prevStatus = std::exchange(item->m_status, (item->m_status & 0xFFFF0000) | 0x300);
		sl::spinlock_unlock(item->m_locked);

		m_mutex_request.unlock();

		if ((prevStatus & 0xFF00) == 0x400) // Request done?
		{
			m_free_queue.enqueue(item);
			skipSemaWait = lastSkipSemaWait;
			continue;
		}

		sl::file_handle_internal_t* file = sl::file_handle_instance(item->m_h_file);
		if (item->m_abort)
		{
			m_h_busy_file = {};
			if (file != nullptr)
			{
				sl::rwspinlock_wlock(file->m_locked);
				file->m_last_async_status = (prevStatus & 0xFF0000) | 0x189; // TODO: Verify correctness
				file->end_async_request();
				sl::rwspinlock_wunlock(file->m_locked);
			}
			m_free_queue.enqueue(item);
			skipSemaWait = lastSkipSemaWait;
			continue;
		}

		// Process the file
		m_h_busy_file = item->m_h_file;
		if (file != nullptr)
		{
			isl_file_access* file_access = *mpp_file_access;
			file->m_error_code = 0;

			// TODO: Did this code look like ths originally? It probably has plenty of duplications
			if (file->m_flags & 0x100)
			{
				// TODO: Use sl pcounters
				LARGE_INTEGER time;
				QueryPerformanceCounter(&time);
				if (loadingWaitStarted)
				{
					if ((((time.QuadPart - lastTick) * 1000) / ticksPerSec) < 1000 && !m_is_finish_req)
					{
						SleepEx(0, TRUE);
						m_busy_queue->enqueue(item);
						skipSemaWait = lastSkipSemaWait;

						sl::semaphore_internal_t* sema = sl::semaphore_handle_instance(m_request);
						ReleaseSemaphore(reinterpret_cast<HANDLE>(sema->h_semaphore), 1, nullptr);
						continue;
					}
				}
				else
				{
					loadingWaitStarted = true;
					lastTick = time.QuadPart;
					sl::semaphore_internal_t* sema = sl::semaphore_handle_instance(m_request);
					ReleaseSemaphore(reinterpret_cast<HANDLE>(sema->h_semaphore), 1, nullptr);
					continue;
				}	
			}
			else
			{
				loadingWaitStarted = false;
			}

			const uint32_t asyncMethod = (prevStatus & 0xFF0000) >> 16;
			switch (asyncMethod) // TODO: Verify correctness
			{
			case 1:
			{
				// TODO: Do
				assert(!"Unimplemented");
				break;
			}
			case 2:
			{
				// TODO: Do
				assert(!"Unimplemented");
				break;
			}
			case 3:
			{
				// Command: Close
				if ((file->m_flags & 0xC0) != 0)
				{
					file_access->close(item->m_h_file);
					if ((file->m_flags & 1) == 0)
					{
						// TODO: sl::filecache_close
					}
				}
				// TODO: This will be duplicated, figure out how it may have looked originally
				// pls no gotos
				m_h_busy_file = {};

				uint32_t error = 0;
				const uint32_t newStatus = error | (asyncMethod << 16) | 0x100;
				sl::spinlock_lock(item->m_locked);
				if (item->m_abort)
				{
					sl::rwspinlock_wlock(file->m_locked);
					file->mp_callback_func = nullptr;
					sl::rwspinlock_wunlock(file->m_locked);
				}
				item->m_status = newStatus;
				sl::spinlock_unlock(item->m_locked);

				file->callback(static_cast<sl::FILE_ASYNC_METHOD>(asyncMethod - 1), newStatus);
				if (asyncMethod == 3)
				{
					sl::file_handle_destroy(file);
				}
				m_free_queue.enqueue(item);
				skipSemaWait = lastSkipSemaWait;
				continue;
			}
			case 4:
			{
				// Command: Read
				int64_t bytesRead = 0;
				const uint64_t offset = file->m_read_offset;
				// TODO: mp_cache
				assert(file->mp_cache == nullptr);
				if (file->mp_cache != nullptr)
				{
				
				}
				else
				{
					if (file->m_flags & 1) // Loose file? Probably
					{
						bytesRead = file_access->read_offset(item->m_h_file, file->mp_buffer, file->m_buffer_size, file->m_file_pointer);
						if (bytesRead == -2) // Can this even happen?
						{
							sl::rwspinlock_wlock(file->m_locked);
							file->m_last_async_status = (prevStatus & 0xFF0000) | 0x189; // TODO: Verify correctness
							file->end_async_request();
							sl::rwspinlock_wunlock(file->m_locked);

							m_free_queue.enqueue(item);
							skipSemaWait = lastSkipSemaWait;
							continue;
						}
					}
					else
					{
						// TODO: Tidy up
						uint64_t offsetToRead = 0x20000; // WARNING: This is different in Y:LAD!
						uint64_t fileSpaceLeft = file->m_real_file_size - offset;
						/*if ((file->m_flags >> 9) & 1) // Y:LAD only
							offsetToRead = 0xFFFFFFFF;*/
						if (file->m_buffer_size - offset < offsetToRead)
							offsetToRead = file->m_buffer_size - offset;

						if (offsetToRead < fileSpaceLeft)
							fileSpaceLeft = offsetToRead;

						bytesRead = file_access->read_offset(item->m_h_file, reinterpret_cast<char*>(file->mp_buffer) + file->m_read_offset, fileSpaceLeft, file->m_file_pointer);
					}

					if (bytesRead == -1)
					{
						// TODO: This will be duplicated, figure out how it may have looked originally
						// pls no gotos
						m_h_busy_file = {};

						uint32_t error = file->m_error_code != 0 ? file->m_error_code : 135;
						const uint32_t newStatus = error | (asyncMethod << 16) | 0x100;
						sl::spinlock_lock(item->m_locked);
						if (item->m_abort)
						{
							sl::rwspinlock_wlock(file->m_locked);
							file->mp_callback_func = nullptr;
							sl::rwspinlock_wunlock(file->m_locked);
						}
						item->m_status = newStatus;
						sl::spinlock_unlock(item->m_locked);

						file->callback(static_cast<sl::FILE_ASYNC_METHOD>(asyncMethod - 1), newStatus);
						if (asyncMethod == 3)
						{
							sl::file_handle_destroy(file);
						}
						m_free_queue.enqueue(item);
						skipSemaWait = lastSkipSemaWait;
						continue;
					}
				}
				file->m_read_offset += bytesRead;
				if (file->m_read_offset >= file->m_buffer_size || file->m_file_pointer >= file->m_real_file_size)
				{
					// TODO: This will be duplicated, figure out how it may have looked originally
					// pls no gotos
					m_h_busy_file = {};

					uint32_t error = 0;
					const uint32_t newStatus = error | (asyncMethod << 16) | 0x100;
					sl::spinlock_lock(item->m_locked);
					if (item->m_abort)
					{
						sl::rwspinlock_wlock(file->m_locked);
						file->mp_callback_func = nullptr;
						sl::rwspinlock_wunlock(file->m_locked);
					}
					item->m_status = newStatus;
					sl::spinlock_unlock(item->m_locked);

					file->callback(static_cast<sl::FILE_ASYNC_METHOD>(asyncMethod - 1), newStatus);
					if (asyncMethod == 3)
					{
						sl::file_handle_destroy(file);
					}
					m_free_queue.enqueue(item);
					skipSemaWait = lastSkipSemaWait;
					continue;
				}

				sl::spinlock_lock(item->m_locked);
				if (!item->m_abort)
				{
					item->m_status = (item->m_status & 0xFFF0000) | 0x200;
					sl::spinlock_unlock(item->m_locked);
					m_busy_queue->enqueue(item);
					lastSkipSemaWait = true;
				}
				else
				{
					sl::spinlock_unlock(item->m_locked);

					sl::rwspinlock_wlock(file->m_locked);
					file->m_last_async_status = (prevStatus & 0xFF0000) | 0x189; // TODO: Verify correctness
					file->end_async_request();
					sl::rwspinlock_wunlock(file->m_locked);

					m_free_queue.enqueue(item);	
				}
				skipSemaWait = lastSkipSemaWait;
				continue;

				break;
			}
			case 5:
				assert(!"Unimplemented");
				break;
			case 6:
				assert(!"Unimplemented");
				break;
			default:
				assert(!"Unimplemented");
				break;
			}
		}

	}
	return 0;
}

uint32_t csl_file_async_request::stub_thread_routine(uint64_t arg)
{
	csl_file_async_request* obj = reinterpret_cast<csl_file_async_request*>(arg);
	return obj->thread_routine();
}
