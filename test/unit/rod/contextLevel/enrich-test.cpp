#include "rod/ContextLevel.hpp"


#include <type_traits>

#include "rod/AsSingleton.hpp"
#include "rod/TypeRegistry.hpp"
#include "rod/Container.hpp"




class Type
{};

class Component
{};


void
test()
{
	using level = rod::CreateContextLevel< Type >::r;

	static_assert( std::is_same<
						level::Enrich< rod::AsSingleton< Component > >::r,
						rod::ContextLevel<
							rod::CreateTypeRegistry< Type, Component >::r,
							rod::CreateContainer< rod::AsSingleton< Component > >::r
						>
				   >::value,
				   "Unexpected ContextLevel structure" );
}