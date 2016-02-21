#include "rod/TypeRegistry.hpp"


#include <type_traits>




class Type
{};

class Component
{};


template< typename T >
struct TypeSelector;

template<>
struct TypeSelector< Type >
{
	enum { r = true };
};

template< typename T >
struct TypeSelector
{
	enum { r = false };
};




void
test()
{
	using reg = rod::CreateTypeRegistry< Type, Component >::r;

	static_assert( std::is_same<
						reg::Find< TypeSelector >::r,
						rod::TypeList< Type >
				   >::value,
				   "Unexpected Find result" );
}