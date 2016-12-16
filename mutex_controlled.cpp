
#include "mutex_controlled.hpp"



namespace mutex_controlled
{
	
	//
	
#ifndef NDEBUG
	void check_with_assert() { ccounted_mutexes_strategy::check_with_assert(); }
#else
	void check_with_assert() { ; }
#endif

	//
	
}




