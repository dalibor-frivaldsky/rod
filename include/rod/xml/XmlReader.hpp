#pragma once


#include <cstring>
#include <memory>
#include <tuple>

#include <rod/Contextual.hpp>
#include <rod/common/Sequence.hpp>
#include <rod/xml/XmlContent.hpp>
#include <rod/xml/annotation/ChildElement.hpp>
#include <rod/xml/annotation/DataNode.hpp>
#include <rod/xml/annotation/RootElement.hpp>

#include <rapidxml.hpp>




namespace rod
{

	namespace xml
	{

		namespace xmlReader
		{

			using XmlDocument = rapidxml::xml_document< char >;
			using XmlNode = rapidxml::xml_node< char >;


			template< template< typename > class Element >
			struct ProcessElement;


			template< typename Attributes, typename ReturnType >
			struct
			CallProcessSelf;

			template< typename... Attribute >
			struct
			CallProcessSelf< annotation::Attributes< Attribute... >, void >
			{
				template< typename BoundElement >
				static
				std::tuple<>
				call( BoundElement* element, XmlNode* node )
				{
					BoundElement::Element::ProcessMethod::call(
							element,
							node->first_attribute( Attribute::toString().c_str() )->value()... );

					return std::make_tuple();
				}
			};

			template< typename... Attribute, typename Return >
			struct
			CallProcessSelf< annotation::Attributes< Attribute... >, Return >
			{
				template< typename BoundElement >
				static
				std::tuple< typename BoundElement::Element::ProcessMethod::Return >
				call( BoundElement* element, XmlNode* node )
				{
					using returnTuple = std::tuple< typename BoundElement::Element::ProcessMethod::Return >;

					return returnTuple(
								BoundElement::Element::ProcessMethod::call(
									element,
									node->first_attribute( Attribute::toString().c_str() )->value()... ) );
				}
			};

			template< typename BoundElement >
			auto
			callProcessSelf( BoundElement* element, XmlNode* node )
				-> decltype( CallProcessSelf<
								typename BoundElement::Element::Attributes,
								typename BoundElement::Element::ProcessMethod::Return >::call( element, node ) )
			{
				return CallProcessSelf<
							typename BoundElement::Element::Attributes,
							typename BoundElement::Element::ProcessMethod::Return >::call( element, node );
			}


			template< typename ChildElementProcessMethod, typename BoundElement,
					  typename... SelfArg, int... SelfSeq,
					  typename... ChildArg, int... ChildSeq >
			void
			callProcessChildSequential(
					BoundElement* element,
					std::tuple< SelfArg... >& selfArg, common::Sequence< SelfSeq... >&&,
					std::tuple< ChildArg... >&& childArg, common::Sequence< ChildSeq... >&& )
			{
				ChildElementProcessMethod::call(
						element,
						std::get< SelfSeq >( selfArg )...,
						std::get< ChildSeq >( std::forward< std::tuple< ChildArg... > >( childArg ) )... );
			}

			template< typename ChildElementProcessMethod, typename BoundElement, typename... SelfArg, typename... ChildArg >
			void
			callProcessChild( BoundElement* element, std::tuple< SelfArg... >& selfArg, std::tuple< ChildArg... >&& childArg )
			{
				using selfSeq = typename common::GenerateSequence< sizeof...( SelfArg ) >::r;
				using childSeq = typename common::GenerateSequence< sizeof...( ChildArg ) >::r;
				callProcessChildSequential< ChildElementProcessMethod >( element, selfArg, selfSeq(), std::move( childArg ), childSeq() );
			}


			template< typename ChildElements >
			struct
			ProcessChildren;

			template<>
			struct
			ProcessChildren< TypeList<> >
			{
				template< typename Contextual, typename BoundElement, typename SelfResult >
				static
				void
				process( Contextual* ctx, BoundElement* element, SelfResult& selfResult, XmlNode* node )
				{
					std::cout << node->type() << std::endl;
					// silently ignore undefined element
				}
			};

			template< template< typename > class ChildElementImpl, typename ProcessMethod, typename... Rest >
			struct
			ProcessChildren<
				TypeList<
					annotation::ChildElement< ChildElementImpl, ProcessMethod >,
					Rest... > >
			{
				template< typename Contextual, typename BoundElement, typename SelfResult >
				static
				void
				process( Contextual* ctx, BoundElement* element, SelfResult& selfResult, XmlNode* node )
				{
					if( node->type() == rapidxml::node_element &&
						decltype( bind< ChildElementImpl >( ctx ) )::Element::Name::toString() == node->name() )
					{
						std::cout << "element node" << std::endl;

						callProcessChild< ProcessMethod >(
								element,
								selfResult,
								ProcessElement< ChildElementImpl >::process( ctx, node ) );
					}
					else
					{
						ProcessChildren< TypeList< Rest... > >::process( ctx, element, selfResult, node );
					}
				}
			};

			template< typename ProcessMethod, typename... Rest >
			struct
			ProcessChildren<
				TypeList<
					annotation::DataNode< ProcessMethod >,
					Rest... > >
			{
				template< typename Contextual, typename BoundElement, typename SelfResult >
				static
				void
				process( Contextual* ctx, BoundElement* element, SelfResult& selfResult, XmlNode* node )
				{
					if( node->type() == rapidxml::node_data )
					{
						std::cout << "data node" << std::endl;

						callProcessChild< ProcessMethod >(
								element,
								selfResult,
								std::make_tuple( std::string( node->value() ) ) );
					}
					else
					{
						ProcessChildren< TypeList< Rest... > >::process( ctx, element, selfResult, node );
					}
				}
			};


			template< template< typename > class Element >
			struct ProcessElement
			{
				template< typename Contextual >
				static
				auto
				process( Contextual* ctx, XmlNode* node )
					-> decltype( callProcessSelf( create< Element >( ctx ).get(), node ) )
				{
					auto	element = create< Element >( ctx );
					using	boundElement = typename decltype( element )::element_type;
					auto	selfResult = callProcessSelf( element.get(), node );

					XmlNode*	childNode = node->first_node();
					while( childNode != 0 )
					{
						ProcessChildren< typename boundElement::Element::ChildElements >::process( ctx, element.get(), selfResult, childNode );

						childNode = childNode->next_sibling();
					}

					return std::move( selfResult );
				}
			};
			
		}


		template< typename Context >
		class XmlReader:
			public Contextual< Context >
		{
		private:

			using This = XmlReader< Context >;

			using XmlDocument = rapidxml::xml_document< char >;
			using XmlNode = rapidxml::xml_node< char >;


		public:

			void
			read()
			{
				auto&	xmlContent = resolve< XmlContent >( this );
				auto	charContent = readContent( xmlContent );

				XmlDocument		doc;
				doc.template parse< 0 >( charContent.get() );

				using	rootElement = typename This::template FindAnnotatedContextual< annotation::IsRootElement >::r::Head::r;

				XmlNode*	rootNode = doc.first_node( rootElement::Element::Name::toString().c_str() );
				xmlReader::ProcessElement< rootElement::template Contextual >::process( this, rootNode );
			}


		private:

			std::unique_ptr< char[] >
			readContent( XmlContent& xmlContent )
			{
				std::string		stringContent;

				while( xmlContent.atEnd() == false )
				{
					stringContent += xmlContent.readLine();
				}

				std::unique_ptr< char[] >		charContent( new char[ stringContent.size() + 1 ] );
				std::memcpy( charContent.get(), stringContent.data(), stringContent.size() );
				charContent[ stringContent.size() ] = '\0';

				return charContent;
			}

		};
		
	}
	
}