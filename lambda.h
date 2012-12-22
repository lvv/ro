//  LAMBDA EXPRESSIONS
				#ifndef  STO_LAMBDA_H
				#define  STO_LAMBDA_H


				#include <sto/meta.h>


					#include <functional>
				namespace sto {


struct  plus1   {};
struct  plus2   {};
struct  minus1  {};
struct  call1   {};

	
//////////////////////////////////////////////////////////////   PLACEHOLDER

	template<int N>
struct  ph {
	typedef void is_functor;
	typedef void is_ph;
	enum {n=N};

	////// 1-ARG

		// non-tuple
		template<class Arg>
		eIF<!is_tuple<Arg>::value, Arg>
	operator() (Arg arg) { static_assert(N==1, "bad placeholder number");  return arg; }

		// tuple
		template<class Arg>
		eIF<is_tuple<Arg>::value, typename std::tuple_element<N,Arg>::type >
	operator() (Arg arg) { return std::get<N>(arg); }

	////// 2-ARG
	
		//  N==1
		template<class Arg1, class Arg2>
		//eIF<(sizeof(Arg1),N==1), Arg1>			// this dosn't work, gcc bug
		typename std::enable_if<(sizeof(Arg1),N==1), Arg1>::type
	operator() (Arg1 arg1, Arg2 arg2) { return arg1; }

		//  N==2
		template<class Arg1, class Arg2>
		//eIF<(sizeof(Arg1), N==2), Arg2>			// this dosn't work, gcc bug
		typename std::enable_if<(sizeof(Arg1),N==2), Arg2>::type
	operator() (Arg1 arg1, Arg2 arg2) { return arg2; }

	operator typename std::_Placeholder<N> const () const { return std::_Placeholder<N>(); }	// non portable(?)

};

ph<0>	_0;
ph<1>	_1;
ph<2>	_2;
ph<3>	_3;

template<class T> 	struct 	is_ph	 			: std::false_type  {};
template<int N> 	struct 	is_ph<ph<N>>			: std::true_type  {};


//////////////////////////////////////////////////////////////   VAR

	template<class T>
struct  var_t : ref_container<T&&>{
		using typename ref_container<T&&>::value_type;
		typedef void is_functor;

	explicit var_t(T&& t)  : ref_container<T&&>(std::forward<T>(t))  {}; // this->value initialised

	value_type operator() (...) { return std::forward<T>(this->value); }
};

	template<class T>
var_t<T> var(T&& t) { return var_t<T>(std::forward<T>(t)); }


//////////////////////////////////////////////////////////////   CONSTANT

	template<class T>
struct  constant_t {
		typedef void is_functor;
		const T&  value_cref;

	explicit constant_t(const T& t)  : value_cref(t)   {};

	const T& operator() (...) { return value_cref; }
};

	template<class T>
constant_t<T>  constant(const T& t) { return constant_t<T>(t); }


//////////////////////////////////////////////////////////////   FUNCTOR_T

	// primary
	template<class Op, class Opnd1, class Opnd2>
struct  functor_t;


	//  +Fr 
	template<class Fr>
struct  functor_t <plus1,Fr,void> {
	typedef void is_functor;
	functor_t(Fr fr) : fr(fr) {};
	Fr fr;
	template<class Arg>
	auto operator() (Arg arg) -> decltype(+arg) { return +fr(arg); }
 };

	//  -Fr 
	template<class Fr>
struct  functor_t <minus1,Fr,void> {
	typedef void is_functor;
	functor_t(Fr fr) : fr(fr) {};
	Fr fr;
	template<class Arg>
	auto operator() (Arg arg)  -> decltype(-arg) { return -fr(arg); }
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
		auto 
	operator() (Arg1 arg1, Arg2 arg2)  -> decltype(arg1+arg2)  { return fr1(arg1,arg2) + fr2(arg1,arg2); }

		//  Arity==1
		template<class Arg>
		auto
	operator() (Arg arg)  -> eIF<!is_tuple<Arg>::value,decltype(fr1(arg) + fr2(arg))> { return fr1(arg) + fr2(arg); }

		//  Tuple
		template<class Arg>
		auto
	operator() (Arg arg) -> eIF<is_tuple<Arg>::value, decltype(fr1(arg) + fr2(arg))> { return fr1(arg) + fr2(arg); }
 };


	
//// unary+

	// + Fr
	template<class Fr, class=typename Fr::is_functor>
	functor_t<plus1,Fr,void>
operator+(Fr fr) {
	return  functor_t<plus1,Fr,void>(fr);
 }


//// unary-

	// - Fr
	template<class Fr, class=typename Fr::is_functor>
	functor_t<minus1,Fr,void>
operator-(Fr fr) {
	return  functor_t<minus1,Fr,void>(fr);
 }


#define  DEF_LAMBDA_OP2(OP,OP_CLASS)										\
		/* Fr OP Fr */											\
		template<class Fr1, class Fr2, class=typename Fr1::is_functor, class=typename Fr2::is_functor>	\
		functor_t<OP_CLASS,Fr1,Fr2>                                                                     \
	operator OP(Fr1 fr1, Fr2 fr2) {                                                                         \
		return  functor_t<OP_CLASS,Fr1,Fr2>(fr1,fr2);                                                   \
	 }                                                                                                      \
                                                                                                                \
		/* Fr OP T */                                                                                   \
		template<class Fr1, class T2, class=typename Fr1::is_functor>                                   \
		eIF<!is_functor<T2>::value, functor_t<OP_CLASS,Fr1,var_t<T2>>>                                  \
	operator OP(Fr1 fr1, T2&& t2) {                                                                         \
		return  functor_t<OP_CLASS,Fr1,var_t<T2>>(fr1,var_t<T2>(std::forward<T2>(t2)));                 \
	 }                                                                                                      \
                                                                                                                \
		/* T + Fr */											\
		template<class T1, class Fr2, class=typename Fr2::is_functor>                                   \
		eIF<!is_functor<T1>::value, functor_t<OP_CLASS,var_t<T1>,Fr2>>                                  \
	operator OP(T1&& t1, Fr2 fr2) {                                                                         \
		return  functor_t<OP_CLASS,var_t<T1>,Fr2>(var_t<T1>(std::forward<T1>(t1)), fr2);                \
	 }

DEF_LAMBDA_OP2(+,plus2)
//DEF_LAMBDA_OP2(-,minus2)

/*
//// binary+

	// Fr + Fr
	template<class Fr1, class Fr2, class=typename Fr1::is_functor, class=typename Fr2::is_functor>
	functor_t<plus2,Fr1,Fr2>
operator+(Fr1 fr1, Fr2 fr2) {
	return  functor_t<plus2,Fr1,Fr2>(fr1,fr2);
 }

	// Fr + T
	template<class Fr1, class T2, class=typename Fr1::is_functor>
	eIF<!is_functor<T2>::value, functor_t<plus2,Fr1,var_t<T2>>>
operator+(Fr1 fr1, T2&& t2) {
	return  functor_t<plus2,Fr1,var_t<T2>>(fr1,var_t<T2>(std::forward<T2>(t2)));
 }

	// T + Fr
	template<class T1, class Fr2, class=typename Fr2::is_functor>
	eIF<!is_functor<T1>::value, functor_t<plus2,var_t<T1>,Fr2>>
operator+(T1&& t1, Fr2 fr2) {
	return  functor_t<plus2,var_t<T1>,Fr2>(var_t<T1>(std::forward<T1>(t1)), fr2);
 }
 */
				};	// namespace sto


				namespace std {
template<int N> 	struct 	is_placeholder<sto::ph<N>>		: std::integral_constant<int,N>  {};
				};	// namespace std


				#endif	// STO_LAMBDA_H
				// vim:ts=8
