#include <cassert>
#include <type_traits>

#include <rod/experimental/construct/TypeConstructor.hpp>




struct Aggregated
{
	int	i;
};

struct Referenced
{
	float f;
};


void
test()
{
	Referenced	referenced { 2.0f };

	using Constructor = rod::experimental::construct::TypeConstructor< double, Aggregated, Referenced& >;

	Constructor	constructor( 1.0, Aggregated { 10 }, referenced );
	assert( ((rod::experimental::construct::MemberConstructor< double >&) constructor).value == 1.0 );
	assert( ((rod::experimental::construct::MemberConstructor< Aggregated >&) constructor).value.i == 10 );
	assert( ((rod::experimental::construct::MemberConstructor< Referenced& >&) constructor).value.f == 2.0f );
}