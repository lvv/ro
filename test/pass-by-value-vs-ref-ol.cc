#include <memory>
#include <iostream>
#include <type_traits>

struct A
{
    A() { std::cout << "A Default constructor\n"; }
    A(const A &) { std::cout << "A Copy\n"; }
    A(A &&) { std::cout << "A Move\n"; }
};

std::aligned_storage<sizeof(A)> contents;
A& alias = *reinterpret_cast<A*>(&contents);

void ByVal(A a)
{
    new (&contents) A(std::move(a));
    alias.~A();
}

void ByLCRef(A const& a)
{
    new (&contents) A(a);
    alias.~A();
}

void ByRRef(A&& a)
{
    new (&contents) A(std::move(a));
    alias.~A();
}

int main()
{
    A a;
    std::cout << "\n";
    std::cout << "ByVal(a);\n";
    ByVal(a);
    std::cout << "ByVal(std::move(a));\n";
    ByVal(std::move(a));
    std::cout << "ByVal(A());\n";
    ByVal(A());
    std::cout << "ByLCRef(a);\n";
    ByLCRef(a);
    std::cout << "ByRRef(std::move(a));\n";
    ByRRef(std::move(a));
    std::cout << "ByRRef(A());\n";
    ByRRef(A());
}
