//  LAMBDA EXPRESSIONS
				#ifndef  STO_LAMBDA_H
				#define  STO_LAMBDA_H


				#include <sto/meta.h>


					#include <functional>
				namespace sto {

//////////////////////////////////////////////////////////////   OPERATOR TYPES

	// artihmetic
	class plus1_action {};
	class plus_action {};
	class minus1_action {};
	class minus_action {};
	class multiply_action {};
	class divide_action {};
	class remainder_action {};

		class plus_assign_action {};
		class minus_assign_action {};
		class multiply_assign_action {};
		class divide_assign_action {};
		class remainder_assign_action {};

	// bitwise
	class leftshift_action {};
	class rightshift_action {};
	class xor_action {};

		class leftshift_assign_action {};
		class rightshift_assign_action {};
		class xor_assign_action {};

	// bitwise/logical
	class and_action {};
	class or_action {};
	class not_action {};

		class and_assign_action {};
		class or_assign_action {};
		class not_assign_action {};

	//  relational
	class less_action {};
	class greater_action {};
	class lessorequal_action {};
	class greaterorequal_action {};
	class equal_action {};
	class notequal_action {};

	/////  UNARY ------------------------------------

	// increment/decrement
	class increment_action {};
	class decrement_action {};

		class postfix_increment_action {};
		class postfix_decrement_action {};

	// other
	class addressof_action {};
	class contentsof_action {};


	/////  MEMBERS ONLY -----------------------------------------

	class assign_action {};
	class subscript_action {};
		
//////////////////////////////////////////////////////////////   PLACEHOLDER

	template<int N>
struct  ph {
	typedef void is_functor;
	typedef void is_ph;
	enum {n=N};

	////// 1-ARG

		// non-tuple
		template<class Arg>
		eIF<!is_tuple<Arg>::value, Arg&&>
	operator() (Arg&& arg) { static_assert(N==1, "bad placeholder number");  return FWD(Arg,arg); }

		// tuple                                                     // FIXME: ref-correctness
		template<class Arg>
		eIF<is_tuple<Arg>::value, typename std::tuple_element<N,Arg>::type >
	operator() (Arg arg) { return std::get<N>(arg); }

	////// 2-ARG
	
		//  N==1
		template<class Arg1, class Arg2>
		//eIF<(sizeof(Arg1),N==1), Arg1>			// this dosn't work, gcc bug
		typename std::enable_if<(sizeof(Arg1),N==1), Arg1&&>::type
	operator() (Arg1&& arg1, Arg2&& arg2) { return FWD(Arg1,arg1); }

		//  N==2
		template<class Arg1, class Arg2>
		//eIF<(sizeof(Arg1), N==2), Arg2>			// this dosn't work, gcc bug
		typename std::enable_if<(sizeof(Arg1),N==2), Arg2&&>::type
	operator() (Arg1&& arg1, Arg2&& arg2) { return FWD(Arg2,arg2); }

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

	explicit var_t(T&& t)  : ref_container<T&&>(FWD(T,t))  {}; // this->value initialised

	value_type operator() (...) { return FWD(T,this->value); }
};

	template<class T>
var_t<T> var(T&& t) { return var_t<T>(FWD(T,t)); }


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
	template<class Op, class Fr1, class Fr2>
struct  functor_t;


#define  DEF_LAMBDA_FUNCTOR1(OP,OP_CLASS) 		      							\
                                                                                                                \
		template<class Fr>                                                                              \
	struct  functor_t <OP_CLASS,Fr,void> {                                                                        \
		typedef void is_functor;                                                                        \
		functor_t(Fr fr) : fr(fr) {};                                                                   \
		Fr fr;                                                                                          \
		template<class Arg>                                                                             \
		auto operator() (Arg&& arg) -> decltype(OP arg) { return  OP fr(FWD(Arg,arg)); }                           \
	 };

	DEF_LAMBDA_FUNCTOR1(+,plus1_action)
	DEF_LAMBDA_FUNCTOR1(-,minus1_action)


#define  DEF_LAMBDA_FUNCTOR2(OP,OP_CLASS) 		      							\
                                                                                                                \
		template<class Fr1, class Fr2>                                                                  \
	struct  functor_t <OP_CLASS,Fr1,Fr2> {                                                                  \
		typedef void is_functor;                                                                        \
		functor_t(Fr1 fr1, Fr2 fr2) : fr1(fr1), fr2(fr2) {};                                            \
		Fr1 fr1;                                                                                        \
		Fr2 fr2;                                                                                        \
			/*  Arity==2 */                                                                         \
			template<class Arg1, class Arg2>                                                        \
			auto                                                                                    \
		operator() (Arg1 arg1, Arg2 arg2)  -> decltype(arg1 OP arg2) {                                  \
			return   fr1(arg1,arg2) OP fr2(arg1,arg2);                                              \
		}                                                                                               \
                                                                                                                \
			/*  Arity==1 */                                                                         \
			template<class Arg>                                                                     \
			auto                                                                                    \
		operator() (Arg&& arg)  -> eIF<!is_tuple<Arg>::value,decltype(fr1(FWD(Arg,arg)) OP fr2(FWD(Arg,arg)))> {            \
			return fr1(arg) OP fr2(arg);                                                            \
		}                                                                                               \
                                                                                                                \
			/*  Tuple    */                                                                         \
			template<class Arg>                                                                     \
			auto                                                                                    \
		operator() (Arg arg) -> eIF<is_tuple<Arg>::value, decltype(fr1(arg) OP fr2(arg))> {             \
			return fr1(arg) OP fr2(arg);                                                            \
		}                                                                                               \
	 };

	DEF_LAMBDA_FUNCTOR2(+,plus_action)
	DEF_LAMBDA_FUNCTOR2(-,minus_action)
	DEF_LAMBDA_FUNCTOR2(*,multiply_action)
	DEF_LAMBDA_FUNCTOR2(/,divide_action)
	DEF_LAMBDA_FUNCTOR2(%,remainder_action)
	DEF_LAMBDA_FUNCTOR2(+=,plus_assign_action)
	//DEF_LAMBDA_FUNCTOR2(=,assign_action)



#define  DEF_LAMBDA_OP1(OP,OP_CLASS)										\
                                                                                                                \
		template<class Fr, class=typename Fr::is_functor>                                               \
		functor_t<OP_CLASS,Fr,void>                                                                     \
	operator OP(Fr fr) {                                                                                    \
		return  functor_t<OP_CLASS,Fr,void>(fr);                                                        \
	 }

	DEF_LAMBDA_OP1(+,plus1_action)
	DEF_LAMBDA_OP1(-,minus1_action)



#define  DEF_LAMBDA_OP2(OP,OP_CLASS)										\
                                                                                                                \
		/* Fr OP Fr */											\
		template<class Fr1, class Fr2>	\
		eIF<is_functor<Fr1>::value && is_functor<Fr2>::value, functor_t<OP_CLASS,Fr1&&,Fr2&&>>	\
	operator OP(Fr1&& fr1, Fr2&& fr2) {                                                                     \
		return  functor_t<OP_CLASS,Fr1&&,Fr2&&> (FWD(Fr1,fr1), FWD(Fr2,fr2));                               \
	 }                                                                                                      \
                                                                                                                \
		/* Fr OP T */                                                                                   \
		template<class Fr1, class T2>                                   \
		eIF<is_functor<Fr1>::value && !is_functor<T2>::value, functor_t<OP_CLASS,Fr1&&,var_t<T2&&>>>                                  \
	operator OP(Fr1&& fr1, T2&& t2) {                                                                         \
		return  functor_t<OP_CLASS,Fr1&&,var_t<T2&&>> (FWD(Fr1,fr1), var_t<T2&&>(FWD(T2,t2)));                \
	 }                                                                                                      \
                                                                                                                \
		/* T + Fr */											\
		template<class T1, class Fr2>                                   \
		eIF<!is_functor<T1>::value && is_functor<Fr2>::value, functor_t<OP_CLASS,var_t<T1&&>,Fr2&&>>                                  \
	operator OP(T1&& t1, Fr2&& fr2) {                                                                         \
		return  functor_t<OP_CLASS,var_t<T1>,Fr2> (var_t<T1>(FWD(T1,t1)), FWD(Fr2,fr2));                \
	 }

	DEF_LAMBDA_OP2(+,plus_action)
	DEF_LAMBDA_OP2(-,minus_action)
	DEF_LAMBDA_OP2(*,multiply_action)
	DEF_LAMBDA_OP2(/,divide_action)
	DEF_LAMBDA_OP2(%,remainder_action)
	DEF_LAMBDA_OP2(+=,plus_assign_action)
	//DEF_LAMBDA_OP2(=,assign_action)


///////////////////////////////////////////////////////////////////////////////  TRAITS


				};	// namespace sto


				namespace std {
template<int N> 	struct 	is_placeholder<sto::ph<N>>		: std::integral_constant<int,N>  {};
				};	// namespace std


				#endif	// STO_LAMBDA_H
				// vim:ts=8
