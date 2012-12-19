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
struct  plus1   {};
struct  plus2   {};
struct  minus1  {};
	
	
	template<int N>
struct  ph_wrap{ enum {value=N}; };

//////////////////////////////////////////////////////////////   FUNCTOR_T

	// primary
	template<class Op, class Opnd1, class Opnd2>
struct  functor_t;


	//  conv Ph -> Fr
	template<int N>
struct  functor_t <void,ph_wrap<N>,void> {
	typedef void is_functor;
	enum { value=N, max_arity=N };

		template<class Arg>
		eIF<N==1 && !is_tuple<Arg>::value, Arg>
	operator() (Arg arg) { return arg; }

		template<class Arg>
		eIF<is_tuple<Arg>::value, typename std::tuple_element<N,Arg>::type >
	operator() (Arg arg) { return std::get<N>(arg); }
 };


	//  +Fr 
	template<class Fr>
struct  functor_t <plus1,Fr,void> {
	typedef void is_functor;
	functor_t(Fr fr) : fr(fr) {};
	Fr fr;
	template<class Arg>
	Arg operator() (Arg arg) { return +fr(arg); }
 };


	//  -Fr 
	template<class Fr>
struct  functor_t <minus1,Fr,void> {
	typedef void is_functor;
	functor_t(Fr fr) : fr(fr) {};
	Fr fr;
	template<class Arg>
	Arg operator() (Arg arg) { return -fr(arg); }
 };

	//  Fr + Fr
	template<class Fr1, class Fr2>
struct  functor_t <plus2,Fr1,Fr2> {
	typedef void is_functor;
	functor_t(Fr1 fr1, Fr2 fr2) : fr1(fr1), fr2(fr2) {};
	Fr1 fr1;
	Fr2 fr2;
		// Arity=2
		template<class Arg1, class Arg2>
		Arg1 
	operator() (Arg1 arg1, Arg2 arg2) { return fr1(arg1) + fr2(arg2); }

		// Arity=1
		template<class Arg>
		eIF<!is_tuple<Arg>::value, Arg>
	operator() (Arg arg) { return fr1(arg) + fr2(arg); }

		// Tuple
		template<class Arg> auto
	operator() (Arg arg) 
	-> eIF<is_tuple<Arg>::value, decltype(fr1(arg) + fr2(arg))>
	{ return fr1(arg) + fr2(arg); }

 };


//////////////////////////////////////////////////////////////   OPERATORS
	
//// unary+

	// + Fr
	template<class Fr, class=typename Fr::is_functor>
	functor_t<plus1,Fr,void>
operator+(Fr fr) {
	return  functor_t<plus1,Fr,void>(fr);
 }

	// + Ph 
	template<class Ph, int N=std::is_placeholder<Ph>::value>
	eIF<N, functor_t<plus1,functor_t<void,ph_wrap<N>,void>,void>>
operator+(Ph ph) {
	return  + functor_t<void, ph_wrap<N>,void>();
 }

//// unary-

	// - Fr
	template<class Fr, class=typename Fr::is_functor>
	functor_t<minus1,Fr,void>
operator-(Fr fr) {
	return  functor_t<minus1,Fr,void>(fr);
 }

	// - Ph 
	template<class Ph, int N=std::is_placeholder<Ph>::value>
	eIF<N, functor_t<minus1,functor_t<void,ph_wrap<N>,void>,void>>
operator-(Ph ph) {
	return  - functor_t<void, ph_wrap<N>,void>();
 }

//// binary+


	// Fr + Fr
	template<class Fr1, class Fr2, class=typename Fr1::is_functor, class=typename Fr2::is_functor>
	functor_t<plus2,Fr1,Fr2>
operator+(Fr1 fr1, Fr2 fr2) {
	return  functor_t<plus2,Fr1,Fr2>(fr1,fr2);
 }

	// Ph1 + Ph2
	template<class Ph1, class Ph2, int N1=std::is_placeholder<Ph1>::value, int N2=std::is_placeholder<Ph2>::value>
	eIF<N1*N2, functor_t<plus2,functor_t<void,ph_wrap<N1>,void>, functor_t<void,ph_wrap<N2>,void> >>
operator+(Ph1 ph1, Ph2 ph2) {
	return  functor_t<void, ph_wrap<N1>,void>() + functor_t<void, ph_wrap<N2>,void>();
 }


				};	// namespace sto
				#endif	// STO_LAMBDA_H
