
#include "hierarchy_mutex.hpp"



namespace mutex_controlled
{
	
	//
		
	thread_local clevels_holder::counter_type clevels_holder::m_thread_level_major = clevels_holder::default_value;
	thread_local clevels_holder::counter_type clevels_holder::m_thread_level_minor = clevels_holder::default_value;
	
	//
	
}
