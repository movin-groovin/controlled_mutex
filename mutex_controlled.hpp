
#ifndef MUTEX_CONTROLLED_HPP_
#define MUTEX_CONTROLLED_HPP_

#include "decorators.hpp"
#include "db_checker_mutex.hpp"
#include "hierarchy_mutex.hpp"



namespace mutex_controlled
{
	//
	// useful functionalilty
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
	// counting of mutex locks
#ifndef NDEBUG
	template <typename MT>
	using counted_check_mytex_type = cmutex_decor_compiletime< MT, ccounted_mutexes_strategy >;
	
	void check_with_assert();
#else
	template <typename MT>
	using counted_check_mytex_type = MT;
	
	void check_with_assert();
#endif
	
	//
	// hierarchy mutex
#ifndef NDEBUG
	template <typename MT, clevels_holder::counter_type level_number>
	using chierarchy_mutex = chierarchy_mutex_internal<MT, level_number, clevels_holder::default_value>;
#else
	template <typename MT, clevels_holder::counter_type level_number>
	using chierarchy_mutex = MT;
#endif
	
	//
	// finish type selection
	template <
		typename MT,
		clevels_holder::counter_type level_number,
		bool disable = false
	>
	using mutex_type = typename if_then_else <
		MT,
		counted_check_mytex_type< chierarchy_mutex< MT, level_number > >,
		disable
	>::type;
	
	//
	
}

#endif // MUTEX_CONTROLLED_HPP_




