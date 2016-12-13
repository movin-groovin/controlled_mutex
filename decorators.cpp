
// g++ -std=c++14 decorators.cpp -o decorators

#include <iostream>
#include <string>
#include <mutex>
#include <memory>
#include <stdexcept>

#include <cassert>



namespace decor {
	
	template <typename T>
	struct cmutex_traits_compiletime
	{
		template <typename U = T>
		static void do_lock(U & obj)
		{
			obj.lock();
		}
		
		template <typename U = T>
		static bool do_try_lock(U & obj)
		{
			return obj.try_lock();
		}
		
		template <typename U = T>
		static bool try_lock_for(U & obj)
		{
			return false;
		}
		
		template <typename U = T>
		static bool try_lock_until(U & obj)
		{
			return false;
		}
		
		template <typename U = T>
		static void do_unlock(U & obj)
		{
			obj.unlock();
		}
	};
	
	
	
	template < typename T, typename Strategy, typename TypeTraits = cmutex_traits_compiletime<T> >
	class cdecor_mutex_compiletime_strategy
	{
	public:
		using type = T;
		using strategy_type = Strategy;
		using traits_type = TypeTraits;
		
	public:
		template <typename ... Args>
		cdecor_mutex_compiletime_strategy(Args ... args): m_internal(std::forward<Args>(args)...) {}
		
		template <typename U = T>
		void lock() {
			traits_type::do_lock(m_internal);
			Strategy::lock();
		}
		
		template <typename U = T>
		bool try_lock() {
			bool ret = traits_type::do_try_lock(m_internal);
			if (ret) Strategy::try_lock();
			return ret;
		}
		
		template <typename U = T>
		bool try_lock_for() {
			bool ret = traits_type::do_try_lock_for(m_internal);
			if (ret) Strategy::try_lock_for();
			return ret;
		}
		
		template <typename U = T>
		bool try_lock_until() {
			bool ret = traits_type::do_try_lock_until(m_internal);
			if (ret) Strategy::try_lock_until();
			return ret;
		}
		
		template <typename U = T>
		void unlock() {
			Strategy::unlock();
			traits_type::do_unlock(m_internal);
		}
		
	private:
		type m_internal;
	};
	
	// ================================================================================================
	
	class ccounted_mutexes_strategy
	{
	private:
		thread_local static size_t m_cnt_call;
		
	public:
		static void lock()
		{
			++m_cnt_call;
		}
		
		static void try_lock()
		{
			++m_cnt_call;
		}
		
		static void unlock()
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
	
	thread_local size_t ccounted_mutexes_strategy::m_cnt_call = size_t();
	
	// ================================================================================================
	
	template <typename T>
	struct cmutex_traits_runtime
	{
		template <typename U = T>
		static void do_lock(U & obj)
		{
			obj -> lock();
		}
		
		template <typename U = T>
		static bool do_try_lock(U & obj)
		{
			return obj -> try_lock();
		}
		
		template <typename U = T>
		static bool try_lock_for(U & obj)
		{
			return false;
		}
		
		template <typename U = T>
		static bool try_lock_until(U & obj)
		{
			return false;
		}
		
		template <typename U = T>
		static void do_unlock(U & obj)
		{
			obj -> unlock();
		}
	};
	
	
	
	template < typename T, typename Strategy, typename TypeTraits = cmutex_traits_runtime<T> >
	class cdecor_mutex_runtime_strategy
	{
	public:
		using type = T;
		using ptr_type = std::unique_ptr<type>;
		using strategy_type = Strategy;
		using ptr_strategy_type = std::unique_ptr<strategy_type>;
		using traits_type = TypeTraits;
		
	public:
		cdecor_mutex_runtime_strategy(
			std::unique_ptr<T> && ptr_t,
			std::unique_ptr<Strategy> && ptr_s
		):
			m_internal(std::move(ptr_t)),
			m_strategy(std::move(ptr_s))
		{}
		
		template <typename U = T>
		void lock() {
			traits_type::do_lock(m_internal);
			m_strategy -> lock();
		}
		
		template <typename U = T>
		bool try_lock() {
			bool ret = traits_type::do_try_lock(m_internal);
			if (ret) m_strategy -> try_lock();
			return ret;
		}
		
