#pragma once

#include <cassert>
#include <algorithm>

// Types that are (hopefully) identical across all games

struct spinlock_t
{
	volatile unsigned int m_lock_status = 0;
};

struct rwspinlock_t
{
	volatile unsigned int m_lock_status = 0;
};

struct recursive_rwspinlock_t
{
	union lock_status_t
	{
		volatile unsigned int status;
		struct
		{
			uint32_t ref_count : 16;
			uint32_t thread_sid : 16;
		} s;
	};

	volatile lock_status_t m_lock_status {};
};

namespace sl {

struct alignas(16) mutex_t
{
	CRITICAL_SECTION m_cs;

	mutex_t();
	~mutex_t();

	void lock();
	void unlock();
};

void mutex_construct(mutex_t& mutex);
void mutex_destruct(mutex_t& mutex);

void spinlock_lock(spinlock_t& spinlock);
void spinlock_unlock(spinlock_t& spinlock);

void rwspinlock_wlock(rwspinlock_t& spinlock);
void rwspinlock_wunlock(rwspinlock_t& spinlock);
void rwspinlock_rlock(rwspinlock_t& spinlock);
void rwspinlock_runlock(rwspinlock_t& spinlock);

enum FILE_SEEK
{
	FILE_SEEK_SET = 0x0,
	FILE_SEEK_CURRENT = 0x1,
	FILE_SEEK_FORCE_U32 = 0xFFFFFFFF,
};

enum FILE_ASYNC_METHOD
{
	FILE_ASYNC_METHOD_OPEN = 0x0,
	FILE_ASYNC_METHOD_CREATE = 0x1,
	FILE_ASYNC_METHOD_CLOSE = 0x2,
	FILE_ASYNC_METHOD_READ = 0x3,
	FILE_ASYNC_METHOD_WRITE = 0x4,
	FILE_ASYNC_METHOD_PRELOAD = 0x5,
	FILE_ASYNC_METHOD_NUM = 0x6,
	FILE_ASYNC_METHOD_INVALID = 0xFFFFFFFF,
};

enum ARCHIVE_FIND_RESULT
{
	ARCHIVE_FIND_FILE_FOUND = 0x0,
	ARCHIVE_FIND_DIRECTORY_FOUND = 0x1,
	ARCHIVE_FIND_NOT_FOUND = 0xFFFFFFFF,
};


struct handle_internal_t
{
	union
	{
		uint64_t m_qhandle;
		struct
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
		uint32_t m_handle = 0;
	} h;

	bool operator==(const handle_t& handle) const { return h.m_handle == handle.h.m_handle; } 
};
inline constexpr handle_t INVALID_HANDLE {};

struct alignas(8) file_handle_t
{
  char m_user_work[48];
  void *m_h_native;
  volatile uint64_t m_file_pointer;
  handle_t m_handle;
  char m_file_path[1040];
};
static_assert(sizeof(file_handle_t) == 1112);

}

template<typename T>
struct t_locked_queue_node
{
	t_locked_queue_node<T> *volatile mp_next = nullptr;
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

	T* dequeue()
	{
		T* item = nullptr;
		sl::spinlock_lock(m_sync);
		t_locked_queue_node<T>* head = this->mp_head;
		if (head != nullptr)
		{
			m_size--;
			this->mp_head = head->mp_next;
			if (this->mp_tail == head)
				this->mp_tail = nullptr;
			head->mp_next = nullptr;
			item = reinterpret_cast<T*>(reinterpret_cast<char*>(head) - offsetof(T, m_node));
		}
		sl::spinlock_unlock(m_sync);
		return item;
	}

private:
	t_locked_queue_node<T> *mp_head = nullptr;
	t_locked_queue_node<T> *mp_tail = nullptr;
	spinlock_t m_sync;
	uint32_t m_size = 0;
};

template<typename T>
class t_fixed_deque
{
public:
	void push_back(const T* p)
	{
		assert(m_element_size < m_deque_size);
		mp_element[m_index_end] = *p;
		m_element_size++;
		m_index_end = m_index_end != m_deque_size ? m_index_end + 1 : 0;
	}

	void reserve(unsigned int size)
	{
		assert(mp_element == nullptr);
		mp_element = new T[size + 1];
		m_deque_size = size;
	}

private:
	T *mp_element = nullptr;
	unsigned int m_deque_size = 0;
	unsigned int m_element_size = 0;
	unsigned int m_index_begin = 0;
	unsigned int m_index_end = 0;
};

template<typename T>
class t_pointer_list
{
public:
	void push_front(T* p_data)
	{
		T* top = mp_top;
		if (top != nullptr)
		{
			// TODO: Cases like this might be why a "linker" class is needed?
			// Right now, the sector cache needs to be public for this to work
			top->m_prev = static_cast<decltype(top->m_prev)>(p_data - top);
		}
		else
		{
			mp_bottom = p_data;
		}
		p_data->m_next = top != nullptr ? static_cast<decltype(p_data->m_next)>(top - p_data) : 0;
		p_data->m_prev = 0;
		mp_top = p_data;
	}

