
#ifndef DB_CHECKER_MUTEX_HPP_
#define DB_CHECKER_MUTEX_HPP_

#include <atomic>
#include <stdexcept>

#include <cassert>



namespace mutex_controlled
{
	
	//
	
	class ccounted_mutexes_strategy
	{
	public:
		using counter_type = std::atomic<size_t>;
		
	private:
		thread_local static counter_type m_cnt_call;
		
	public:
		
		static void lock()
		{
			++m_cnt_call;
		}
		
		static void try_lock()
		{
			++m_cnt_call;
		}
		
		static void unlock() noexcept
		{
			--m_cnt_call;
		}
		
		static size_t get_count() noexcept
		{
			return m_cnt_call;
		}
		
		static void check_with_exception()
		{
			if (get_count() > 0)
				throw std::runtime_error("ccounted_mutexes_strategy: mutex lock counter more than 0");
		}
		
		static void check_with_assert()
		{
			if (get_count() > 0) assert( 1 != 1 );
		}
	};
	
	//
	
}

#endif // DB_CHECKER_MUTEX_HPP_
