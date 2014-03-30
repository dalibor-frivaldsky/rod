#pragma once


#include <iostream>
#include <typeinfo>

#include <rod/Reduce.hpp>
#include <rod/common/NullType.hpp>
#include <rod/common/Pair.hpp>
#include <rod/common/Select.hpp>
#include <rod/common/SuperSubClass.hpp>
#include <rod/common/TypeName.hpp>




namespace rod
{

	template< typename... Types >
	struct TypeList;



	namespace typelist
	{

		template< typename TypeList >
		struct Head;

		template<>
		struct Head< TypeList<> >
		{
			using r = common::NullType;
		};

		template< typename Head_, typename... Tail >
		struct Head< TypeList< Head_, Tail... > >
		{
			using r = Head_;
		};


		template< typename TypeList >
		struct Tail;

		template<>
		struct Tail< TypeList<> >
		{
			using r = TypeList<>;
		};

		template< typename Head, typename... Tail_ >
		struct Tail< TypeList< Head, Tail_... > >
		{
			using r = TypeList< Tail_... >;
		};

	
		template< typename TypeList >
		struct Length;

		template<>
		struct Length< TypeList<> >
		{
			enum { r = 0 };
		};

		template< typename Head, typename... Tail >
		struct Length< TypeList< Head, Tail... > >
		{
			enum { r = 1 + Length< TypeList< Tail... > >::r };
		};


		template< typename TypeList, int Index >
		struct TypeAt;

		template< typename Head, typename... Tail >
		struct TypeAt< TypeList< Head, Tail... >, 0 >
		{
			using r = Head;
		};

		template< typename Head, typename... Tail, int Index >
		struct TypeAt< TypeList< Head, Tail... >, Index >
		{
			using r = typename TypeAt< TypeList< Tail... >, Index - 1 >::r;
		};


		template< typename TypeList, typename SearchedType >
		struct IndexOf;

		template< typename SearchedType >
		struct IndexOf< TypeList<>, SearchedType >
		{
			enum { r = -1 };
		};

		template< typename Head, typename... Tail >
		struct IndexOf< TypeList< Head, Tail... >, Head >
		{
			enum { r = 0 };
		};

		template< typename Head, typename... Tail, typename SearchedType >
		struct IndexOf< TypeList< Head, Tail... >, SearchedType >
		{
		private:
			enum { temp = IndexOf< TypeList< Tail... >, SearchedType >::r };

		public:
			enum { r = temp == -1 ? -1 : 1 + temp };
		};

		/*template< typename... TypesBefore, typename... TypesAfter, typename SearchFor >
		struct IndexOf< TypeList< TypesBefore..., SearchFor, TypesAfter... >, SearchFor >
		{
			static constexpr int	r = sizeof...( TypesBefore );
		};

		template< typename... Types, typename SearchFor >
		struct IndexOf< TypeList< Types... >, SearchFor >
		{
			static constexpr int	r = -1;
		};*/


		template< typename SearchIn, typename SearchFor >
		struct Contains
		{
			enum { r = IndexOf< SearchIn, SearchFor >::r == -1 ? false : true };
		};


		template< typename SearchIn, typename SearchFor >
		struct ContainsAll;

		template< typename SearchIn, typename... SearchFor >
		struct ContainsAll< SearchIn, TypeList< SearchFor... > >
		{
		private:
			template< bool v, bool acc >
			struct And
			{
				enum{ r = v && acc };
			};

		public:
			enum { r = ReduceBool< And, true, Contains< SearchIn, SearchFor >::r... >::r };

		};


		template< typename TypeList, typename ToAdd >
		struct Append;

		template< typename... Types, typename ToAdd >
		struct Append< TypeList< Types... >, ToAdd >
		{
			using r = TypeList< Types..., ToAdd >;
		};


		template< typename AppendTo, typename ToAppend >
		struct AppendAll;

		template< typename... AppendToTypes, typename... ToAppendTypes >
		struct AppendAll< TypeList< AppendToTypes... >, TypeList< ToAppendTypes... > >
		{
			using r = TypeList< AppendToTypes..., ToAppendTypes... >;
		};


		template< typename TypeList, typename ToAdd >
		struct Prepend;

		template< typename... Types, typename ToAdd >
		struct Prepend< TypeList< Types... >, ToAdd >
		{
			using r = TypeList< ToAdd, Types... >;
		};

		template< typename PrependTo, typename ToPrepend >
		struct PrependAll;

		template< typename... PrependToTypes, typename... ToPrependTypes >
		struct PrependAll< TypeList< PrependToTypes... >, TypeList< ToPrependTypes... > >
		{
			using r = TypeList< ToPrependTypes..., PrependToTypes... >;
		};

		
		template< typename TypeList, int SplitPos >
		struct Split;

		template<>
		struct Split< TypeList<>, 0 >
		{
			using r = common::Pair< TypeList<>, TypeList<> >;
		};

		template< typename Head, typename... Tail >
		struct Split< TypeList< Head, Tail... >, 0 >
		{
			using r = common::Pair< TypeList<>, TypeList< Head, Tail... > >;
		};

