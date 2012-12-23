//  LAMBDA EXPRESSIONS
				#ifndef  STO_LAMBDA_H
				#define  STO_LAMBDA_H


				#include <sto/meta.h>


					#include <functional>
				namespace sto {

	#define  IS_FR(F)  sto::is_lambda_functor<F>::value

//////////////////////////////////////////////////////////////   OPERATOR TYPES

	/////  UNARY ------------------------------------

	// artihmetic
	class plus1_action {};
	class minus1_action {};

	// increment/decrement
	class increment_action {};
	class decrement_action {};

		class postfix_increment_action {};
		class postfix_decrement_action {};

	// bitwise/logical
	class not_action {};

	// other
	class addressof_action {};
	class contentsof_action {};

	/////  BINARY ------------------------------------
	
	// artihmetic
	class plus_action {};
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


	/////  MEMBERS ONLY -----------------------------------------

	class assign_action {};
	class subscript_action {};
	class call_action {};
		
//////////////////////////////////////////////////////////////   PLACEHOLDER

	template<int N>
struct  ph {
	typedef void is_lambda_functor;
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
struct  var_t : ref_container<T&&> {
		using typename ref_container<T&&>::value_type;
		typedef  void  is_lambda_functor;

	explicit var_t(T&& t)  : ref_container<T&&>(FWD(T,t))  {}; // this->value initialised

	value_type operator() (...) { return FWD(T,this->value); }
};

	template<class T>
var_t<T> var(T&& t) { return var_t<T>(FWD(T,t)); }


//////////////////////////////////////////////////////////////   CONSTANT

	template<class T>
struct  constant_t {
		typedef void is_lambda_functor;
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
	struct  functor_t <OP_CLASS,Fr,void> : ref_container<Fr&&> {                                            \
			typedef void is_lambda_functor;                                                         \
			using typename ref_container<Fr&&>::value_type;                                         \
		explicit functor_t(Fr&& fr) :  ref_container<Fr&&>(FWD(Fr,fr)) {};                              \
		template<class Arg>                                                                             \
		auto operator() (Arg&& arg) -> decltype(OP arg) { return  OP this->value(FWD(Arg,arg)); }       \
	 };

#define  DEF_LAMBDA_POSTFIX_FUNCTOR1(OP,OP_CLASS) 		      				      		\
                                                                                                                \
		template<class Fr>                                                                              \
	struct  functor_t <OP_CLASS,Fr,void> : ref_container<Fr&&> {                                            \
			typedef void is_lambda_functor;                                                         \
			using typename ref_container<Fr&&>::value_type;                                         \
		explicit functor_t(Fr&& fr) :  ref_container<Fr&&>(FWD(Fr,fr)) {};                              \
		template<class Arg>                                                                             \
		auto operator() (Arg&& arg) -> decltype(arg OP) { return  this->value(FWD(Arg,arg)) OP; }       \
	 };

	DEF_LAMBDA_FUNCTOR1(+,plus1_action)
	DEF_LAMBDA_FUNCTOR1(-,minus1_action)
	DEF_LAMBDA_FUNCTOR1(++,increment_action)
	DEF_LAMBDA_FUNCTOR1(--,decrement_action)
	DEF_LAMBDA_POSTFIX_FUNCTOR1(++,postfix_increment_action)
	DEF_LAMBDA_POSTFIX_FUNCTOR1(--,postfix_decrement_action)


#define  DEF_LAMBDA_FUNCTOR2(OP,OP_CLASS) 		      							\
                                                                                                                \
		template<class Fr1, class Fr2>                                                                  \
	struct  functor_t <OP_CLASS,Fr1,Fr2> : ref_container<Fr1&&>, ref_container2<Fr2&&> {                    \
		typedef void is_lambda_functor;                                                                 \
		explicit functor_t(Fr1&& fr1, Fr2&& fr2) :							\
			ref_container <Fr1&&>(FWD(Fr1,fr1)),							\
			ref_container2<Fr2&&>(FWD(Fr2,fr2))							\
		{};                                                                                             \
			/*  Arity==2 */                                                                         \
			template<class Arg1, class Arg2>                                                        \
			auto                                                                                    \
		operator() (Arg1&& arg1, Arg2&& arg2)  -> decltype(FWD(Arg1,arg1) OP FWD(Arg2,arg2)) {          \
			return      this->value (FWD(Arg1,arg1),FWD(Arg2,arg2))     	   			\
				OP  this->value2(FWD(Arg1,arg1),FWD(Arg2,arg2));            			\
		}                                                                                               \
                                                                                                                \
			/*  Arity==1 */                                                                         \
			template<class Arg>                                                                     \
			auto                                                                                    \
		operator() (Arg&& arg) 										\
			-> eIF<!is_tuple<Arg&&>::value,decltype(this->value(FWD(Arg,arg)) OP this->value2(FWD(Arg,arg)))> {            \
			return this->value(FWD(Arg,arg)) OP this->value2(FWD(Arg,arg));                                                            \
		}                                                                                               \
                                                                                                                \
			/*  Tuple    */                                                                         \
			template<class Arg>                                                                     \
			auto                                                                                    \
		operator() (Arg&& arg) -> eIF<is_tuple<Arg&&>::value, decltype(this->value(FWD(Arg,arg)) OP this->value2(FWD(Arg,arg)))> { \
			return this->value(FWD(Arg,arg)) OP this->value2(FWD(Arg,arg));                                                            \
		}                                                                                               \
	 };

	DEF_LAMBDA_FUNCTOR2(+,plus_action)
	DEF_LAMBDA_FUNCTOR2(-,minus_action)
	DEF_LAMBDA_FUNCTOR2(*,multiply_action)
	DEF_LAMBDA_FUNCTOR2(/,divide_action)
	DEF_LAMBDA_FUNCTOR2(%,remainder_action)
	DEF_LAMBDA_FUNCTOR2(+=,plus_assign_action)

	//////  MEMBER FUNCTORS
	//DEF_LAMBDA_FUNCTOR2(=,assign_action)



#define  DEF_LAMBDA_OP1(OP,OP_CLASS)										\
                                                                                                                \
		template<class Fr>                                               \
		eIF<IS_FR(Fr), functor_t<OP_CLASS,Fr&&,void>>                                                                    \
	operator OP(Fr&& fr) {                                                                                    \
		return  functor_t<OP_CLASS,Fr&&,void>(FWD(Fr,fr));                                                        \
	 }

#define  DEF_LAMBDA_POSTFIX_OP1(OP,OP_CLASS)										\
                                                                                                                \
		template<class Fr>                                               \
		eIF<IS_FR(Fr), functor_t<OP_CLASS,Fr&&,void>>                                                                    \
	operator OP(Fr&& fr,int) {                                                                                    \
		return  functor_t<OP_CLASS,Fr&&,void>(FWD(Fr,fr));                                                        \
	 }

	DEF_LAMBDA_OP1(+,plus1_action)
	DEF_LAMBDA_OP1(-,minus1_action)
	DEF_LAMBDA_OP1(++,increment_action)
	DEF_LAMBDA_OP1(--,decrement_action)
	DEF_LAMBDA_POSTFIX_OP1(++,postfix_increment_action)
	DEF_LAMBDA_POSTFIX_OP1(--,postfix_decrement_action)



#define  DEF_LAMBDA_OP2(OP,OP_CLASS)										\
                                                                                                                \
		/* Fr OP Fr */											\
		template<class Fr1, class Fr2>	                                                                \
		eIF<IS_FR(Fr1) && IS_FR(Fr2), functor_t<OP_CLASS,Fr1&&,Fr2&&>>					\
	operator OP(Fr1&& fr1, Fr2&& fr2) {                                                                     \
		return  functor_t<OP_CLASS,Fr1&&,Fr2&&> (FWD(Fr1,fr1), FWD(Fr2,fr2));                           \
	 }                                                                                                      \
                                                                                                                \
		/* Fr OP T */                                                                                   \
		template<class Fr1, class T2>                                   				\
		eIF<IS_FR(Fr1) && !IS_FR(T2), functor_t<OP_CLASS,Fr1&&,var_t<T2&&>>>                           	\
	operator OP(Fr1&& fr1, T2&& t2) {                                                                      	\
		return  functor_t<OP_CLASS,Fr1&&,var_t<T2&&>> (FWD(Fr1,fr1), var_t<T2&&>(FWD(T2,t2)));         	\
	 }                                                                                                      \
                                                                                                                \
		/* T + Fr */											\
		template<class T1, class Fr2>									\
		eIF<!IS_FR(T1) && IS_FR(Fr2), functor_t<OP_CLASS,var_t<T1&&>,Fr2&&>>                           	\
	operator OP(T1&& t1, Fr2&& fr2) {                                                                      	\
		return  functor_t<OP_CLASS,var_t<T1&&>,Fr2&&> (var_t<T1&&>(FWD(T1,t1)), FWD(Fr2,fr2));         	\
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
