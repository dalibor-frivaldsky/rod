#include <cassert>

#include <rod/Rod.hpp>




void
test()
{
	bool called = false;

	rod::enter(
	[&] ( rod::Root& root )
	{
		called = true;
	});

	assert( called );
}