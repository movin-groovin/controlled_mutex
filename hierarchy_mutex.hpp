
#ifndef HIERARCHY_MUTEX_HPP_
#define HIERARCHY_MUTEX_HPP_

#include <atomic>
#include <chrono>

#include <cassert>



namespace mutex_controlled
{
	
	//
	
	class clevels_holder
	{
	public:
		using counter_type = size_t;
		
		static const counter_type default_value = -1;
		
		thread_local static counter_type m_thread_level_major;
		thread_local static counter_type m_thread_level_minor;
	};
	
	template <typename MT, size_t mutex_level_major, size_t mutex_level_minor>
	class chierarchy_mutex_internal
	{
	public:
		using mutex_type = MT;
		using counter_type = clevels_holder::counter_type;
		
	private:
		size_t m_thread_prev_level_major;
		size_t m_thread_prev_level_minor;
		
		mutex_type m_mutex;
		
	public:
		void check_level_violation()
		{
			if (clevels_holder::m_thread_level_major < mutex_level_major) {
				assert( 1 != 1 );
			}
			if (clevels_holder::m_thread_level_minor < mutex_level_minor) {
				assert( 2 != 2 );
			}
		}
		
		void set_level()
		{
			m_thread_prev_level_major = clevels_holder::m_thread_level_major;
			m_thread_prev_level_minor = clevels_holder::m_thread_level_minor;
			clevels_holder::m_thread_level_major = mutex_level_major;
			clevels_holder::m_thread_level_minor = mutex_level_minor;
		}
		
		void restore_level()
		{
			clevels_holder::m_thread_level_major = m_thread_prev_level_major;
			clevels_holder::m_thread_level_minor = m_thread_prev_level_minor;
		}
		
		void lock()
		{
			check_level_violation();
			m_mutex.lock();
			set_level();
		}
		
		bool try_lock()
		{
			bool ret;
			
			check_level_violation();
			if ( !(ret = m_mutex.try_lock()) ) return ret;
			set_level();
			
			return ret;
		}
		
		template <class Rep, class Period>
		bool try_lock_for(const std::chrono::duration<Rep,Period> & rel_time)
		{
			bool ret;
			
			check_level_violation();
			if ( !(ret = m_mutex.try_lock(rel_time)) ) return ret;
			set_level();
			
			return ret;
		}
		
		template <class Clock, class Duration>
		bool try_lock_until(const std::chrono::time_point<Clock,Duration> & abs_time)
		{
			bool ret;
			
			check_level_violation();
			if ( !(ret = m_mutex.try_lock(abs_time)) ) return ret;
			set_level();
			
			return ret;
		}
		
		void unlock()
		{
			restore_level();
			m_mutex.unlock();
		}
	};
	
	//
	
}

#endif // HIERARCHY_MUTEX_HPP_