	void push_back(T* p_data)
	{
		if (mp_top == nullptr)
		{
			push_front(p_data);
			return;
		}
		mp_bottom->m_next = static_cast<decltype(mp_bottom->m_next)>(p_data - mp_bottom);
		p_data->m_prev = static_cast<decltype(p_data->m_prev)>(mp_bottom - p_data);
		p_data->m_next = 0;
		mp_bottom = p_data;
	}

private:
	T* mp_top = nullptr;
	T* mp_bottom = nullptr;
};

template<typename T>
struct t_status_ptr
{
	union
	{
		struct
		{
			int64_t m_ptr : 48;
			uint64_t m_status : 16;
		} p;
		uint64_t m_status_ptr;
	} v;
};

template<typename T>
struct t_lockfree_ptr
{
	struct counter_ptr_t
	{
		union
		{
			struct
			{
				int64_t m_ptr : 48;
				int64_t m_counter : 16;
			} p;
			uint64_t m_quad_ptr;
		} val;
	};

	counter_ptr_t m_counter_ptr;
};

template<typename T>
class t_lockfree_stack
{
public:
	void push(T* p)
	{
		auto desired = m_top.m_counter_ptr.val;
		decltype(desired) exchanged;
		p->mp_link = reinterpret_cast<T*>(desired.p.m_ptr);
		exchanged.p.m_ptr = reinterpret_cast<int64_t>(p);
		exchanged.p.m_counter = desired.p.m_counter + 1;
		while (desired.m_quad_ptr != InterlockedCompareExchange(&m_top.m_counter_ptr.val.m_quad_ptr, exchanged.m_quad_ptr,
														desired.m_quad_ptr))
		{
			_mm_pause();
			desired = m_top.m_counter_ptr.val;
			p->mp_link = reinterpret_cast<T*>(desired.p.m_ptr);
			exchanged.p.m_ptr = reinterpret_cast<int64_t>(p);
			exchanged.p.m_counter = desired.p.m_counter + 1;
		}
	}

	T* pop()
	{
		auto desired = m_top.m_counter_ptr.val;
		T* result = reinterpret_cast<T*>(desired.p.m_ptr);
		if (result == nullptr) return nullptr;

		decltype(desired) exchanged;
		exchanged.p.m_ptr = reinterpret_cast<int64_t>(result->mp_link);
		exchanged.p.m_counter = desired.p.m_counter + 1;
		do
		{
			if (desired.m_quad_ptr == InterlockedCompareExchange(&m_top.m_counter_ptr.val.m_quad_ptr, exchanged.m_quad_ptr,
														desired.m_quad_ptr))
			{
				break;
			}

			desired = m_top.m_counter_ptr.val;
			result = reinterpret_cast<T*>(desired.p.m_ptr);
			exchanged.p.m_ptr = reinterpret_cast<int64_t>(result->mp_link);
			exchanged.p.m_counter = desired.p.m_counter + 1;
		}
		while (result != nullptr);
		return result;
	}

private:
	t_lockfree_ptr<T> m_top;
};

template<typename T>
class t_instance_tbl
{
public:
	void initialize(char* space, unsigned int size)
	{
		m_instance_max = size;
		m_free_top = 0;
		m_free_tbl_size = (size + 63) / 64;
		if (space != nullptr)
		{
			m_status |= 1;
			mpp_instance_tbl = reinterpret_cast<T**>(space);
			mp_free_tbl = reinterpret_cast<uint64_t*>(space + sizeof(mpp_instance_tbl[0]) * size);
		}
		else
		{
			mpp_instance_tbl = new T*[size];
			mp_free_tbl = new uint64_t[size];
		}
		std::fill_n(mp_free_tbl, m_free_tbl_size, uint64_t(-1));
		std::fill_n(mpp_instance_tbl, m_instance_max, nullptr);
	}

	T* get(unsigned int id) const
	{
		assert(id >= 1);
		const unsigned int index = id - 1;
		return index < m_instance_max ? mpp_instance_tbl[index] : nullptr;
	}

private:
	T** mpp_instance_tbl;
	uint64_t* mp_free_tbl;
	unsigned int m_status;
	unsigned int m_instance_max;
	unsigned int m_free_top;
	unsigned int m_free_tbl_size;
};

template<typename T>
class t_avl_tree_node
{

private:
	t_status_ptr<t_avl_tree_node<T> > mp_left;
	t_status_ptr<t_avl_tree_node<T> > mp_right;
};

struct csl_hash
{
	union
	{
		struct
		{
			uint16_t m_hash_id;
			char m_sz_hash_name[30];
		} h;
		uint64_t m_hash[4];
	} hash {};
};