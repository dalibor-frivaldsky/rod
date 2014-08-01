#include "rod/AsSingleton.hpp"
#include "rod/Contextual.hpp"


#include <type_traits>

#include "Common.hpp"




struct T
{};


template< typename Context >
class Test:
	public rod::Contextual<
					Context,
					rod::AsSingleton< T > >
{
private:

	using This = Test< Context >;
	

public:

	ROD_Contextual_Constructor( Test )
	
	void
	enter()
	{
		static_assert( std::is_same<
							typename This::Ctx::GetTypeRegistry::r,
							rod::TypeRegistry< T >
					   >::value,
					   "Unexpected TypeRegistry" );
	}
};


void
test()
{
	enter< Test >();
}