#include <rod/Extend.hpp>
#include <rod/Rod.hpp>
#include <rod/Singleton.hpp>
#include <rod/annotation/ExtendWith.hpp>




struct Component
{};


template< typename Context >
class Context1
{
public:
	using ExtendWith = rod::annotation::ExtendWith< rod::Singleton< Component > >;

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
		auto context1 = rod::extend( root )
							.as< Context1 >()();
		auto context2 = rod::extend( root )
							.with< rod::Singleton< Component > >()
							.as< Context2 >()();
	});
}