// Copyright Dalibor Frivaldsky 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#pragma once

#include <QtCore/QModelIndex>


namespace example {

	/* We create a custom classes based on the QModelIndex. This will
	 * drive the rod library to select our custom 'query_entities'
	 * function when resolving from MyIndex instances. */
	class MyIndex: public QModelIndex {};

}