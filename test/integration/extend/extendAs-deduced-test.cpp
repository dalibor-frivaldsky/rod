#include <cassert>
#include <type_traits>

#include <rod/Extend.hpp>
#include <rod/Find.hpp>
#include <rod/Rod.hpp>
#include <rod/annotation/ExtendWith.hpp>




bool constructed = false;


struct Type
{};


template< typename Context >
struct TestContext
{
private:
	Context&	context;


public:
	using ExtendWith = rod::annotation::ExtendWith< Type >;

	TestContext( Context& context ):
	  context( context )
	{
		static_assert( std::is_same<
							typename rod::Find<
								Context,
								rod::IsType< Type > >::r,
							rod::TypeList< Type > >::value,
					   "Type not found in context" );

		constructed = true;
	}
};


void
test()
{
	rod::enter(
	[] ( rod::Root& root )
	{
		auto context = rod::extend( root ).as< TestContext >()();

		static_assert( std::is_same<
						rod::Find<
							decltype( context ),
							rod::IsType< Type > >::r,
						rod::TypeList< Type > >::value,
					   "Type not found in context" );
	});

	assert( constructed );
}