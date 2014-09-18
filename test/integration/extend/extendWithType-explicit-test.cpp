#include <type_traits>

#include <rod/Extend.hpp>
#include <rod/Find.hpp>
#include <rod/Rod.hpp>




struct Type
{};


void
test()
{
	rod::enter(
	[] ( rod::Root& root )
	{
		rod::Extend< rod::Root >
			::With< Type >::r		context( root );

		static_assert( std::is_same<
							rod::Find<
								decltype( context ),
								rod::IsType< Type > >::r,
							rod::TypeList< Type > >::value,
					   "Type not found in context" );
	});
}