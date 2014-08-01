#include "rod/TypeRegistry.hpp"


#include <type_traits>




class Type
{};

class Component
{};




void
test()
{
	using reg1 = rod::CreateTypeRegistry< Type >::r;
	using reg2 = rod::CreateTypeRegistry< Component >::r;

	static_assert( std::is_same<
						reg1::Merge< reg2 >::r,
						rod::TypeRegistry< Type, Component >
				   >::value,
				   "Unexpected Merge result" );
}