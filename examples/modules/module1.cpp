// Copyright Dalibor Frivaldsky 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#include <rod/api/all>

#include "Service.hpp"


namespace example {

	class Module1Service: public Service {
	public:
		virtual int method() const {
			return 1;
		}
	};

}


extern "C" rod::source::type_index* getModule()
{
	/* We create a tuple containing instance of Service interface implementation
	 * and a Hana type of the Service interface class. This is needed to enable
	 * base class resolving of a type-erased resolve source. */
	static auto moduleServices = boost::hana::tuple{
		example::Module1Service{},
		boost::hana::type_c< example::Service >
	};

	/* We create a type-erased resolve source from the Hana tuple resolve source. */
	static rod::source::type_index module = moduleServices | rod::erase;

	return &module;
}
