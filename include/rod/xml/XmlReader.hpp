#pragma once


#include <tuple>

#include <rod/Contextual.hpp>
#include <rod/common/Sequence.hpp>
#include <rod/xml/XmlNodeType.hpp>
#include <rod/xml/annotation/ChildElement.hpp>
#include <rod/xml/annotation/DataNode.hpp>
#include <rod/xml/annotation/RootElement.hpp>
#include <rod/xml/annotation/XmlParserProvider.hpp>




namespace rod
{

	namespace xml
	{

		namespace xmlReader
		{

			template< typename Node >
			auto
			nodeVar( Node* node )
				-> decltype( node )
			{
				return node;
			}

			template< typename Node >
			auto
			nodeVarRef( typename std::remove_reference< Node >::type& node )
				-> decltype( std::ref( node ) )
			{
				return std::ref( node );
			}

			template< typename Node >
			auto
			nodeVarRef( typename std::remove_reference< Node >::type&& node )
				-> decltype( std::move( node ) )
			{
				return std::move( node );
			}

			template< typename Node >
			auto
			nodeVar( Node&& node )
				-> decltype( nodeVarRef< Node >( node ) )
			{
				return nodeVarRef< Node >( node );
			}


			template< typename Node >
			struct NodeArg;

			template< typename Node >
			struct NodeArg< Node* >
			{
				using r = Node*;
			};

			template< typename Node >
			struct NodeArg< std::reference_wrapper< Node > >
			{
				using r = Node&;
			};

			template< typename Node >
			struct NodeArg
			{
				using r = Node&;
			};


			template< typename ParserProvider, template< typename > class Element >
			struct ProcessElement;


			template< typename ParserProvider, typename Attributes, typename ProcessMethod >
			struct
			TryCallProcessSelf;

			template< typename ParserProvider, typename Attributes >
			struct
			TryCallProcessSelf< ParserProvider, Attributes, common::NullType >
			{
				template< typename Node, typename BoundElement >
				static
				std::tuple<>
				call( BoundElement*, Node )
				{
					return std::make_tuple();
				}
			};

			template< typename ParserProvider, typename Attributes, typename Return >
			struct CallProcessSelf;

			template< typename ParserProvider, typename... Attribute >
			struct CallProcessSelf< ParserProvider, annotation::Attributes< Attribute... >, void >
			{
				template< typename Node, typename BoundElement >
				static
				std::tuple<>
				call( BoundElement* element, Node node )
				{
					using nodeTool = typename ParserProvider::NodeTool;

					BoundElement::Element::ProcessMethod::call(
							element,
							nodeTool( node ).getAttributeValue( Attribute::toString() )... );
					
					return std::make_tuple();
				}
			};

			template< typename ParserProvider, typename... Attribute, typename Return >
			struct CallProcessSelf< ParserProvider, annotation::Attributes< Attribute... >, Return >
			{
				template< typename Node, typename BoundElement >
				static
				std::tuple< Return >
				call( BoundElement* element, Node node )
				{
					using returnTuple = std::tuple< Return >;
					using nodeTool = typename ParserProvider::NodeTool;

					return returnTuple(
								BoundElement::Element::ProcessMethod::call(
									element,
									nodeTool( node ).getAttributeValue( Attribute::toString() )... ) );
				}
			};

			template< typename ParserProvider, typename Attributes, typename ProcessMethod >
			struct
			TryCallProcessSelf
			{
				template< typename Node, typename BoundElement >
				static
				auto
				call( BoundElement* element, Node node )
					-> decltype( CallProcessSelf<
										ParserProvider,
										Attributes,
										typename ProcessMethod::Return >
												::template call< Node >( element, node ) )
				{
					return CallProcessSelf<
								ParserProvider,
								Attributes,
								typename ProcessMethod::Return >
										::template call< Node >( element, node );
				}
			};

