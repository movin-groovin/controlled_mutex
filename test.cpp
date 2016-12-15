
// g++ -std=c++14 hierarchy_mutex.cpp db_checker_mutex.cpp mutex_controlled.cpp test.cpp test_test.cpp -o test
// g++ -std=c++14 hierarchy_mutex.cpp db_checker_mutex.cpp mutex_controlled.cpp test.cpp test_test.cpp -o test -DNDEBUG

#include <iostream>
#include <string>
#include <mutex>

#include "mutex_controlled.hpp"



struct cmajor_minor1
{
	static const size_t major = 100;
	static const size_t minor = 0;
};


struct cmajor_minor2
{
	static const size_t major = 1000;
	static const size_t minor = 0;
};



int main () {
	using mutex_type = mutex_controlled::mutex_type<std::mutex>;
	
	mutex_type mut1;
	
	mutex_controlled::check_with_assert();
	{
		std::cout << "Mark1\n";
		
		//std::lock_guard<mutex_type> lck(mut1);
		mutex_controlled::check_with_assert();
		
		std::cout << "Mark2\n";
	}
	mutex_controlled::check_with_assert();
	
	std::cout << "\n\n";
	
	{
		using mut_type1 = mutex_controlled::cmutex_decor_runtime< std::mutex, mutex_controlled::chierarchy_mutexes_strategy<cmajor_minor1> >;
		mut_type1 mut1(
			std::make_unique<std::mutex>(),
			std::make_unique< mutex_controlled::chierarchy_mutexes_strategy<cmajor_minor1> >()
		);
		
		using mut_type2 = mutex_controlled::cmutex_decor_runtime< std::mutex, mutex_controlled::chierarchy_mutexes_strategy<cmajor_minor2> >;
		mut_type2 mut2(
			std::make_unique<std::mutex>(),
			std::make_unique< mutex_controlled::chierarchy_mutexes_strategy<cmajor_minor2> >()
		);
		
		//std::lock_guard<mut_type1> lck1(mut1);
		std::lock_guard<mut_type2> lck2(mut2);
		std::lock_guard<mut_type1> lck1(mut1);
		
		std::cout << "Mark3\n";
	}
	
	
	return 0;
}










