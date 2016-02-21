#pragma once




namespace rod
{

	template< template< bool, bool > class ReduceOp, bool Default, bool... ReduceWhat >
	struct ReduceBool;

	template< template< bool, bool > class ReduceOp, bool Default >
	struct ReduceBool< ReduceOp, Default >
	{
		enum { r = Default };
	};

	template< template< bool, bool > class ReduceOp, bool Default, bool ReduceWhat, bool... ReduceRest >
	struct ReduceBool< ReduceOp, Default, ReduceWhat, ReduceRest... >
	{
		enum{ r = ReduceOp< ReduceWhat, ReduceBool< ReduceOp, Default, ReduceRest... >::r >::r };
	};


	template< template< typename, typename > class ReduceOp, typename Initial, typename... ReduceWhat >
	struct Reduce;

	template< template< typename, typename > class ReduceOp, typename Initial >
	struct Reduce< ReduceOp, Initial >
	{
		using r = Initial;
	};

	template< template< typename, typename > class ReduceOp, typename Initial, typename ReduceWhat, typename... ReduceRest >
	struct Reduce< ReduceOp, Initial, ReduceWhat, ReduceRest... >
	{
	private:
		using reduced = typename ReduceOp< ReduceWhat, Initial >::r;

	public:
		using r = typename Reduce< ReduceOp, reduced, ReduceRest... >::r;
	};

	
}