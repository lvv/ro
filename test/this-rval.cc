// t.cpp
#include <iostream>

struct test{
  void f() &{ std::cout << "lvalue object\n"; }
  void f() &&{ std::cout << "rvalue object\n"; }
};

int main(){
  test t;
  t.f(); // lvalue
  test().f(); // rvalue
}
