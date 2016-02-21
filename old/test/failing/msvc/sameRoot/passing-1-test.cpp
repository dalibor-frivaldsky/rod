#include <rod/Extend.hpp>
#include <rod/Rod.hpp>




void
test()
{
	rod::enter(
	[] ( rod::Root& root )
	{
		auto context1 = rod::extend( root )();
		auto context2 = rod::extend( root )();
	});
}