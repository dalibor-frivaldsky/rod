#include <cassert>
#include <type_traits>

#include <rod/annotation/Requires.hpp>
#include <rod/experimental/construct/Provided.hpp>
#include <rod/experimental/construct/TypeCreator.hpp>




struct Referenced
{
	float f;
};

struct Type
{
	using Requires = rod::annotation::Requires< int, Referenced& >;

	int			i;
	Referenced&	r = rod::experimental::construct::Provided::ref;
};


void
test()
{
	Referenced	referenced { 2.0f };

	using Creator = rod::experimental::construct::TypeCreator< Type >;

	Type	t = Creator( 10, referenced );
	assert( t.r.f == 2.0f );
}