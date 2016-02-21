#include <rod/Extend.hpp>
#include <rod/Rod.hpp>




void
test()
{
	rod::enter(
	[] ( rod::Root& root )
	{
		rod::Extend< rod::Root >::r		context( root );
	});
}