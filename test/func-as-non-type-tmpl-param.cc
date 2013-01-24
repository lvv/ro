#include <iostream>
using namespace std;


struct  plus1_op{
	template <class Fr>
	static auto eval (Fr&& fr) -> decltype(+fr) { return +fr; };
};

struct  plus_op{
	template <class Fr1, class Fr2>
	static auto eval (Fr1&& fr1, Fr2&& fr2) -> decltype(fr1+fr2) { return fr1+fr2; };
};

template <class Op,  class Fr1, class Fr2=void*>
struct fr_t {
	Fr1 fr1;
	Fr2 fr2;
	template<class Arg>               auto operator() (Arg&& arg)                 -> decltype(Op::eval(std::forward<Arg>(arg)))                                { return  Op::eval(arg); }
	template<class Arg1, class Arg2>  auto operator() (Arg1&& arg1, Arg2&& arg2)  -> decltype(Op::eval(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2)))    { return  Op::eval(arg1,arg2); }
};


int main() {

	fr_t<plus1_op,int> plus1_fr;
	fr_t<plus_op,int>  plus_fr;
	cout << plus1_fr(10) << endl;
	cout << plus_fr(10,10) << endl;

}
