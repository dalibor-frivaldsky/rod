#include <type_traits>

#include <rod/Extend.hpp>
#include <rod/Find.hpp>
#include <rod/Rod.hpp>
#include <rod/annotation/ExtendWith.hpp>




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
	}
};


void
test()
{
	rod::enter(
	[] ( rod::Root& root )
	{
		rod::Extend< rod::Root >
			::As< TestContext >::r		context( root );

		static_assert( std::is_same<
						rod::Find<
							decltype( context ),
							rod::IsType< Type > >::r,
						rod::TypeList< Type > >::value,
					   "Type not found in context" );
	});
}