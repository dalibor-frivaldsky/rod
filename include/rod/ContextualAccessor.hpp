#pragma once




namespace rod
{

	template< typename BoundContextual >
	class ContextualAccessor
	{
	private:
		BoundContextual*	contextual;


	public:
		using Context = typename BoundContextual::Ctx;


		ContextualAccessor( BoundContextual* contextual ):
		  contextual( contextual )
		{}

		Context&
		context()
		{
			return contextual->context;
		}
	};
	
}