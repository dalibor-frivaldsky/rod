#include <type_traits>

#include <rod/Extend.hpp>
#include <rod/Find.hpp>
#include <rod/Rod.hpp>
#include <rod/match/Type.hpp>




struct Type
{};


void
test()
{
	rod::enter(
	[] ( rod::Root& root )
	{
		auto context = rod::extend( root ).with< Type >()();

		static_assert( std::is_same<
							rod::Find<
								decltype( context ),
								rod::match::Type< Type > >::r,
							rod::TypeList< Type > >::value,
					   "Type not found in context" );
	});
}