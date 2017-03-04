# What is rod?
Rod is a header-only, lightweight feature resolution library for C++. In order to explain this, let's look at a short snippet of code and disect it.
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
auto [fPtr] = tuple | rod::resolve(q::instance{ q::as< float* > }) | rod::get;
assert(*fPtr == 5.0f);
```
Obtaining references works as well:
```C++
auto [dRef] = tuple | rod::resolve(q::instance{ q::as< double& > }) | rod::get;
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
