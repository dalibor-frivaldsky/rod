#include "rod/TypeRegistry.hpp"


#include <type_traits>




class Type
{};

class Component
{};




void
test()
{
	static_assert( std::is_same<
						rod::CreateTypeRegistry< Type, Component >::r,
						rod::TypeRegistry< Type, Component >
				   >::value,
				   "Unexpected TypeRegistry structure" );
}