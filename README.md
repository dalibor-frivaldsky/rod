# What is rod?
Rod is a header-only, lightweight feature resolution library for C++. In order to explain this, let's look at a short snippet of code and disect it.
```
namespace q = rod::feature::query;
auto result = someObject | rod::resolve(q::instace{ q::as< SomeType& > });
```
The code above could read something like this: Do you, `someObject`, have a feature that allows me to obtain a reference to an instance of `SomeType`?
If you do, give me the means to utilize the feature and obtain such reference.
