#include <rod/Extend.hpp>
#include <rod/Rod.hpp>




template< typename Context >
class Context1
{
public:
	Context1( Context& )
	{}
};

template< typename Context >
class Context2
{
public:
	Context2( Context& )
	{}

	Context2( Context2< Context >&&, Context& )
	{}
};


void
test()
{
	rod::enter(
	[] ( rod::Root& root )
	{
		auto context1 = rod::extend( root ).as< Context1 >()();
		auto context2 = rod::extend( root ).as< Context2 >()();
	});
}