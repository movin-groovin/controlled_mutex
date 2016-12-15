
#ifndef DECORATORS_HPP_
#define DECORATORS_HPP_

#include <memory>
#include <chrono>



namespace mutex_controlled
{
	//
	
	template <typename T>
	struct cmutex_traits_compiletime
	{
		static void do_lock(T & obj)
		{
			obj.lock();
		}
		
		static bool do_try_lock(T & obj)
		{
			return obj.try_lock();
		}
		
		template <class Rep, class Period>
		static bool try_lock_for(T & obj, const std::chrono::duration<Rep,Period> & rel_time)
		{
			return obj.try_lock_for(rel_time);
		}
		
		template <class Clock, class Duration>
		static bool try_lock_until(T & obj, const std::chrono::time_point<Clock,Duration> & abs_time)
		{
			return obj.try_lock_for(abs_time);
		}
		
		static void do_unlock(T & obj)
		{
			obj.unlock();
		}
	};
	
	//
	
	template < typename T, typename Strategy, typename TypeTraits = cmutex_traits_compiletime<T> >
	class cmutex_decor_compiletime
	{
	public:
		using type = T;
		using strategy_type = Strategy;
		using traits_type = TypeTraits;
		
	public:
		template <typename ... Args>
		cmutex_decor_compiletime(Args ... args): m_internal(std::forward<Args>(args)...) {}
		
		void lock() {
			traits_type::do_lock(m_internal);
			try {
				Strategy::lock();
			} catch (...) {
				traits_type::do_unlock(m_internal);
				throw;
			}
		}
		
		bool try_lock() {
			bool ret = traits_type::do_try_lock(m_internal);
			if (ret) {
				try{
					Strategy::try_lock();
				} catch (...) {
					traits_type::do_unlock(m_internal);
					throw;
				}
			}
			return ret;
		}
		
		template <class Rep, class Period>
		bool try_lock_for(const std::chrono::duration<Rep,Period> & rel_time) {
			bool ret = traits_type::do_try_lock_for(m_internal, rel_time);
			if (ret) {
				try {
					Strategy::try_lock_for(rel_time);
				} catch (...) {
					traits_type::do_unlock(m_internal);
					throw;
				}
			}
			return ret;
		}
		
		template <class Clock, class Duration>
		bool try_lock_until(const std::chrono::time_point<Clock,Duration> & abs_time) {
			bool ret = traits_type::do_try_lock_until(m_internal, abs_time);
			if (ret) {
				try {
					Strategy::try_lock_until(abs_time);
				} catch (...) {
					traits_type::do_unlock(m_internal);
					throw;
				}
			}
			return ret;
		}
		
		void unlock() {
			try{
				traits_type::do_unlock(m_internal);
				Strategy::unlock();
			} catch(...) {
				Strategy::unlock();
				throw;
			}
			return;
		}
		
	private:
		type m_internal;
	};
	
	//
	
	template <typename T>
	struct cmutex_traits_runtime
	{
		static void do_lock(std::unique_ptr<T> & obj)
		{
			obj -> lock();
		}
		
		static bool do_try_lock(std::unique_ptr<T> & obj)
		{
			return obj -> try_lock();
		}
		
		template <class Rep, class Period>
		static bool try_lock_for(std::unique_ptr<T> & obj, const std::chrono::duration<Rep,Period> & rel_time)
		{
			return obj -> try_lock_for(rel_time);
		}
		
		template <class Clock, class Duration>
		static bool try_lock_until(std::unique_ptr<T> & obj, const std::chrono::time_point<Clock,Duration> & abs_time)
		{
			return obj -> try_lock_until(abs_time);
		}
		
		static void do_unlock(std::unique_ptr<T> & obj)
		{
			obj -> unlock();
		}
	};
	
	//
	
	template < typename T, typename Strategy, typename TypeTraits = cmutex_traits_runtime<T> >
	class cmutex_decor_runtime
	{
	public:
		using type = T;
		using ptr_type = std::unique_ptr<type>;
		using strategy_type = Strategy;
		using ptr_strategy_type = std::unique_ptr<strategy_type>;
		using traits_type = TypeTraits;
		
	public:
		cmutex_decor_runtime(
			std::unique_ptr<T> && ptr_t,
			std::unique_ptr<Strategy> && ptr_s
		):
			m_internal(std::move(ptr_t)),
			m_strategy(std::move(ptr_s))
		{}
		
		void lock() {
			traits_type::do_lock(m_internal);
			try {
				m_strategy -> lock();
			} catch (...) {
				traits_type::do_unlock(m_internal);
				throw;
			}
			return;
		}
		
		bool try_lock() {
			bool ret = traits_type::do_try_lock(m_internal);
			if (ret) {
				try {
					m_strategy -> try_lock();
				} catch (...) {
					traits_type::do_unlock(m_internal);
					throw;
				}
			}
			return ret;
		}
		
		template <class Rep, class Period>
		bool try_lock_for(const std::chrono::duration<Rep,Period> & rel_time) {
			bool ret = traits_type::do_try_lock_for(m_internal, rel_time);
			if (ret) {
				try {
					m_strategy -> try_lock_for(rel_time);
				} catch (...) {
					traits_type::do_unlock(m_internal);
					throw;
				}
			}
			return ret;
		}
		
		template <class Clock, class Duration>
		bool try_lock_until(const std::chrono::time_point<Clock,Duration> & abs_time) {
			bool ret = traits_type::do_try_lock_until(m_internal, abs_time);
			if (ret) {
				try {
					m_strategy -> try_lock_until(abs_time);
				} catch (...) {
					traits_type::do_unlock(m_internal);
					throw;
				}
			}
			return ret;
		}
		
		void unlock() {
			try {
				traits_type::do_unlock(m_internal);
				m_strategy -> unlock();
			} catch (...) {
				m_strategy -> unlock();
				throw;
			}
			return;
		}
		
	private:
		ptr_type m_internal;
		ptr_strategy_type m_strategy;
	};
	
	//
	
}

#endif // DECORATORS_HPP_
