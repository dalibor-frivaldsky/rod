#include <cassert>

#include <rod/Annotation.hpp>
#include <rod/Dispatcher.hpp>
#include <rod/Extend.hpp>
#include <rod/Rod.hpp>
#include <rod/match/Annotation.hpp>




int	value = 0;


namespace annotation
{

	struct Annotation
	{};

	ROD_Annotation( Annotation )
	
}


struct Controller1
{
	using Annotation = annotation::Annotation;

	enum { id = 1 };

	void
	method( int multiplier )
	{
		value = 10 * multiplier;
	}
};


struct Controller2
{
	using Annotation = annotation::Annotation;

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
	template< typename Branch, typename Context >
	static
	void
	perform( Context&, int multiplier )
	{
		Branch().method( multiplier );
	}
};


void
test()
{
	rod::enter(
	[] ( rod::Root& root )
	{
		auto	withControllers = rod::extend( root ).with< Controller1, Controller2 >()();
		auto	dispatcher = rod::dispatcher<
								rod::match::Annotation< annotation::IsAnnotation >,
								Handle,
								Performer >( withControllers );

		dispatcher.dispatch( 1, 7 );
		assert( value == 70 );
		
		dispatcher.dispatch( 2, 3 );
		assert( value == 60 );
	});
}