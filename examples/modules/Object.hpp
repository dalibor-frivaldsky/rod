// Copyright Dalibor Frivaldsky 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#pragma once

#include <iostream>


namespace example {

	class Object {
	public:
		Object() {
			std::cout << "Object constructor" << std::endl;
		}

		virtual ~Object() {
			std::cout << "Object destructor" << std::endl;
		}
	};

}
