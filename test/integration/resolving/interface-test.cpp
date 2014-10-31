#include <cassert>

#include <rod/Extend.hpp>
#include <rod/Rod.hpp>
#include <rod/Singleton.hpp>




bool called = false;


class Interface
{
public:
	virtual void	method() = 0;
};


class Component:
	public Interface
{
public:
	virtual
	void
	method()
	{
		called = true;
	}
};


void
test()
{
	rod::enter(
	[] ( rod::Root& root )
	{
		auto withComponent = rod::extend( root ).with< rod::Singleton< Component > >()();
		rod::resolve< Interface& >( withComponent ).method();
	});

	assert( called );
}