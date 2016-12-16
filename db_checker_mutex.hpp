
#ifndef DB_CHECKER_MUTEX_HPP_
#define DB_CHECKER_MUTEX_HPP_

#include <atomic>
#include <chrono>

#include <cassert>



namespace mutex_controlled
{
	
	//
	
	class ccount_lock_holder
	{
	public:
		thread_local static size_t m_count_lock;
	};
	
	class ccounted_mutexes_strategy
	{
	public:
		
		static void lock()
		{
			++ccount_lock_holder::m_count_lock;
		}
		
		static bool try_lock()
		{
			++ccount_lock_holder::m_count_lock;
			return true;
		}
		
		template <class Rep, class Period>
		static bool try_lock_for(const std::chrono::duration<Rep,Period> & rel_time)
		{
			++ccount_lock_holder::m_count_lock;
			return true;
		}
		
		template <class Clock, class Duration>
		static bool try_lock_until(const std::chrono::time_point<Clock,Duration> & abs_time)
		{
			++ccount_lock_holder::m_count_lock;
			return true;
		}
		
		static void unlock() noexcept
		{
			--ccount_lock_holder::m_count_lock;
		}
		
		static size_t get_count() noexcept
		{
			return ccount_lock_holder::m_count_lock;
		}
		
		static void check_with_assert()
		{
			if (get_count() > 0) assert( 1 != 1 );
		}
	};
	
	//
	
}

#endif // DB_CHECKER_MUTEX_HPP_
