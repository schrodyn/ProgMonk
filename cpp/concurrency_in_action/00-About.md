#About this book

This book is an in-depth guide to the concurrency and multithreading 
facilities from the new C++ Standard, from the basic usage of 
`std::thread`, `std::mutex`, and `std::async`,
to the complexities of atomic operations and the memory model.

##Software requirements

To use the code from this book unchanged, you’ll need a recent C++ 
compiler that supports the new C++11 language features used in the 
examples (see appendix A), and you’ll need a copy of the C++ Standard 
Thread Library.

At the time of writing, `g++` is the only compiler I’m aware of that 
ships with an implementation of the Standard Thread Library. The `g++` 
implementation of the Thread Library was first introduced in a basic 
form in g++ 4.3 and extended in subsequent releases. g++ 4.3 also 
introduced the first support for some of the new C++11 language 
features; more of the new language features are supported in each 
subsequent release. 

The Boost Thread Library provides an API that’s based on the C++11 
Standard Thread Library proposals and is portable to many platforms. 
Most of the examples from the book can be modified to work with the 
Boost Thread Library by judicious replacement of `std::` with `boost::`
and use of the appropriate `#include` directives. There are a few 
facilities that are either not supported (such as `std::async`) or have
different names (such as `boost::unique_future`) in the Boost Thread 
Library.
