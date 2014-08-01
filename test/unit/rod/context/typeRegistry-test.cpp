#include "rod/Context.hpp"


#include <type_traits>




struct Type1
{};


struct Type2
{};



void
test()
{
	using ctx1 = rod::CreateInitialContext< Type1 >::r;
	using ctx2 = ctx1::CreateChildContext< Type2 >::r;

	static_assert( std::is_same<
						ctx2::GetTypeRegistry::r,
						rod::TypeRegistry< Type1, Type2 >
				   >::value,
				   "Unexpected FindAnnotated result" );
}