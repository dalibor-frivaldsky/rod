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
	using level = rod::CreateContextLevel< Type >::r;

	static_assert( std::is_same<
						level::Enrich< rod::Singleton< Component > >::r,
						rod::ContextLevel<
							rod::CreateTypeRegistry< Type, rod::Singleton< Component > >::r,
							rod::CreateContainer< rod::Singleton< Component > >::r
						>
				   >::value,
				   "Unexpected ContextLevel structure" );
}