			template< typename ParserProvider, typename Node, typename BoundElement >
			auto
			callProcessSelf( BoundElement* element, Node node )
				-> decltype( TryCallProcessSelf<
								ParserProvider,
								typename BoundElement::Element::Attributes,
								typename BoundElement::Element::ProcessMethod >::template call< Node >( element, node ) )
			{
				return TryCallProcessSelf<
							ParserProvider,
							typename BoundElement::Element::Attributes,
							typename BoundElement::Element::ProcessMethod >::template call< Node >( element, node );
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


			template< typename ParserProvider, typename ChildElements >
			struct
			ProcessChildren;

			template< typename ParserProvider >
			struct
			ProcessChildren< ParserProvider, TypeList<> >
			{
				template< typename Node, typename Contextual, typename BoundElement, typename SelfResult >
				static
				void
				process( Contextual* ctx, BoundElement* element, SelfResult& selfResult, Node node )
				{
					// silently ignore undefined element
				}
			};

			template< typename ParserProvider, template< typename > class ChildElementImpl, typename ProcessMethod, typename... Rest >
			struct
			ProcessChildren<
				ParserProvider,
				TypeList<
					annotation::ChildElement< ChildElementImpl, ProcessMethod >,
					Rest... > >
			{
				template< typename Node, typename Contextual, typename BoundElement, typename SelfResult >
				static
				void
				process( Contextual* ctx, BoundElement* element, SelfResult& selfResult, Node node )
				{
					using nodeTool = typename ParserProvider::NodeTool;

					if( nodeTool( node ).getNodeType() == XmlNodeType::ElementNode &&
						decltype( bind< ChildElementImpl >( ctx ) )::Element::Name::toString() == nodeTool( node ).getNodeName() )
					{
						callProcessChild< ProcessMethod >(
								element,
								selfResult,
								ProcessElement< ParserProvider, ChildElementImpl >
										::template process< Node >( ctx, node ) );
					}
					else
					{
						ProcessChildren<
								ParserProvider,
								TypeList< Rest... > >
										::template process< Node >( ctx, element, selfResult, node );
					}
				}
			};

			template< typename ParserProvider, template< typename > class ChildElementImpl, typename... Rest >
			struct
			ProcessChildren<
				ParserProvider,
				TypeList<
					annotation::ChildElement< ChildElementImpl, common::NullType >,
					Rest... > >
			{
				template< typename Node, typename Contextual, typename BoundElement, typename SelfResult >
				static
				void
				process( Contextual* ctx, BoundElement* element, SelfResult& selfResult, Node node )
				{
					using nodeTool = typename ParserProvider::NodeTool;

					if( nodeTool( node ).getNodeType() == XmlNodeType::ElementNode &&
						decltype( bind< ChildElementImpl >( ctx ) )::Element::Name::toString() == nodeTool( node ).getNodeName() )
					{
						ProcessElement<
								ParserProvider,
								ChildElementImpl >
										::template process< Node >( ctx, node );
					}
					else
					{
						ProcessChildren<
								ParserProvider,
								TypeList< Rest... > >
										::template process< Node >( ctx, element, selfResult, node );
					}
				}
			};

			template< typename ParserProvider, typename ProcessMethod, typename... Rest >
			struct
			ProcessChildren<
				ParserProvider,
				TypeList<
					annotation::DataNode< ProcessMethod >,
					Rest... > >
			{
				template< typename Node, typename Contextual, typename BoundElement, typename SelfResult >
				static
				void
				process( Contextual* ctx, BoundElement* element, SelfResult& selfResult, Node node )
				{
					using nodeTool = typename ParserProvider::NodeTool;

					if( nodeTool( node ).getNodeType() == XmlNodeType::DataNode )
					{
						callProcessChild< ProcessMethod >(
								element,
								selfResult,
								std::make_tuple( nodeTool( node ).getNodeValue() ) );
					}
					else
					{
						ProcessChildren<
								ParserProvider,
								TypeList< Rest... > >
										::template process< Node >( ctx, element, selfResult, node );
					}
				}
			};


			template< typename ParserProvider, template< typename > class Element >
			struct ProcessElement
			{
				template< typename Node, typename Contextual >
				static
				auto
				process( Contextual* ctx, Node node )
					-> decltype( callProcessSelf< ParserProvider, Node >( create< Element >( ctx ).get(), node ) )
				{
					auto	element = create< Element >( ctx );
					using	boundElement = typename decltype( element )::element_type;
					auto	selfResult = callProcessSelf< ParserProvider, Node >( element.get(), node );

					using	nodeTool = typename ParserProvider::NodeTool;
					auto	childNode = nodeVar( nodeTool( node ).getFirstChildNode() );
					using	childNodeArgType = typename xmlReader::NodeArg< decltype( childNode ) >::r;
					while( nodeTool( childNode ).isValid() )
					{
						ProcessChildren<
								ParserProvider,
								typename boundElement::Element::ChildElements >
										::template process< childNodeArgType >( ctx, element.get(), selfResult, childNode );

						childNode = nodeVar( nodeTool( childNode ).getNextSibling() );
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

			using ParserProvider = typename This::template FindAnnotated< annotation::IsXmlParserProvider >::r::Head::r::XmlParserProvider;
		

		public:

			void
			read()
			{
				auto	parser = create< ParserProvider::template XmlParser >( this );
				using	rootElement = typename This::template FindAnnotatedContextual< annotation::IsRootElement >::r::Head::r;
				
				auto	rootNode = xmlReader::nodeVar( parser->getRootNode() );
				using	nodeArgType = typename xmlReader::NodeArg< decltype( rootNode ) >::r;
				xmlReader::ProcessElement<
						ParserProvider,
						rootElement::template Contextual >
								::template process< nodeArgType >( this, rootNode );
			}

		};
		
	}
	
}