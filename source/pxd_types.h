#pragma once

// Types that are (hopefully) identical across all games

struct spinlock_t
{
  volatile unsigned int m_lock_status;
};

namespace sl {

void spinlock_lock(spinlock_t& spinlock);
void spinlock_unlock(spinlock_t& spinlock);

enum FILE_SEEK
{
	FILE_SEEK_SET = 0x0,
	FILE_SEEK_CURRENT = 0x1,
	FILE_SEEK_FORCE_U32 = 0xFFFFFFFF,
};

struct handle_internal_t
{
	union
	{
		uint64_t m_qhandle;
		union
		{
			uint64_t m_serial : 12;
			uint64_t m_ptr : 44;
			uint64_t m_type : 8;
		} data;
	} intern;
};

struct handle_t
{
	union
	{
		struct
		{
			uint32_t m_bank : 20;
			uint32_t m_serial : 12;
		} data;
		uint32_t m_handle;
	} h;
};

}

template<typename T>
struct t_locked_queue_node
{
	t_locked_queue_node<T> *volatile mp_next;
};

template<typename T>
class alignas(16) t_locked_queue
{
public:
	void enqueue(T* p)
	{
		sl::spinlock_lock(m_sync);

		m_size++;
		p->m_node.mp_next = nullptr;
		if (this->mp_head != nullptr)
			mp_tail->mp_next = &p->m_node;
		else
			this->mp_head = &p->m_node;
		this->mp_tail = &p->m_node;

		sl::spinlock_unlock(m_sync);
	}

private:
	t_locked_queue_node<T> *mp_head;
	t_locked_queue_node<T> *mp_tail;
	spinlock_t m_sync;
	uint32_t m_size;
};