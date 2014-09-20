#include "rod/ContextLevel.hpp"


#include <type_traits>

#include "rod/Singleton.hpp"
#include "rod/TypeRegistry.hpp"
#include "rod/Container.hpp"




class Type
{};

class Component
{};


void
test()
{
	static_assert( std::is_same<
						rod::CreateContextLevel<>::r,
						rod::ContextLevel<
							rod::CreateTypeRegistry<>::r,
							rod::CreateContainer<>::r
						>
				   >::value,
				   "Unexpected ContextLevel structure" );

	static_assert( std::is_same<
						rod::CreateContextLevel<
							Type
						>::r,
						rod::ContextLevel<
							rod::CreateTypeRegistry< Type >::r,
							rod::CreateContainer<>::r
						>
				   >::value,
				   "Unexpected ContextLevel structure" );

	static_assert( std::is_same<
						rod::CreateContextLevel<
							Type,
							rod::Singleton< Component >
						>::r,
						rod::ContextLevel<
							rod::CreateTypeRegistry< Type, Component >::r,
							rod::CreateContainer< rod::Singleton< Component > >::r
						>
				   >::value,
				   "Unexpected ContextLevel structure" );
}