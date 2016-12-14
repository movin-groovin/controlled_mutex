
// g++ -std=c++14 db_checker_mutex.cpp mutex_controlled.cpp test.cpp test_test.cpp -o test
// g++ -std=c++14 db_checker_mutex.cpp mutex_controlled.cpp test.cpp test_test.cpp -o test -DNDEBUG

#include <iostream>
#include <string>
#include <mutex>

#include "mutex_controlled.hpp"



int main () {
	using mutex_type = mutex_controlled::mutex_type<std::mutex>;
	
	mutex_type mut1;
	
	mutex_controlled::check_with_assert();
	{
		std::cout << "Mark1\n";
		
		std::lock_guard<mutex_type> lck(mut1);
		mutex_controlled::check_with_assert();
		
		std::cout << "Mark2\n";
	}
	mutex_controlled::check_with_assert();
	
	
	return 0;
}










