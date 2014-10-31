#include <cassert>

#include <rod/Each.hpp>
#include <rod/Extend.hpp>
#include <rod/Rod.hpp>
#include <rod/Singleton.hpp>




int value = 0;


struct Interface
{
	virtual void	method() = 0;
};

struct Implementor1:
	public Interface
{
	virtual
	void
	method()
	{
		value += 1;
	}
};

struct Implementor2:
	public Interface
{
	virtual
	void
	method()
	{
		value += 2;
	}
};


void
test()
{
	rod::enter(
	[] ( rod::Root& root )
	{
		auto	extended = rod::extend( root )
								.with<
									rod::Singleton< Implementor1 >,
									rod::Singleton< Implementor2 > >()();

		rod::each< Interface >( extended,
		[] ( Interface& i )
		{
			i.method();
		});
	});

	assert( value == 3 );
}