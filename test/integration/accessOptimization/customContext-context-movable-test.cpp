#include <cassert>

#include <rod/Extend.hpp>
#include <rod/Rod.hpp>
#include <rod/Singleton.hpp>
#include <rod/annotation/ExtendWith.hpp>
#include <rod/annotation/Requires.hpp>




bool called = false;


struct BigComponent
{
	char	data[ 10000 ];
};


template< typename Context >
class UnmovableContext
{
public:
	using ExtendWith = rod::annotation::ExtendWith< rod::Singleton< BigComponent > >;

	using Requires = rod::annotation::Requires< BigComponent& >;

	UnmovableContext( Context&, BigComponent& )
	{}
};

template< typename Context >
class MovableContext
{
private:
	Context&		context;
	BigComponent&	bigComponent;


public:
	using Requires = rod::annotation::Requires< BigComponent& >;

	MovableContext( Context& context, BigComponent& bigComponent ):
	  context( context ),
	  bigComponent( bigComponent )
	{}

	MovableContext( MovableContext< Context >&& other, Context& context, BigComponent& bigComponent ):
	  context( context ),
	  bigComponent( bigComponent )
	{
		called = true;
		assert( &this->context != &other.context );
		assert( &this->bigComponent != &other.bigComponent );
	}
};


void
test()
{
	rod::enter(
	[] ( rod::Root& root )
	{
		auto unmovableContext = rod::extend( root ).as< UnmovableContext >()();
		static_assert( sizeof( unmovableContext ) < 10000, "Unmovable context contains value component" );

		auto movableContext = rod::extend( root )
									.with< rod::Singleton< BigComponent > >()
									.as< MovableContext >()();
		static_assert( sizeof( movableContext ) >= 10000, "Movable context does not contain value component" );
		auto movableContext2 = std::move( movableContext );
		assert( called );
	});
}