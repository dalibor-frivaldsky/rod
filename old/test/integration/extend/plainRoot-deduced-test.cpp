#include <rod/Extend.hpp>
#include <rod/Rod.hpp>




void
test()
{
	rod::enter(
	[] ( rod::Root& root )
	{
		auto context = rod::extend( root )();
	});
	
}