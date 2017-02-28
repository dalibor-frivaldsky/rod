// Copyright Dalibor Frivaldsky 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#include <dlfcn.h>
#include <iostream>
#include <rod/api/all>

#include "Factory.hpp"
#include "Service.hpp"


rod::source::type_index* loadModule(const char* moduleName) {
	void* moduleHandle = dlopen(moduleName, RTLD_LAZY);
	
	return ((rod::source::type_index* (*)())dlsym(moduleHandle, "getModule"))();
}

int main() {
	/* We expect each module to export a function "getModule"
	 * taking zero arguments and returning a pointer to a type-erased
	 * resolve source. We load these modules and obtain the pointer. */
	rod::source::type_index* module1 = loadModule("./libmodule1.so");
	rod::source::type_index* module2 = loadModule("./libmodule2.so");

	/* The type erased source provides the same type-safe resolving
	 * facilities as a Hana tuple. We can thus resolve instance features
	 * from the modules. This is demonstrated by obtaining
	 * a reference to implementations of the Service interface
	 * from the loaded modules and calling a method of the interface. */
	int sum = 0;
	for(auto module: {module1, module2}) {
		(*module) | rod::with([&] (example::Service& service) {
			sum += service.method();
		});
	}

	/* We expect 3 to be written to the std output. */
	std::cout << sum << std::endl;

	/* Demonstration of object lifetime created by a Factory implementation
	 * in one of the modules. Should print messages to std output from
	 * Object constructor, LoggedLifetimeObject constructor and destructor
	 * and Object destructor. */
	(*module2) | rod::with([] (example::Factory& factory) {
		auto o = factory.createObject();
	});

	return 0;
}
