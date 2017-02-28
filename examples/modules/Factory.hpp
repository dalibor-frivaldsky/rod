// Copyright Dalibor Frivaldsky 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#pragma once

#include <memory>

#include "Object.hpp"


namespace example {

	class Factory {
	public:
		virtual std::unique_ptr< Object >	createObject() const = 0;
	};

}
