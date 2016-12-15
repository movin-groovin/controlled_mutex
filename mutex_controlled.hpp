
#ifndef MUTEX_CONTROLLED_HPP_
#define MUTEX_CONTROLLED_HPP_

#include "decorators.hpp"
#include "db_checker_mutex.hpp"
#include "hierarchy_mutex.hpp"



namespace mutex_controlled
{
	
	//
	
#ifndef NDEBUG
	template <typename MT>
	using counted_check_mytex_type = cmutex_decor_compiletime< MT, ccounted_mutexes_strategy >;
	
	void check_with_assert();
	void check_with_exception ();
#else
	template <typename MT>
	using counted_check_mytex_type = MT;
	
	void check_with_assert();
	void check_with_exception ();
#endif
	
	//
	
	// for tests
	struct check_test
	{
		static void lock () noexcept {}
		static void try_lock() noexcept {}
		static void unlock() noexcept {}
	};
	
#ifndef NDEBUG
	template <typename MT>
	using test_mytex_type = cmutex_decor_compiletime< MT, check_test >;
#else
	template <typename MT>
	using test_mytex_type = MT;
#endif
	
	//
	
	template <typename T1, typename T2, bool selection>
	struct if_then_else;
	
	template <typename T1, typename T2>
	struct if_then_else<T1, T2, true>
	{
		using type = T1;
	};
	
	template <typename T1, typename T2>
	struct if_then_else<T1, T2, false>
	{
		using type = T2;
	};
	
	//
	
	template <typename MT, bool disable = false>
	using mutex_type = typename if_then_else <
		MT,
		counted_check_mytex_type< test_mytex_type< MT> >,
		disable
	>::type;
	
	//
	
}

#endif // MUTEX_CONTROLLED_HPP_




