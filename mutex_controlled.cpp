
#include "mutex_controlled.hpp"



namespace mutex_controlled
{
	
	//
	
#ifndef NDEBUG
	void check_with_assert() { ccounted_mutexes_strategy<>::check_with_assert(); }
	void check_with_exception () { ccounted_mutexes_strategy<>::check_with_exception(); }
#else
	void check_with_assert() { ; }
	void check_with_exception () { ; }
#endif

	//
	
}




