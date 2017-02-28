// Copyright Dalibor Frivaldsky 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#include <iostream>

#include "MyModel.hpp"


int main() {
	/* Let's utilize MyIndex and our custom implementation of the 'query_entities'.
	 * We will show alternative ways to query data from a MyModel instance. */
	example::MyModel	model;

	/* The traditional way to retrieve data from a model is to obtain an index,
	 * pass it to the 'data' method of the model along with a data role and then
	 * type-safely retrieve instance of the expected type from the returned variant. */
	std::string str = model.data(model.index(0, 0), example::MyModel::StringDataRole)
		.value< example::StringData >().str;
	std::cout << str << std::endl;

	/* Since we've enabled MyIndex instances to act as resolve sources, we can use
	 * the rod's 'resolve' function to query features from them. This function is
	 * a bit low level, as we can see. We need to construct a proper instance query,
	 * retrieve the value provided by a Resolver function object (using rod::get) and
	 * de-structuring the result into a variable ('resolve' mechanism returns std::tuple,
	 * elements of which are resolvers/instance for each of the queried feature). */
	namespace q = rod::feature::query;
	auto	[stringData] = model.myIndex(0, 0)
		| rod::resolve(q::instance{ q::as< example::StringData > }) | rod::get;
	std::cout << stringData.str << std::endl;

	/* Rod provide some light-weight tools built on top of the 'resolve' function
	 * that simplify frequent use-cases. The rod::with tool retrieves instances
	 * from a resolve source based on the parameter types of the passed-in
	 * lambda function. */
	model.myIndex(0, 0) | rod::with([] (example::StringData stringData) {
		std::cout << stringData.str << std::endl;
	});

	/* The next two snippets provide direct comparison between standard model
	 * data querying and instance resolving with rod. The direct obvious benefits
	 * are a little shorter code (though arguably by not that much) and little higher
	 * maintainability (e.g. if a data role name changes, you need to modify just the
	 * 'query_entities' function). The less obvious benefits are that we've provided
	 * a universal way to work with data of a model. We will be able to use any tools
	 * built on top of instance feature resolving also with MyIndex instances.
	 * rod::with is one such tool. */
	int sum = 0;
	for(int i = 0; i < model.rowCount(); ++i) {
		sum += model.data(model.index(i, 0), example::MyModel::IntRole).value< int >();
	}
	std::cout << sum << std::endl;

	int	sum2 = 0;
	for(int i = 0; i < model.rowCount(); ++i) {
		model.myIndex(i, 0) | rod::with([&] (int i) { sum2 += i; });
	}
	std::cout << sum2 << std::endl;

	/* To further enhance the last statement of the previous part, lets type-erase an
	 * instance of MyIndex and retrieve a StringData instance from the type-erased source.
	 * The practical aspect of doing this is arguable, but it shows that rod::erase is another
	 * tool than can now use our custom MyIndex resolve source.*/
	example::MyIndex	row1Index = model.myIndex(1, 0);
	rod::source::type_index erasedIndex = row1Index | rod::erase;
	erasedIndex | rod::with([] (example::StringData stringData) {
		std::cout << stringData.str << std::endl;
	});

	return 0;
}