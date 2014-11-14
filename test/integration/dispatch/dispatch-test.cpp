#include <cassert>

#include <rod/Dispatcher.hpp>




int	value = 0;


struct Controller1
{
	enum { id = 1 };

	void
	method( int multiplier )
	{
		value = 10 * multiplier;
	}
};


struct Controller2
{
	enum { id = 2 };

	void
	method( int multiplier )
	{
		value = 20 * multiplier;
	}
};


struct Handle
{
	template< typename Branch >
	static
	int
	handle()
	{
		return Branch::id;
	}
};


struct Performer
{
	template< typename Branch >
	static
	void
	perform( int multiplier )
	{
		Branch().method( multiplier );
	}
};


void
test()
{
	auto	dispatcher = rod::Dispatcher<
							rod::TypeList< Controller1, Controller2 >,
							Handle,
							Performer >();

	dispatcher.dispatch( 1, 7 );
	assert( value == 70 );
	
	dispatcher.dispatch( 2, 3 );
	assert( value == 60 );
}