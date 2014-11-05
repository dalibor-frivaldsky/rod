#include <cassert>
#include <utility>

#include <rod/Each.hpp>
#include <rod/Extend.hpp>
#include <rod/Evaluable.hpp>
#include <rod/Rod.hpp>
#include <rod/Singleton.hpp>
#include <rod/With.hpp>
#include <rod/annotation/Requires.hpp>
#include <rod/evaluable/EvaluableResolver.hpp>




int value = 0;


struct Interface
{
	virtual void	method() = 0;
};

struct Implementer1:
	public Interface
{
	virtual
	void
	method()
	{
		value += 1;
	}
};

struct Implementer2:
	public Interface
{
	virtual
	void
	method()
	{
		value += 2;
	}
};

struct CallerService
{
	struct CallMethods
	{
		template< typename Context >
		void
		operator () ( Context& context )
		{
			rod::each< Interface >( context,
			[&] ( Interface& i )
			{
				i.method();
			});
		}
	};

	rod::Evaluable< CallMethods >	evaluate;


	using Requires = rod::annotation::Requires< rod::Evaluable< CallMethods > >;

	CallerService( rod::Evaluable< CallMethods > evaluate ):
	  evaluate( std::move( evaluate ) )
	{}


	void
	performCalls()
	{
		evaluate( CallMethods() );
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
									rod::evaluable::EvaluableResolver,
									rod::Singleton< Implementer1 >,
									rod::Singleton< Implementer2 >,
									rod::Singleton< CallerService > >()();

		rod::with( extended,
		[&] ( CallerService& callerService )
		{
			callerService.performCalls();
		});
	});

	assert( value == 3 );
}