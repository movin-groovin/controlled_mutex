
#include "hierarchy_mutex.hpp"



namespace mutex_controlled
{
	
	//
	
	thread_local size_t cthread_level_holder::m_thread_current_level = cthread_level_holder::default_level;
	
	//
	
}
