
// g++ -std=c++14 db_invoke.cpp -o db_invoke -lboost_system

#include <iostream>
#include <string>
#include <chrono>
#include <list>
#include <vector>
#include <memory>
#include <mutex>



struct DatabaseCheckStrategy
{
	thread_local static size_t m_lock_count;
	
	static void do_actions()
	{
		++m_lock_count;
	}
	
	static void undo_actions()
	{
		--m_lock_count;
	}
};

thread_local size_t DatabaseCheckStrategy::m_lock_count = size_t();



class CInterface {
public:
	virtual void lock() = 0;
	virtual void unlock() = 0;
	virtual ~ CInterface () {}
};



template <typename T, typename ActionsStrategy, typename InterfaceT = CInterface, bool store_by_ptr = false>
class CDecorMutex: public InterfaceT {
private:
	T m_val;
	
public:
	template <typename ... Args> 
	CDecorMutex(Args ... args): m_val( std::forward<Args>(args)... ) {}
	
	virtual void lock()
	{
		ActionsStrategy::do_actions();
		m_val.lock();
	}
	
	virtual void unlock()
	{
		ActionsStrategy::undo_actions();
		m_val.unlock();
	}
};

template <typename T, typename ActionsStrategy, typename InterfaceT>
class CDecorMutex<T, ActionsStrategy, InterfaceT, true>: public InterfaceT {
private:
	std::unique_ptr<T> m_val;
	
public:
	template <typename ... Args> 
	CDecorMutex(Args ... args): m_val( std::make_unique<T>( std::forward<Args>(args)... ) ) {}
	
	virtual void lock()
	{
		ActionsStrategy::do_actions();
		m_val->lock();
	}
	
	virtual void unlock()
	{
		ActionsStrategy::undo_actions();
		m_val->unlock();
	}
};



#ifdef DEBUG_MUTEX

template <typename T, typename ActionsStrategy = DatabaseCheckStrategy, bool store_by_ptr = false>
using mutex_type = CDecorMutex<T, ActionsStrategy, store_by_ptr>;

#else

template <typename T>
using mutex_type = T;

#endif



namespace actions {
	
	struct one1
	{
		static void do_actions()
		{
			std::cout << "111\n";
		}
		
		static void undo_actions()
		{
			std::cout << "~111\n";
		}
	};
	
	struct one2
	{
		static void do_actions()
		{
			std::cout << "222\n";
		}
		
		static void undo_actions()
		{
			std::cout << "~222\n";
		}
	};
	
	struct one3
	{
		static void do_actions()
		{
			std::cout << "333\n";
		}
		
		static void undo_actions()
		{
			std::cout << "~333\n";
		}
	};
	
}



void ExecuteDb ()
{
	if (DatabaseCheckStrategy::m_lock_count > 0) throw std::exception();
	std::cout << "void ExecuteDb ()\n";
}



int main () {
	{
		mutex_type<std::mutex> obj1;
		//CDecorMutex<std::mutex, true> obj2;
		//std::unique_ptr<CInterface> ptr1( std::make_unique< CDecorMutex<std::mutex> >() );
		//std::unique_ptr<CInterface> ptr2( std::make_unique< CDecorMutex<std::mutex> >() );
		
		
		std::lock_guard< mutex_type<std::mutex> > lck1(obj1);
		ExecuteDb();
	}
	
	{
		using mutex_type = CDecorMutex<
			CDecorMutex<
				CDecorMutex< std::mutex, actions::one3, CInterface, false >,
				actions::one2,
				CInterface,
				false
			>,
			actions::one1,
			CInterface,
			false
		>;
		
		mutex_type mut1;
		
		std::lock_guard<mutex_type> lck(mut1);
		
		std::cout << "Under lock\n";
	}
	
	
	return 0;
}












