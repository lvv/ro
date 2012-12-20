//  LAMBDA EXPRESSIONS
				#ifndef  STO_LAMBDA_H
				#define  STO_LAMBDA_H


				#include <sto/meta.h>


				namespace sto {


struct  plus1   {};
struct  plus2   {};
struct  minus1  {};

	
//////////////////////////////////////////////////////////////   PLACEHOLDER

//	template<int N>
//struct  ph_wrap{ enum {value=N}; };

	template<int N>
struct  placeholder {
	typedef void is_functor;
	typedef void is_placeholder;

		// 1 arg
		template<class Arg>
		eIF<!is_tuple<Arg>::value, Arg>
	operator() (Arg arg) { static_assert(N==1, "bad placeholder number"); return arg; }

		// 2 arg, N==1
		template<class Arg1, class Arg2>
		//eIF<(sizeof(Arg1),N==1), Arg1>			// this dosn't work, gcc bug
		typename std::enable_if<(sizeof(Arg1),N==1), Arg1>::type
	operator() (Arg1 arg1, Arg2 arg2) { return arg1; }

		// 2 arg, N==2
		template<class Arg1, class Arg2>
		//eIF<(sizeof(Arg1), N==2), Arg2>			// this dosn't work, gcc bug
		typename std::enable_if<(sizeof(Arg1),N==2), Arg2>::type
	operator() (Arg1 arg1, Arg2 arg2) { return arg2; }

		// tuple
		template<class Arg>
		eIF<is_tuple<Arg>::value, typename std::tuple_element<N,Arg>::type >
	operator() (Arg arg) { return std::get<N>(arg); }
};

placeholder<0>	_0;
placeholder<1>	_1;
placeholder<2>	_2;
placeholder<3>	_3;

template<class T> 	struct 	is_ph	 				: std::false_type  {};
template<int N> 	struct 	is_ph<placeholder<N>>			: std::true_type  {};

//////////////////////////////////////////////////////////////   FUNCTOR_T

	// primary
	template<class Op, class Opnd1, class Opnd2>
struct  functor_t;

/*
	//  conv Ph -> Fr
	template<int N>
struct  functor_t <void,ph_wrap<N>,void> {
	typedef void is_functor;
	enum { value=1 };

		// 1 arg
		template<class Arg>
		eIF<!is_tuple<Arg>::value, Arg>
	operator() (Arg arg) { static_assert(N==1, "bad placeholder number"); return arg; }

		// 2 arg, N==1
		template<class Arg1, class Arg2>
		//eIF<(sizeof(Arg1),N==1), Arg1>			// this dosn't work, gcc bug
		typename std::enable_if<(sizeof(Arg1),N==1), Arg1>::type
	operator() (Arg1 arg1, Arg2 arg2) { return arg1; }

		// 2 arg, N==2
		template<class Arg1, class Arg2>
		//eIF<(sizeof(Arg1), N==2), Arg2>			// this dosn't work, gcc bug
		typename std::enable_if<(sizeof(Arg1),N==2), Arg2>::type
	operator() (Arg1 arg1, Arg2 arg2) { return arg2; }

		// tuple
		template<class Arg>
		eIF<is_tuple<Arg>::value, typename std::tuple_element<N,Arg>::type >
	operator() (Arg arg) { return std::get<N>(arg); }
 };
*/

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
		//  Arity==2
		template<class Arg1, class Arg2>
		Arg1 
	operator() (Arg1 arg1, Arg2 arg2) { return fr1(arg1,arg2) + fr2(arg1,arg2); }

		//  Arity==1
		template<class Arg>
		eIF<!is_tuple<Arg>::value, Arg>
	operator() (Arg arg) { return fr1(arg) + fr2(arg); }

		//  Tuple
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
	eIF<N, functor_t<plus1,placeholder<N>,void>>
operator+(Ph ph) {
	return  + placeholder<N>();
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
	eIF<N, functor_t<minus1,placeholder<N>,void>>
operator-(Ph ph) {
	return  - placeholder<N>();
 }


//// binary+

	// Fr + Fr
	template<class Fr1, class Fr2, class d1=typename Fr1::is_functor, class d2=typename Fr2::is_functor>
	functor_t<plus2,Fr1,Fr2>
operator+(Fr1 fr1, Fr2 fr2) {
	return  functor_t<plus2,Fr1,Fr2>(fr1,fr2);
 }

	// Ph1 + Ph2
	template<class Ph1, class Ph2, int N1=std::is_placeholder<Ph1>::value, int N2=std::is_placeholder<Ph2>::value>
	eIF< N1 && N2, functor_t<plus2,placeholder<N1>, placeholder<N2> >>
operator+(Ph1 ph1, Ph2 ph2) {
	return  placeholder<N1>() + placeholder<N2>();
 }


				};	// namespace sto
				#endif	// STO_LAMBDA_H
