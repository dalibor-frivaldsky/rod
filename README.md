# What is rod?
Rod is a header-only, lightweight feature resolution library for C++. In order to explain this, let's look at a short snippet of code:
```C++
namespace q = rod::feature::query;
auto [someTypePtr] = someObject | rod::resolve(q::instace{ q::as< SomeType* > }) | rod::get;
```
The code above could read something like this: Do you, `someObject`, have a feature that allows me to obtain a pointer to an instance of `SomeType`? If you do, give me the means to utilize the feature and obtain such pointer.

## rod::resolve ##
`rod::resolve` is the core function (function object) of the `rod` library. It provides the means to query any resolve-enabled object for features. We call any resolve-enabled object a `resolve source`. The `rod::resolve` function object takes zero or more `feature queries` and returns a range of features that match those queries. The type of the returned range depends on the implementation that enabled the queried object to be used as a `resolve source`.

The returning of a range is the reason why we call `rod::get` at the end of the previous example. It returns the first matched feature from the range. The need for de-structuring the result to obtain the final value is due to the fact that matched features are represented as `std::tuple` objects (more on that later).

## Resolving boost::hana::tuple ##
`rod` comes with out-of-the box support for resolving instances and types from `boost::hana::tuple` objects (to be more precise, from any object modeling the `Monad` concept). Type of the returned range of matched features is again a `Monad`.

### Primitive values ###
Let's start by creating a tuple of primitive values - int, float and double:
```C++
namespace hana = boost::hana;
auto primitives = hana::tuple{ 10, 5.0f, 2.5 };
```
To obtain an int value from the tuple, we can write
```C++
auto [i] = primitives | rod::resolve(q::instance{ q::as< int > }) | rod::get;
assert(i == 10);
```
This will create a copy of the int value `10` from the tuple. Obtaining a pointer to one of the values in the tuple works similarly:
```C++
auto [fPtr] = primitives | rod::resolve(q::instance{ q::as< float* > }) | rod::get;
assert(*fPtr == 5.0f);
```
Obtaining references works as well:
```C++
auto [dRef] = primitives | rod::resolve(q::instance{ q::as< double& > }) | rod::get;
assert(dRef == 2.5);
```

### Class hierarchies ###
Pointers and references to the base types of class hierarchies can be resolved as well. Assuming a base class `Base` and two derived classes `DerivedA` and `DerivedB`:
```C++
auto hierachy = hana::tuple{ DerivedA{}, DerivedB{} };
auto [baseRef] = hierachy | rod::resolve(q::instance{ q::as< Base& > }) | rod::get;
```
Since there are two features that provide instance of `Base&`, `rod::get` will obtain only the first one from the returned range - in this case reference to `DerivedA` instance. We can take advantage of the fact the the returned range is a `Monad` and use the tools provided by the `boost::hana` library to utilize all the features. `rod::get_all` will obtain all the features from the range:
```C++
hana::for_each(
    hierarchy | rod::resolve(q::instance{ q::as< Base& > }) | rod::get_all,
    [&] (auto&& f) {
        auto [baseRef] = f;
        // do something
    });
```

### Resolving types ###
`boost::hana::tuple` can be also queried for types. Any contained value within the tuple also provides information about its type:
```C++
auto [derivedType] = hierarchy | rod::resolve(q::type{ q::of_base< Base > }) | rod::get;
BOOST_HANA_CONSTANT_CHECK(derivedType == hana::type_c< DerivedA >);
```
If the `boost::hana::tuple` contains values of `boost::hana::type_c<?>`, these are also used to provide answers to type feature queries:
```C++
auto types = hana::tuple{ hana::type_c< Base > };
auto [baseTypeOfDerivedB] = types | rod::resolve(q::type{ q::base_of< DerivedB > }) | rod::get;
BOOST_HANA_CONSTANT_CHECK(baseTypeOfDerivedB == hana::type_c< Base >);
```
To see more examples (also utilizing both instance and type feature queries), see the [resolve API functional test](test/functional/rod/resolve/resolve.api.spec.cpp).

## rod::with ##
The `rod::resolve` function object is a fairly low-level tool. But being, together with the feature queries, a core concept of the `rod` library, more user-friendly tools can be built on top of it. One such tool is `rod::with`. It takes a lambda, determines the types of the  expected arguments, resolves instances matching those arguments and calls the lambda with them:
```C++
primitives | rod::with([] (int a, float& b, double* c) {
    assert(a == 10);
    assert(b == 5.0f);
    assert(*c == 2.5);
});
```

## rod::erase and type-erased resolving ##
There is a second `resolve source` provided by the `rod` library - `rod::source::type_index`. It's purpose is to enable type-erased, but still type-safe resolving of instance features. We can create such source using `rod::erase` tool on a `boost::hana::tuple` resolve source, like this:
```C++
rod::source::type_index erasedSource = primitives | rod::erase;
```
The type-erased source can be used the same way as `boost::hana::tuple` to resolve any instance features that can be resolved from the tuple:
```C++
auto [dRef] = erasedSource | rod::resolve(q::instance{ q::as< double& > }) | rod::get;
erasedSource | rod::with([] (int a, float& b, double* c) {
    assert(a == 10);
    assert(b == 5.0f);
    assert(*c == 2.5);
});
```
There are two caveats. Since the types have been erased, resolving type features is not supported. Also due to type erasure, to support resolving instances of type hierarchies, the `resolve source` being erased needs to provide type features of any resolvable base types:
```C++
auto hierarchyWithBaseTypes = hana::tuple{ DerivedA{}, DerivedB{}, hana::type_c< Base > };
auto erasedHierarchy = hierarchyWithBaseTypes | rod::erase;

auto [baseRef] = erasedHierarchy | rod::resolve(q::instance{ q::as< Base& > }) | rod::get;
```
It also needs to be noted that the range returned from resolving the `rod::source::type_index` is not of boost::hana's `Monad` concept, but a view range from the `range-v3` library.

### Modules example ###
The type-erased `resolve source` can be used to pass it around your code base without making your functions into function templates (which would be needed when passing around `boost::hana::tuple` template). Another example is modularizing an application into modules/plugins in form of shared libraries. Such modules can expose single function returning the type-erased `rod::source::type_index` source and the application can query for various features provided by the modules. See the [modules](examples/modules) example for further info. The code is commented to provide additional information.

## Creating new `resolve source`s ##
It is possible to transform any type into a `resolve source`. We will not go into details here, but you can check the [q-model-index](examples/q-model-index) example to learn how to do so. The code is commented to guide you with the process.

# Dependencies #
  - C++ compiler supporting most of the C++17 features and concepts-lite (at the moment of writing, only trunk GCC with `-std=c++17 -fconcepts`)
  - [Boost Hana](https://github.com/boostorg/hana)
  - [range-v3](https://github.com/ericniebler/range-v3)
  - [CallableTraits](https://github.com/badair/callable_traits)
  - [Catch](https://github.com/philsquared/Catch) - for the tests
  
# License #
The project is licensed under the [Boost software license](http://www.boost.org/users/license.html)
