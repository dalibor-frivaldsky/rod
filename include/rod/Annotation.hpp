#pragma once


#include <rod/common/MakeVoid.hpp>




#define ROD_Annotation( a ) \
template< typename NotAnnotated, typename = void > \
struct Has ## a ## Annotation \
{ \
	enum { r = false }; \
}; \
\
template< typename Annotated > \
struct Has ## a ## Annotation< Annotated, typename ::rod::common::MakeVoid< typename Annotated::a >::r > \
{ \
	enum { r = true }; \
}; \
\
\
template< typename Type > \
struct Is ## a \
{ \
	enum { r = Has ## a ## Annotation< Type >::r }; \
};
