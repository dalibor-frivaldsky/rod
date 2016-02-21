#include <cassert>

#include <rod/dispatch/Linear.hpp>




int	value = 0;


struct Controller1
{
	enum { id = 1 };

	void
	method()
	{
		value = 10;
	}
};


struct Controller2
{
	enum { id = 2 };

	void
	method()
	{
		value = 20;
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
	perform()
	{
		Branch().method();
	}
};


void
test()
{
	using linear = rod::dispatch::Linear<
						rod::TypeList<
							Controller1,
							Controller2 >,
						Handle,
						Performer >;
	linear	l;
	
	l.dispatch( 1 );
	assert( value == 10 );

	l.dispatch( 2 );
	assert( value == 20 );
}