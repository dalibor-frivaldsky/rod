// Copyright Dalibor Frivaldsky 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#pragma once

#include <QtCore/QAbstractItemModel>
#include <rod/api/all>
#include <vector>

#include "MyIndex.hpp"


namespace example {

	struct StringData {
		std::string		str;
	};


	/* A standard implementation of the QAbstractItemModel interface class.
	 * The 'data' method understands two user-defined data role types.
	 * The additional functionality is provided by the 'myIndex' method,
	 * which acts similarly as 'index' method, but returns our resolve-enabled
	 * MyIndex instance instead. */
	class MyModel: public QAbstractItemModel {

	private:
		struct RowData {
			int			i;
			StringData	stringData;
		};

		std::vector< RowData >	modelData;

	public:

		enum Role {
			IntRole = Qt::UserRole,
			StringDataRole
		};

		MyModel();

		virtual QModelIndex	index(int row, int col, const QModelIndex& parent = QModelIndex()) const;
		virtual QModelIndex	parent(const QModelIndex&) const;
		virtual int			rowCount(const QModelIndex& parent = QModelIndex()) const;
		virtual int			columnCount(const QModelIndex& parent = QModelIndex()) const;
		virtual QVariant	data(const QModelIndex& index, int role) const;

		MyIndex	myIndex(int row, int col) const;

	};


	/* Let's assume we are going to use MyModel to retrieve only the int value or
	 * StringData value of rows using the user defined data roles. We can transform
	 * MyIndex instances into resolve sources by providing implementation of the
	 * 'query_entities' function. It takes two arguments - 1. instance of the resolve
	 * source type (MyIndex in our case) and 2. parameter pack of feature queries.
	 * For our simple use case, we can ignore the passed-in feature queries altogether.
	 * Our goal is to return an instance of an object for which there is an implementation
	 * of 'filter_entities' function. We will return Hana tuple of rod::source::build::entities,
	 * for which there is such a function implemented directly in the rod library.
	 * The 'query_entities' function needs to be implemented in the same namespace as
	 * the type we want to enable rod::resolve for. */
	template< typename... Qs >
	auto query_entities(MyIndex index, Qs&&...) {
		using namespace rod::source::build;

		return boost::hana::tuple{
			/* Entity is any type that implements operator (), which takes one
			 * argument - instance of a feature tag. The return type of the operator
			 * is expected to be a Resolver function object. It is expected for an
			 * entity to implement multiple overload of the () operator to provide
			 * Resolver function objects for multple features.
			 *
			 * For our example, we are going to use tools from the rod::source::build
			 * namespace, which allows us to create entities with simple and
			 * straightforward syntax.
			 *
			 * We want to resolve int and StringData types from MyIndex instances.
			 * We thus create two entities, both of which will provide Resolver for
			 * the instance feature (represented by instance_tag). In each respective
			 * Resolver (a lambda function in our case), we query the model pointed to
			 * from the QModelIndex with the provided index and respective data role. */
			entity{
				feature_provider{
					rod::feature::instance_tag{},
					[=] {
						return index.model()->data(index, MyModel::StringDataRole)
							.value< StringData >();
					}
				}
			},
			entity{
				feature_provider{
					rod::feature::instance_tag{},
					[=] {
						return index.model()->data(index, MyModel::IntRole)
							.value< int >();
					}
				}
			}
		};
	}

}

Q_DECLARE_METATYPE(example::StringData)
