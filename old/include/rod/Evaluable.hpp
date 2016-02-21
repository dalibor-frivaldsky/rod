#pragma once


#include <functional>
#include <utility>




namespace rod
{

	template< typename EvaluableCallback >
	struct Evaluable
	{
	private:
		std::function< void( EvaluableCallback& ) >		trampoline;


	public:
		Evaluable( Evaluable< EvaluableCallback >& other ):
		  trampoline( other.trampoline )
		{}

		Evaluable( Evaluable< EvaluableCallback >&& other ):
		  trampoline( std::move( other.trampoline ) )
		{}

		template< typename Context >
		Evaluable( Context& context )
		{
			trampoline = [&] ( EvaluableCallback& evaluableCallback )
			{
				evaluableCallback( context );
			};
		}

		inline
		void
		operator () ( EvaluableCallback&& evaluableCallback )
		{
			trampoline( evaluableCallback );
		}
	};
	
}