#pragma once




namespace rod
{

	template< typename ContextOwner >
	class ContextAccessor
	{
	private:
		ContextOwner&	contextOwner;


	public:
		using Context = typename ContextOwner::GetContext::r;


		ContextAccessor( ContextOwner& contextOwner ):
		  contextOwner( contextOwner )
		{}

		Context&
		context()
		{
			return contextOwner.getContext();
		}
	};


	template< typename ContextOwner >
	auto
	accessContext( ContextOwner& contextOwner )
		-> decltype( ContextAccessor< ContextOwner >( contextOwner ).context() )
	{
		return ContextAccessor< ContextOwner >( contextOwner ).context();
	}
	
}