		template <typename U = T>
		bool try_lock_for() {
			bool ret = traits_type::do_try_lock_for(m_internal);
			if (ret) m_strategy -> try_lock_for();
			return ret;
		}
		
		template <typename U = T>
		bool try_lock_until() {
			bool ret = traits_type::do_try_lock_until(m_internal);
			if (ret) m_strategy -> try_lock_until();
			return ret;
		}
		
		template <typename U = T>
		void unlock() {
			m_strategy -> unlock();
			traits_type::do_unlock(m_internal);
		}
		
	private:
		ptr_type m_internal;
		ptr_strategy_type m_strategy;
	};
	
}


struct one1
{
	static void lock()
	{
		std::cout << "111\n";
	}
	
	static void unlock()
	{
		std::cout << "~111\n";
	}
};

struct one2
{
	static void lock()
	{
		std::cout << "222\n";
	}
	
	static void unlock()
	{
		std::cout << "~222\n";
	}
};

struct one3
{
	static void lock()
	{
		std::cout << "333\n";
	}
	
	static void unlock()
	{
		std::cout << "~333\n";
	}
};

struct one4
{
	static void lock()
	{
		std::cout << "444\n";
	}
	
	static void unlock()
	{
		std::cout << "~444\n";
	}
};

struct one5
{
	static void lock()
	{
		std::cout << "555\n";
	}
	
	static void unlock()
	{
		std::cout << "~555\n";
	}
};


int main()
{
	using namespace decor;
	using mutex_type = cdecor_mutex_compiletime_strategy<std::mutex, ccounted_mutexes_strategy>;
	mutex_type mut;
	
	std::cout << "cnt: " << ccounted_mutexes_strategy::get_count() << "\n";
	{
		std::unique_lock<mutex_type> lck1(mut);
		std::lock_guard<mutex_type> lck2(mut);
		std::cout << "cnt: " << ccounted_mutexes_strategy::get_count() << "\n";
	}
	std::cout << "cnt: " << ccounted_mutexes_strategy::get_count() << "\n";
	
	//
	
	{
		using mutex_type = cdecor_mutex_compiletime_strategy
		<
			cdecor_mutex_compiletime_strategy
			<
				cdecor_mutex_compiletime_strategy
				<
					std::mutex,
					one1
				>,
				one2
			>,
			one3
		>;
		
		mutex_type mut;
		
		std::lock_guard<mutex_type> lck(mut);
	}
	
	//
	
	{
		std::cout << "\n\n";
		
		using mutex_type3 = cdecor_mutex_runtime_strategy<std::mutex, one1>;
		using mutex_type2 = cdecor_mutex_runtime_strategy<mutex_type3, one2>;
		using mutex_type1 = cdecor_mutex_runtime_strategy<mutex_type2, one3>;
		
		auto mut3( std::make_unique<mutex_type3>( std::make_unique<std::mutex>(), std::make_unique<one1>() ) );
		auto mut2( std::make_unique<mutex_type2>( std::move(mut3), std::make_unique<one2>() ) );
		mutex_type1 mut1( std::move(mut2), std::make_unique<one3>() );
		
		std::lock_guard<decltype(mut1)> lck(mut1);
	}
	
	//
	
	{
		std::cout << "\n\n";
		
		using mutex_type3 = cdecor_mutex_runtime_strategy<std::mutex, one1>;
		using mutex_type2 = cdecor_mutex_runtime_strategy<mutex_type3, one2>;
		using mutex_type1 = cdecor_mutex_runtime_strategy<mutex_type2, one3>;
		
		using mutex_type = cdecor_mutex_compiletime_strategy <
			cdecor_mutex_compiletime_strategy <
				mutex_type1,
				one4
			>,
			one5
		>;
		
		auto mut3( std::make_unique<mutex_type3>( std::make_unique<std::mutex>(), std::make_unique<one1>() ) );
		auto mut2( std::make_unique<mutex_type2>( std::move(mut3), std::make_unique<one2>() ) );
		mutex_type mut( std::move(mut2), std::make_unique<one3>() );
		
		std::lock_guard<mutex_type> lck(mut);
	}
	
	
	return 0;
}












