// Copyright Dalibor Frivaldsky 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#include <rod/api/all>

#include "Factory.hpp"
#include "Object.hpp"
#include "Service.hpp"


namespace example {

	class Module2Service: public Service {
	public:
		virtual int method() const {
			return 2;
		}
	};

	class LoggedLifetimeObject: public Object {
	public:
		LoggedLifetimeObject():
			Object()
		{
			std::cout << "LoggedLifetimeObject constructor" << std::endl;
		}

		~LoggedLifetimeObject() {
			std::cout << "LoggedLifetimeObject destructor" << std::endl;
		}
	};

	class LoggedLifetimeObjectFactory: public Factory {
	public:
		virtual std::unique_ptr< Object >	createObject() const {
			return std::make_unique< LoggedLifetimeObject >();
		}
	};

}


extern "C" rod::source::type_index* getModule()
{
	/* We create a tuple containing instance of Service interface implementation
	 * and a Hana type of the Service interface class. This is needed to enable
	 * base class resolving of a type-erased resolve source. We do the same thing
	 * for the LoggedLifetimeObjectFactory and Factory classes. */
	static auto moduleServices = boost::hana::tuple{
		example::Module2Service{},
		boost::hana::type_c< example::Service >,
		example::LoggedLifetimeObjectFactory{},
		boost::hana::type_c< example::Factory >
	};

	/* We create a type-erased resolve source from the Hana tuple resolve source. */
	static rod::source::type_index module = moduleServices | rod::erase;

	return &module;
}
