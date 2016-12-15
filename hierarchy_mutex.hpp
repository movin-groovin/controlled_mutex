
#ifndef HIERARCHY_MUTEX_HPP_
#define HIERARCHY_MUTEX_HPP_

#include <atomic>
#include <stdexcept>
#include <chrono>

#include <cassert>



namespace mutex_controlled
{
	
	//
	
	struct cmajor_minor
	{
		static const size_t major = 0;
		static const size_t minor = 0;
	};
	
	
	class cthread_level_holder
	{
	public:
		static const size_t default_level = -1;
		
		thread_local static size_t m_thread_current_level;
	};
	
	
	template <typename NumberTraits = cmajor_minor>
	class chierarchy_mutexes_strategy
	{
	private:
		size_t m_thread_prev_level;
		
	public:
		void check_level_violation_assert()
		{
			if (cthread_level_holder::m_thread_current_level < NumberTraits::major)
			{
				assert( 1 != 1);
			}
		}
		
		void set_level()
		{
			m_thread_prev_level = cthread_level_holder::m_thread_current_level;
			cthread_level_holder::m_thread_current_level = NumberTraits::major;
		}
		
		void lock()
		{
			check_level_violation_assert();
			set_level();
		}
		
		bool try_lock()
		{
			check_level_violation_assert();
			set_level();
			return true;
		}
		
		template <class Rep, class Period>
		bool try_lock_for(const std::chrono::duration<Rep,Period> & rel_time)
		{
			check_level_violation_assert();
			set_level();
			return true;
		}
		
		template <class Clock, class Duration>
		bool try_lock_until(const std::chrono::time_point<Clock,Duration> & abs_time)
		{
			check_level_violation_assert();
			set_level();
			return true;
		}
		
		void unlock() noexcept
		{
			// restore thread level
			cthread_level_holder::m_thread_current_level = m_thread_prev_level;
			m_thread_prev_level = 0;
		}
	};
	
	//
	
}

#endif // HIERARCHY_MUTEX_HPP_








