#pragma once


#include <string>

#include <rod/Reduce.hpp>
#include <rod/common/IntToType.hpp>
#include <rod/common/Select.hpp>




namespace rod
{

	template< char... chars >
	struct TString;


	namespace tstring
	{

		template< typename TStr, int Index >
		struct At;

		template< char FirstChar, char... Rest >
		struct At< TString< FirstChar, Rest... >, 0 >
		{
			enum { r = FirstChar };
		};

		template< char FirstChar, char... Rest, int Index >
		struct At< TString< FirstChar, Rest... >, Index >
		{
			enum { r = At< TString< Rest... >, Index - 1 >::r };
		};


		template< typename AppendTo, typename AppendWhat >
		struct Append;

		template< char... AppendToChars, char... AppendWhatChars >
		struct Append< TString< AppendToChars... >, TString< AppendWhatChars... > >
		{
			using r = TString< AppendToChars..., AppendWhatChars... >;
		};


		template< typename TString >
		struct GatherString;

		template<>
		struct GatherString< TString<> >
		{
			static
			std::string
			gather()
			{
				return std::string();
			}
		};

		template< char FirstChar, char... Rest >
		struct GatherString< TString< FirstChar, Rest... > >
		{
			static
			std::string
			gather()
			{
				return std::string( 1, FirstChar ) + GatherString< TString< Rest... > >::gather();
			}
		};


		template< std::size_t N >
		common::IntToType< N >
		stringLength( const char (&s)[ N ] )
		{}


		template< char... chars >
		struct CreateTString
		{
		private:
			template< typename SingleCharString, typename ResultString >
			struct AppendChar
			{
				using r = typename common::Select<
											SingleCharString::template At< 0 >::r == 0,
											ResultString,
											typename ResultString::template Append< SingleCharString >::r >::r;
			};

		public:
			using r = typename Reduce< AppendChar, TString<>, TString< chars >... >::r;
		};
		
	}


	template< char... chars >
	struct TString
	{
	private:

		using This = TString< chars... >;


	public:

		template< int Index >
		using At = tstring::At< This, Index >;

		template< typename ToAppend >
		using Append = tstring::Append< This, ToAppend >;

		static
		const std::string&
		toString()
		{
			static std::string	str = tstring::GatherString< This >::gather();

			return str;
		}

	};
	
}


#define ROD_TString_Extract( str, index ) index < decltype( rod::tstring::stringLength( (str) ) )::v ? (str)[ index ] : 0

#define ROD_TString( str ) rod::tstring::CreateTString< ROD_TString_Extract( (str), 0 ), \
														ROD_TString_Extract( (str), 1 ), \
														ROD_TString_Extract( (str), 2 ), \
														ROD_TString_Extract( (str), 3 ), \
														ROD_TString_Extract( (str), 4 ), \
														ROD_TString_Extract( (str), 5 ), \
														ROD_TString_Extract( (str), 6 ), \
														ROD_TString_Extract( (str), 7 ), \
														ROD_TString_Extract( (str), 8 ), \
														ROD_TString_Extract( (str), 9 ), \
														ROD_TString_Extract( (str), 10 ), \
														ROD_TString_Extract( (str), 11 ), \
														ROD_TString_Extract( (str), 12 ), \
														ROD_TString_Extract( (str), 13 ), \
														ROD_TString_Extract( (str), 14 ), \
														ROD_TString_Extract( (str), 15 ), \
														ROD_TString_Extract( (str), 16 ), \
														ROD_TString_Extract( (str), 17 ), \
														ROD_TString_Extract( (str), 18 ), \
														ROD_TString_Extract( (str), 19 ), \
														ROD_TString_Extract( (str), 20 ), \
														ROD_TString_Extract( (str), 21 ), \
														ROD_TString_Extract( (str), 22 ), \
														ROD_TString_Extract( (str), 23 ), \
														ROD_TString_Extract( (str), 24 ), \
														ROD_TString_Extract( (str), 25 ), \
														ROD_TString_Extract( (str), 26 ), \
														ROD_TString_Extract( (str), 27 ), \
														ROD_TString_Extract( (str), 28 ), \
														ROD_TString_Extract( (str), 29 ), \
														ROD_TString_Extract( (str), 30 ), \
														ROD_TString_Extract( (str), 31 ), \
														ROD_TString_Extract( (str), 32 ), \
														ROD_TString_Extract( (str), 33 ), \
														ROD_TString_Extract( (str), 34 ), \
														ROD_TString_Extract( (str), 35 ), \
														ROD_TString_Extract( (str), 36 ), \
														ROD_TString_Extract( (str), 37 ), \
														ROD_TString_Extract( (str), 38 ), \
														ROD_TString_Extract( (str), 39 ), \
														ROD_TString_Extract( (str), 40 ), \
														ROD_TString_Extract( (str), 41 ), \
														ROD_TString_Extract( (str), 42 ), \
														ROD_TString_Extract( (str), 43 ), \
														ROD_TString_Extract( (str), 44 ), \
														ROD_TString_Extract( (str), 45 ), \
														ROD_TString_Extract( (str), 46 ), \
														ROD_TString_Extract( (str), 47 ), \
														ROD_TString_Extract( (str), 48 ), \
														ROD_TString_Extract( (str), 49 ), \
														ROD_TString_Extract( (str), 50 ), \
														ROD_TString_Extract( (str), 51 ), \
														ROD_TString_Extract( (str), 52 ), \
														ROD_TString_Extract( (str), 53 ), \
														ROD_TString_Extract( (str), 54 ), \
														ROD_TString_Extract( (str), 55 ), \
														ROD_TString_Extract( (str), 56 ), \
														ROD_TString_Extract( (str), 57 ), \
														ROD_TString_Extract( (str), 58 ), \
														ROD_TString_Extract( (str), 59 ), \
														ROD_TString_Extract( (str), 60 ), \
														ROD_TString_Extract( (str), 61 ), \
														ROD_TString_Extract( (str), 62 ), \
														ROD_TString_Extract( (str), 63 ), \
														ROD_TString_Extract( (str), 64 ) >::r
