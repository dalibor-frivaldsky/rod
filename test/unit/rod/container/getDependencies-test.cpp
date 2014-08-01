#include "rod/Container.hpp"


#include <type_traits>

#include "rod/AsSingleton.hpp"
#include "rod/TypeList.hpp"
#include "rod/annotation/ConstructWith.hpp"




struct C1
{};

struct C2
{};

struct C3
{};

struct D1
{
	using ConstructWith = rod::annotation::ConstructWith< C1 >;
};

struct D2
{
	using ConstructWith = rod::annotation::ConstructWith< C1, C3 >;
};

struct D3
{
	using ConstructWith = rod::annotation::ConstructWith< D1, C2 >;
};


void
test()
{
	static_assert( std::is_same<
						rod::CreateContainer<
								rod::AsSingleton< D1 >,
								rod::AsSingleton< D2 >,
								rod::AsSingleton< D3 > >::r::GetDependencies::r,
						rod::TypeList< C1, C3, D1, C2 >
				   >::value,
				   "Unexpected declared dependencies" );
}