#pragma once

struct spinlock_t
{
  volatile unsigned int m_lock_status;
};