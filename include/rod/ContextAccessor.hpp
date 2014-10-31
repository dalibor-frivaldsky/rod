#pragma once




namespace rod
{

	template< typename ContextOwner_ >
	class ContextAccessor
	{
	private:
		ContextOwner_&	contextOwner;


	public:
		using Context = typename ContextOwner_::GetContext::r;


		ContextAccessor( ContextOwner_& contextOwner ):
		  contextOwner( contextOwner )
		{}

		Context&
		context()
		{
			return contextOwner.getContext();
		}
	};


	template< typename ContextOwner_ >
	auto
	accessContext( ContextOwner_& contextOwner )
		-> decltype( ContextAccessor< ContextOwner_ >( contextOwner ).context() )
	{
		return ContextAccessor< ContextOwner_ >( contextOwner ).context();
	}

	template< typename ContextOwner_ >
	struct AccessContext
	{
		using r = typename ContextAccessor< ContextOwner_ >::Context;
	};
	
}