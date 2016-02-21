#pragma once


#include <tuple>
#include <utility>

#include <rod/ContextAccessor.hpp>
#include <rod/TypeList.hpp>




template< typename... ToProvide >
struct MockContext;

template< typename... ToProvide >
struct MockContext
{
private:
	using This = MockContext< ToProvide... >;

	std::tuple< ToProvide... >	toProvideTuple;

	inline
	This&
	getContext()
	{
		return *this;
	}

	struct GetContext
	{
		using r = This;
	};


public:
	friend class rod::ContextAccessor< This >;

	MockContext( ToProvide&&... toProvide ):
	  toProvideTuple( std::forward< ToProvide >( toProvide )... )
	{}

	template< typename ToResolve >
	ToResolve
	resolve()
	{
		return std::get<
					rod::TypeList< ToProvide... >::template IndexOf<
						ToResolve >::r >( toProvideTuple );
	}
};


template< typename... ToProvide >
MockContext< ToProvide... >
mockContext( ToProvide&&... toProvide )
{
	return MockContext< ToProvide... >(
				std::forward< ToProvide >( toProvide )... );
}
