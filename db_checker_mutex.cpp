
#include "db_checker_mutex.hpp"



namespace mutex_controlled
{
	
	//
	
	thread_local size_t ccount_lock_holder::m_count_lock = 0;
	
	//
	
}




