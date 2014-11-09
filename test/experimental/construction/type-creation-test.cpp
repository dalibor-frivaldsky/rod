#include <cassert>
#include <type_traits>

#include <rod/annotation/Requires.hpp>
#include <rod/experimental/construct/TypeCreator.hpp>




struct Aggregated
{
	int	i;
};

struct Referenced
{
	float f;
};

struct Type
{
	using Requires = rod::annotation::Requires< double, Aggregated, Referenced& >;

	double		d;
	Aggregated	a;
	Referenced&	r;

	Type( Referenced& r ):
	  r( r )
	{
		// not called
		assert( false );
	}
};


void
test()
{
	Referenced	referenced { 2.0f };

	using Creator = rod::experimental::construct::TypeCreator< Type >;

	Type	t = Creator( 1.0, Aggregated { 10 }, referenced );
	assert( t.d == 1.0 );
	assert( t.a.i == 10 );
	assert( t.r.f == 2.0f );
}