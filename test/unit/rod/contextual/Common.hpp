#pragma once


#include <cassert>
#include <iostream>

#include "rod/Context.hpp"




template< template< typename > class Test >
void
enter()
{
	using initialContext = rod::CreateInitialContext<>::r;
	using boundTest = Test< initialContext >;

	auto			nullCtx = rod::createNullContext();
	boundTest		test( nullCtx );
	test.enter();
}


struct Trackable
{
	static int	constructors;
	static int	copyConstructors;
	static int	moveConstructors;
	static int	copyAssignments;
	static int	moveAssignments;
	static int	destructors;

	Trackable()
	{
		++constructors;
	}

	Trackable( const Trackable& )
	{
		++copyConstructors;
	}

	Trackable( Trackable&& )
	{
		++moveConstructors;
	}

	Trackable&
	operator = ( const Trackable& )
	{
		++copyAssignments;
		return *this;
	}

	Trackable&
	operator = ( Trackable&& )
	{
		++moveAssignments;
		return *this;
	}

	~Trackable()
	{
		++destructors;
	}
};

int	Trackable::constructors = 0;
int	Trackable::copyConstructors = 0;
int	Trackable::moveConstructors = 0;
int	Trackable::copyAssignments = 0;
int	Trackable::moveAssignments = 0;
int	Trackable::destructors = 0;

void
assertTrackable( int c, int cc, int mc, int ca, int ma, int d )
{
	std::cout << "Constructors: " << Trackable::constructors << ", expected " << c << std::endl;
	std::cout << "Copy constructors: " << Trackable::copyConstructors << ", expected " << cc << std::endl;
	std::cout << "Move constructors: " << Trackable::moveConstructors << ", expected " << mc << std::endl;
	std::cout << "Copy assignments: " << Trackable::copyAssignments << ", expected " << ca << std::endl;
	std::cout << "Move assignments: " << Trackable::moveAssignments << ", expected " << ma << std::endl;
	std::cout << "Destructors: " << Trackable::destructors << ", expected " << d << std::endl;
	assert( c == Trackable::constructors );
	assert( cc == Trackable::copyConstructors );
	assert( mc == Trackable::moveConstructors );
	assert( ca == Trackable::copyAssignments );
	assert( ma == Trackable::moveAssignments );
	assert( d == Trackable::destructors );
}