		template< typename Head, typename... Tail, int SplitPos >
		struct Split< TypeList< Head, Tail... >, SplitPos >
		{
		private:
			using tailSplit = typename Split< TypeList< Tail... >, SplitPos - 1 >::r;

		public:
			using r = common::Pair< typename Prepend< typename tailSplit::First, Head >::r,
									typename tailSplit::Second >;
		};


		template< typename TypeList, typename ToRemove >
		struct Remove;

		template< typename... Tail, typename ToRemove >
		struct Remove< TypeList< ToRemove, Tail... >, ToRemove >
		{
			using r = TypeList< Tail... >;
		};

		template< typename ToRemove >
		struct Remove< TypeList<>, ToRemove >
		{
			using r = TypeList<>;
		};

		template< typename Head, typename... Tail, typename ToRemove >
		struct Remove< TypeList< Head, Tail... >, ToRemove >
		{
			using r = typename Prepend< typename Remove< TypeList< Tail... >, ToRemove >::r, Head >::r;
		};



		template< typename RemoveFrom, typename ToRemove >
		struct RemoveList;

		template< typename RemoveFrom, typename... ToRemove >
		struct RemoveList< RemoveFrom, TypeList< ToRemove... > >
		{
		private:
			template< typename RemoveWhat, typename RemoveFromRes >
			struct RemoveOp
			{
				using r = typename RemoveFromRes::template Remove< RemoveWhat >::r;
			};

		public:
			using r = typename Reduce< RemoveOp, RemoveFrom, ToRemove... >::r;
		};


		template< typename TypeList, typename ToRemove >
		struct RemoveAll;

		template< typename ToRemove >
		struct RemoveAll< TypeList<>, ToRemove >
		{
			using r = TypeList<>;
		};

		template< typename... Tail, typename ToRemove >
		struct RemoveAll< TypeList< ToRemove, Tail... >, ToRemove >
		{
			using r = typename RemoveAll< TypeList< Tail... >, ToRemove >::r;
		};

		template< typename Head, typename... Tail, typename ToRemove >
		struct RemoveAll< TypeList< Head, Tail... >, ToRemove >
		{
			using r = typename Prepend< typename RemoveAll< TypeList< Tail... >, ToRemove >::r, Head >::r;
		};


		template< typename TypeList >
		struct RemoveDuplicates;

		template<>
		struct RemoveDuplicates< TypeList<> >
		{
			using r = TypeList<>;
		};

		template< typename Head, typename... Tail >
		struct RemoveDuplicates< TypeList< Head, Tail... > >
		{
		private:
			using noHeadInTail = typename RemoveAll< TypeList< Tail... >, Head >::r;

		public:
			using r = typename Prepend< typename RemoveDuplicates< noHeadInTail >::r, Head >::r;
		};


		template< typename TypeList, typename ReplaceWhat, typename ReplaceWith >
		struct Replace;

		template< typename ReplaceWhat, typename ReplaceWith >
		struct Replace< TypeList<>, ReplaceWhat, ReplaceWith >
		{
			using r = TypeList<>;
		};

		template< typename... Tail, typename ReplaceWhat, typename ReplaceWith >
		struct Replace< TypeList< ReplaceWhat, Tail... >, ReplaceWhat, ReplaceWith >
		{
		private:
			using nextReplaced = typename Replace< TypeList< Tail... >, ReplaceWhat, ReplaceWith >::r;

		public:
			using r = typename Prepend< nextReplaced, ReplaceWith >::r;
		};

		template< typename Head, typename... Tail, typename ReplaceWhat, typename ReplaceWith >
		struct Replace< TypeList< Head, Tail... >, ReplaceWhat, ReplaceWith >
		{
		private:
			using nextReplaced = typename Replace< TypeList< Tail... >, ReplaceWhat, ReplaceWith >::r;

		public:
			using r = typename Prepend< nextReplaced, Head >::r;
		};


		template< typename TypeList, template< typename > class Selector >
		struct Select;

		template< template< typename > class Selector >
		struct Select< TypeList<>, Selector >
		{
			using r = TypeList<>;
		};

		template< typename Head, typename... Tail, template< typename > class Selector >
		struct Select< TypeList< Head, Tail... >, Selector >
		{
		private:
			using tailSelect = typename Select< TypeList< Tail... >, Selector >::r;

		public:
			using r = typename common::Select<
									Selector< Head >::r,
									typename Prepend< tailSelect, Head >::r,
									tailSelect >::r;
		};


		template< typename TypeList, template< typename > class Applier >
		struct Apply;

		template< template< typename > class Applier >
		struct Apply< TypeList<>, Applier >
		{
			using r = TypeList<>;
		};

		template< typename Head, typename... Rest, template< typename > class Applier >
		struct Apply< TypeList< Head, Rest... >, Applier >
		{
		private:
			using restApplied = typename Apply< TypeList< Rest... >, Applier >::r;
			using headApplied = typename Applier< Head >::r;

		public:
			using r = typename Prepend< restApplied, headApplied >::r;
		};


