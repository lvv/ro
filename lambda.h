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
	
	
	template<int N>
struct  ph_wrap{ enum {value=N}; };

//////////////////////////////////////////////////////////////   FUNCTOR_T

// primary
	template<class Op, class Opnd1, class Opnd2>
struct  functor_t;


// +Ph
	template<int N>
struct  functor_t <void,ph_wrap<N>,void> {
	typedef void is_functor;
	enum { value=N };
	template<class Arg>
	Arg operator() (Arg arg) { return arg; }
 };

// +Fr 
	template<class Fr>
struct  functor_t <plus1,Fr,void> {
	typedef void is_functor;
	functor_t(Fr fr) : fr(fr) {};
	Fr fr;
	template<class Arg>
	Arg operator() (Arg arg) { return +fr(arg); }
 };


//////////////////////////////////////////////////////////////   OPERATORS
	
	// + Ph 
	template<class Ph, int N=std::is_placeholder<Ph>::value>
	eIF<N, functor_t<plus1,functor_t<void,ph_wrap<N>,void>,void>>
operator+(Ph ph) {
	return  functor_t<plus1, functor_t<void, ph_wrap<N>,void>,void>(
	   	functor_t<void, ph_wrap<N>,void>() 
	);
 }

	// + Fr
	template<class Fr, class=typename Fr::is_functor>
	functor_t<plus1,Fr,void>
operator+(Fr fr) {
	return  functor_t<plus1,Fr,void>(fr);
 }





				};	// namespace sto


				#endif	// STO_LAMBDA_H
