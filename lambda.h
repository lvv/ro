//  LAMBDA EXPRESSIONS
				#ifndef  STO_LAMBDA_H
				#define  STO_LAMBDA_H


				#include <sto/meta.h>


				namespace sto {

				using	std::placeholders::_1;
				using	std::placeholders::_2;
				using	std::placeholders::_3;
				using	std::placeholders::_4;
				using	std::placeholders::_5;

// -- artihmetic ----------------------

 class plus_action {};
class minus_action {};
class multiply_action {};
class divide_action {};
class remainder_action {};

// -- actioun group templates --------------------

template <class Action> class arithmetic_action;
template <class Action> class bitwise_action;
template <class Action> class logical_action;
template <class Action> class relational_action;
template <class Action> class arithmetic_assignment_action;
template <class Action> class bitwise_assignment_action;
template <class Action> class unary_arithmetic_action;
template <class Action> class pre_increment_decrement_action;
template <class Action> class post_increment_decrement_action;

//-------------------------
struct plus1{};
	
	
	template<class Op, class Opnd1=void, class Opnd2=void>
struct  functor_t;

	template<int N>
struct  ph_wrap{ enum {value=N}; };

	template<int N>
struct  functor_t <ph_wrap<N>> {
	typedef void is_functor;
	enum { value=N };
	template<class Arg>
	Arg operator() (Arg arg) { return arg; }
 };

	template<class F>
struct  functor_t <plus1,F> {
	typedef void is_functor;
	functor_t(F f) : f(f) {};
	F f;
	template<class Arg>
	Arg operator() (Arg arg) { return +f(arg); }
 };

	
	// + Ph 
	template<class Ph, int N=std::is_placeholder<Ph>::value>
	eIF<N, functor_t<plus1,functor_t<ph_wrap<N>>>>
operator+(Ph ph) {
	return  functor_t<plus1, functor_t<ph_wrap<N>>>(
	   	functor_t<ph_wrap<N>>() 
	);
 }

	// + Func
	template<class F, class=typename F::is_functor>
	functor_t<plus1,F>
operator+(F f) {
	return  functor_t<plus1,F>(f);
 }





				};	// namespace sto


				#endif	// STO_LAMBDA_H