		template< typename TypeList, template< typename... > class Destination >
		struct UnpackTo;

		template< typename... Type, template< typename... > class Destination >
		struct UnpackTo< TypeList< Type... >, Destination >
		{
			using r = Destination< Type... >;
		};


		template< typename TypeList, typename Super >
		struct MostSuper;

		template< typename Super >
		struct MostSuper< TypeList<>, Super >
		{
			using r = Super;
		};

		template< typename Head, typename... Tail, typename Super >
		struct MostSuper< TypeList< Head, Tail... >, Super >
		{
		private:
			enum { headIsSuper = common::StrictSuperSubClass< Head, Super >::r };
			using currentSuper = typename common::Select< headIsSuper, Head, Super >::r;

		public:
			using r = typename MostSuper< TypeList< Tail... >, currentSuper >::r;
		};


		template< typename TypeList, typename Derived >
		struct MostDerived;

		template< typename Derived >
		struct MostDerived< TypeList<>, Derived >
		{
			using r = Derived;
		};

		template< typename Head, typename... Tail, typename Derived >
		struct MostDerived< TypeList< Head, Tail... >, Derived >
		{
		private:
			enum { headIsDerived = common::StrictSuperSubClass< Derived, Head >::r };
			using currentDerived = typename common::Select< headIsDerived, Head, Derived >::r;

		public:
			using r = typename MostDerived< TypeList< Tail... >, currentDerived >::r;
		};


		template< typename TypeList >
		struct DerivedToFront;

		template<>
		struct DerivedToFront< TypeList<> >
		{
			using r = TypeList<>;
		};

		template< typename Head >
		struct DerivedToFront< TypeList< Head > >
		{
			using r = TypeList< Head >;
		};

		template< typename Head, typename... Tail >
		struct DerivedToFront< TypeList< Head, Tail... > >
		{
		private:
			using currentDerived = typename MostDerived< TypeList< Tail... >, Head >::r;
			using nextTypeList = typename Replace< TypeList< Tail... >, currentDerived, Head >::r;
			using partialSort = typename DerivedToFront< nextTypeList >::r;

		public:
			using r = typename Prepend< partialSort, currentDerived >::r;
		};

	}



	namespace debug
	{

		template< typename TL >
		struct Print;

		template<>
		struct Print< TypeList<> >
		{
			Print()
			{
				std::cout << "[";
				std::cout << "]" << std::endl;
			}
		};

		template< typename TL >
		struct Print
		{
		private:
			template< typename TL_ >
			struct PrintTypes;

			template< typename Head >
			struct PrintTypes< TypeList< Head > >
			{
				PrintTypes()
				{
					std::cout << common::typeName< Head >();
				}
			};

			template< typename Head, typename... Tail >
			struct PrintTypes< TypeList< Head, Tail... > >
			{
				PrintTypes()
				{
					std::cout << common::typeName< Head >() << ",";
					PrintTypes< TypeList< Tail... > >();
				}
			};

		public:
			Print()
			{
				std::cout << "[";
				PrintTypes< TL >();
				std::cout << "]" << std::endl;
			}
		};

	}



	template< typename... Types >
	struct TypeList
	{
	private:
		using This = TypeList< Types... >;


	public:
		using Head = typelist::Head< This >;

		using Tail = typelist::Tail< This >;

		using Length = typelist::Length< This >;

		template< int Index >
		using At = typelist::TypeAt< This, Index >;

		template< typename Type >
		using IndexOf = typelist::IndexOf< This, Type >;

		template< typename SearchFor >
		using Contains = typelist::Contains< This, SearchFor >;

		template< typename SearchFor >
		using ContainsAll = typelist::ContainsAll< This, SearchFor >;

		template< typename Type >
		using Append = typelist::Append< This, Type >;

		template< typename ToAppend >
		using AppendAll = typelist::AppendAll< This, ToAppend >;

		template< typename Type >
		using Prepend = typelist::Prepend< This, Type >;

		template< typename ToPrepend >
		using PrependAll = typelist::PrependAll< This, ToPrepend >;

		template< int Index >
		using Split = typelist::Split< This, Index >;

		template< typename Type >
		using Remove = typelist::Remove< This, Type >;

		template< typename ToRemove >
		using RemoveList = typelist::RemoveList< This, ToRemove >;

		template< typename Type >
		using RemoveAll = typelist::RemoveAll< This, Type >;

		using RemoveDuplicates = typelist::RemoveDuplicates< This >;

		template< typename What, typename With >
		using Replace = typelist::Replace< This, What, With >;

		template< template< typename > class Selector >
		using Select = typelist::Select< This, Selector >;

		template< template< typename > class Applier >
		using Apply = typelist::Apply< This, Applier >;

		template< template< typename... > class Destination >
		using UnpackTo = typelist::UnpackTo< This, Destination >;

		using MostSuper = typelist::MostSuper< This, typename Head::r >;

		using MostDerived = typelist::MostDerived< This, typename Head::r >;

		using DerivedToFront = typelist::DerivedToFront< This >;
	};

}