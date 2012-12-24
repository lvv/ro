//  LAMBDA EXPRESSIONS
				#ifndef  STO_LAMBDA_H
				#define  STO_LAMBDA_H


				#include <sto/meta.h>


					#include <functional>
				namespace sto {

////////////////////////////////////////////////////////////////////////////////////////////////   META+FORWARDS

	#define  IS_FR(F)  sto::is_lambda_functor<F>::value
	template<bool A, bool B, bool C=true, bool D=true>  struct  AND  { enum { value = A && B && C && D }; };


	template<class Op, class Fr1, class Fr2> 	struct  functor_t;
	template<class T> 				struct  var_t;

/////////////////////////////////////////////////////////////////////////////////////////////////   OPERATOR TYPES

	/////  UNARY ------------------------------------

	// artihmetic
	class plus1_action {};
	class minus1_action {};

	// increment/decrement
	class increment_action {};              class postfix_increment_action {}; 
	class decrement_action {};              class postfix_decrement_action {}; 


	// bitwise/logical
	class not_action {};

	// other
	class addressof_action {};
	class contentsof_action {};
  	class comma_action {}; // boost special
	class member_pointer_action {}; // boost special

	/////  BINARY ------------------------------------
	
	// artihmetic
	class plus_action {};                  class plus_assign_action {};       
	class minus_action {};                 class minus_assign_action {};      
	class multiply_action {};              class multiply_assign_action {};   
	class divide_action {};                class divide_assign_action {};     
	class remainder_action {};             class remainder_assign_action {};  

	// bitwise
	class leftshift_action {};             class leftshift_assign_action {};       
	class rightshift_action {};            class rightshift_assign_action {};      
	class xor_action {};                   class xor_assign_action {};             

	// bitwise/logical
	class and_action {};                   class and_assign_action {};   
	class or_action {};                    class or_assign_action {};    

	//  relational
	class less_action {};                  class lessorequal_action {};      
	class greater_action {};               class greaterorequal_action {};   
	class equal_action {};
	class notequal_action {};


	/////  MEMBERS ONLY -----------------------------------------

	class assign_action {};
	class subscript_action {};
	class call_action {};
		
/////////////////////////////////////////////////////////////////////////////////////////   DEF_LAMBDA_FUNCTOR2

#define  DEF_LAMBDA_FUNCTOR2(OP,OP2,OP_CLASS) 		      							\
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
		operator() (Arg1&& arg1, Arg2&& arg2)  -> decltype(FWD(Arg1,arg1) OP FWD(Arg2,arg2) OP2 ) {      \
			return      this->value (FWD(Arg1,arg1),FWD(Arg2,arg2))     	   			\
				OP  this->value2(FWD(Arg1,arg1),FWD(Arg2,arg2)) OP2;          			\
		}                                                                                               \
                                                                                                                \
			/*  Arity==1 */                                                                         \
			template<class Arg>                                                                     \
			auto                                                                                    \
		operator() (Arg&& arg) 										\
			-> eIF<!is_tuple_or_pair<Arg&&>::value,decltype(this->value(FWD(Arg,arg)) OP this->value2(FWD(Arg,arg)) OP2 )> {            \
			return this->value(FWD(Arg,arg)) OP this->value2(FWD(Arg,arg)) OP2;                                                            \
		}                                                                                               \
                                                                                                                \
			/*  Tuple    */                                                                         \
			template<class Arg>                                                                     \
			auto                                                                                    \
		operator() (Arg&& arg) -> eIF<is_tuple_or_pair<Arg&&>::value, decltype(this->value(FWD(Arg,arg)) OP this->value2(FWD(Arg,arg)) OP2 )> { \
			return this->value(FWD(Arg,arg)) OP this->value2(FWD(Arg,arg)) OP2;                                                            \
		}                                                                                               \
	 };

         DEF_LAMBDA_FUNCTOR2(=,,assign_action)
         DEF_LAMBDA_FUNCTOR2([,],subscript_action)
         //DEF_LAMBDA_FUNCTOR2((,),call_action)

/*
	class assign_action {};
	class subscript_action {};
	class call_action {};
*/
/////////////////////////////////////////////////////////////////////////////////////////////////   PLACEHOLDER

	template<int N>
struct  ph {
	typedef void is_lambda_functor;
	typedef void is_ph;
	enum {n=N};

	///////////////////////////////////// FUNCTOR CALL IFACE

	////// 1-ARG

		// non-tuple
		template<class Arg>
		eIF<!is_tuple_or_pair<Arg>::value, Arg&&>
	operator() (Arg&& arg) { static_assert(N==1, "bad placeholder number");  return FWD(Arg,arg); }

		// tuple                                                     // FIXME: ref-correctness
		template<class Arg>
		eIF<is_tuple_or_pair<Arg>::value, typename std::tuple_element<N,Arg>::type >
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


	///////////////////////////////////// MEMBER-ONLY OVERLOADS

	#define  DEF_LAMBDA_MEMBER_OP2(OP,OP_CLASS)										\
															\
			/* Ph OP Fr */											\
			template<class Fr>	                                                                \
			eIF<IS_FR(Fr), functor_t<OP_CLASS,ph<N>,Fr&&>>					\
		operator OP(Fr&& fr) {                                                                     \
			return  functor_t<OP_CLASS,ph<N>,Fr&&> (ph<N>(), FWD(Fr,fr));                           \
		 }                                                                                                      \
															\
			/* Ph OP T */                                                                                   \
			template<class T>                                   				\
			eIF<!IS_FR(T), functor_t<OP_CLASS,ph<N>,var_t<T&&>>>                           	\
		operator OP(T&& t) {                                                                      	\
			return  functor_t<OP_CLASS,ph<N>,var_t<T&&>> (ph<N>(), var_t<T&&>(FWD(T,t)));         	\
		 }                                                                                                      \

		DEF_LAMBDA_MEMBER_OP2(=,assign_action)
		DEF_LAMBDA_MEMBER_OP2([],subscript_action)
		//DEF_LAMBDA_MEMBER_OP2((),call_action)


	///////  Convertion to std:placeholder::...
	operator typename std::_Placeholder<N> const () const { return std::_Placeholder<N>(); }	// non portable(?)

};

ph<0>	_0;
ph<1>	_1;
ph<2>	_2;
ph<3>	_3;

template<class T> 	struct 	is_ph	 			: std::false_type  {};
template<int N> 	struct 	is_ph<ph<N>>			: std::true_type  {};

/////////////////////////////////////////////////////////////////////////////////////////////////   VAR

	template<class T>
struct  var_t : ref_container<T&&> {
		using typename ref_container<T&&>::value_type;
		typedef  void  is_lambda_functor;

	explicit var_t(T&& t)  : ref_container<T&&>(FWD(T,t))  {}; // this->value initialised

		template<class Arg>
	value_type operator() (Arg&& arg) { return FWD(T,this->value); }

		template<class Arg1, class Arg2>
	value_type operator() (Arg1&& arg1, Arg2&& arg2) { return FWD(T,this->value); }
};

	template<class T>
var_t<T> var(T&& t) { return var_t<T>(FWD(T,t)); }


/////////////////////////////////////////////////////////////////////////////////////////////////   CONSTANT

	template<class T>
struct  constant_t {
		typedef void is_lambda_functor;
		const T&  value_cref;

	explicit constant_t(const T& t)  : value_cref(t)   {};

		template<class Arg>
	const T& operator() (Arg&& arg) { return value_cref; }

		template<class Arg1, class Arg2>
	const T& operator() (Arg1&& arg1, Arg2&& arg2) { return value_cref; }
};

	template<class T>
constant_t<T>  constant(const T& t) { return constant_t<T>(t); }



////////////////////////////////////////////////////////////////////////////////////////////////////   UNARY

#define  DEF_LAMBDA_FUNCTOR1(OP,OP2,OP_CLASS) 		      							\
                                                                                                                \
		template<class Fr>                                                                              \
	struct  functor_t <OP_CLASS,Fr,void> : ref_container<Fr&&> {                                            \
			typedef void is_lambda_functor;                                                         \
			using typename ref_container<Fr&&>::value_type;                                         \
		explicit functor_t(Fr&& fr) :  ref_container<Fr&&>(FWD(Fr,fr)) {};                              \
		template<class Arg>                                                                             \
		auto operator() (Arg&& arg) -> decltype(OP arg OP2) { return  OP (this->value(FWD(Arg,arg))) OP2 ;} \
	 };


	DEF_LAMBDA_FUNCTOR1(+,,plus1_action)
	DEF_LAMBDA_FUNCTOR1(-,,minus1_action)
	DEF_LAMBDA_FUNCTOR1(++,,increment_action)
	DEF_LAMBDA_FUNCTOR1(--,,decrement_action)
	DEF_LAMBDA_FUNCTOR1(,++,postfix_increment_action)
	DEF_LAMBDA_FUNCTOR1(,--,postfix_decrement_action)

	DEF_LAMBDA_FUNCTOR1(!,,not_action)
	DEF_LAMBDA_FUNCTOR1(&,,addressof_action)

	DEF_LAMBDA_FUNCTOR1(*,,contentsof_action) 		// TOFIX


#define  DEF_LAMBDA_OP1(OP,OP_CLASS)				       	\
									\
		template<class Fr>					\
		eIF<IS_FR(Fr), functor_t<OP_CLASS,Fr&&,void>>		\
	operator OP(Fr&& fr) {						\
		return  functor_t<OP_CLASS,Fr&&,void>(FWD(Fr,fr));	\
	 }

#define  DEF_LAMBDA_POSTFIX_OP1(OP,OP_CLASS)				\
									\
		template<class Fr>					\
		eIF<IS_FR(Fr), functor_t<OP_CLASS,Fr&&,void>>		\
	operator OP(Fr&& fr,int) {					\
		return  functor_t<OP_CLASS,Fr&&,void>(FWD(Fr,fr));	\
	 }

	DEF_LAMBDA_OP1(+,plus1_action)
	DEF_LAMBDA_OP1(-,minus1_action)
	DEF_LAMBDA_OP1(++,increment_action)
	DEF_LAMBDA_OP1(--,decrement_action)
	DEF_LAMBDA_POSTFIX_OP1(++,postfix_increment_action)
	DEF_LAMBDA_POSTFIX_OP1(--,postfix_decrement_action)

	DEF_LAMBDA_OP1(!,not_action)
	DEF_LAMBDA_OP1(&,addressof_action)

	DEF_LAMBDA_OP1(*,contentsof_action)


	

#define  DEF_LAMBDA_OP2(OP,OP_CLASS)										\
                                                                                                                \
		/*  Fr  OP  Fr  */											\
		template<class Fr1, class Fr2>	                                                                \
		eIF<AND<IS_FR(Fr1), IS_FR(Fr2)>::value, functor_t<OP_CLASS,Fr1&&,Fr2&&>>					\
	operator OP(Fr1&& fr1, Fr2&& fr2) {                                                                     \
		return  functor_t<OP_CLASS,Fr1&&,Fr2&&> (FWD(Fr1,fr1), FWD(Fr2,fr2));                           \
	 }                                                                                                      \
                                                                                                                \
		/*  Fr  OP  T  */                                                                                   \
		template<class Fr1, class T2>                                   				\
		eIF<AND<IS_FR(Fr1), !IS_FR(T2)>::value, functor_t<OP_CLASS,Fr1&&,var_t<T2&&>>>                           	\
	operator OP(Fr1&& fr1, T2&& t2) {                                                                      	\
		return  functor_t<OP_CLASS,Fr1&&,var_t<T2&&>> (FWD(Fr1,fr1), var_t<T2&&>(FWD(T2,t2)));         	\
	 }                                                                                                      \
                                                                                                                \
		/*  T  OP  Fr  */											\
		template<class T1, class Fr2>									\
		eIF<AND<!IS_FR(T1), !is_range<T1>::value, IS_FR(Fr2)>::value, functor_t<OP_CLASS,var_t<T1&&>,Fr2&&>>                           	\
	operator OP(T1&& t1, Fr2&& fr2) {                                                                      	\
		return  functor_t<OP_CLASS,var_t<T1&&>,Fr2&&> (var_t<T1&&>(FWD(T1,t1)), FWD(Fr2,fr2));         	\
	 }

	DEF_LAMBDA_FUNCTOR2(+,,plus_action)                	DEF_LAMBDA_OP2(+,plus_action)
	DEF_LAMBDA_FUNCTOR2(-,,minus_action)               	DEF_LAMBDA_OP2(-,minus_action)
	DEF_LAMBDA_FUNCTOR2(*,,multiply_action)            	DEF_LAMBDA_OP2(*,multiply_action)
	DEF_LAMBDA_FUNCTOR2(/,,divide_action)              	DEF_LAMBDA_OP2(/,divide_action)
	DEF_LAMBDA_FUNCTOR2(%,,remainder_action)           	DEF_LAMBDA_OP2(%,remainder_action)
                                                          
	DEF_LAMBDA_FUNCTOR2(+=,,plus_assign_action)        	DEF_LAMBDA_OP2(+=,plus_assign_action)
	DEF_LAMBDA_FUNCTOR2(-=,,minus_assign_action)       	DEF_LAMBDA_OP2(-=,minus_assign_action)
	DEF_LAMBDA_FUNCTOR2(*=,,multiply_assign_action)    	DEF_LAMBDA_OP2(*=,multiply_assign_action)
	DEF_LAMBDA_FUNCTOR2(/=,,divide_assign_action)      	DEF_LAMBDA_OP2(/=,divide_assign_action)
	DEF_LAMBDA_FUNCTOR2(%=,,remainder_assign_action)   	DEF_LAMBDA_OP2(%=,remainder_assign_action)


	DEF_LAMBDA_FUNCTOR2(<<,,leftshift_action)		DEF_LAMBDA_OP2(<<,leftshift_action)
	DEF_LAMBDA_FUNCTOR2(>>,,rightshift_action)		DEF_LAMBDA_OP2(>>,rightshift_action)
	DEF_LAMBDA_FUNCTOR2(^,,xor_action)			DEF_LAMBDA_OP2(^,xor_action)

	DEF_LAMBDA_FUNCTOR2(<<=,,leftshift_assign_action)	DEF_LAMBDA_OP2(<<=,leftshift_assign_action)
	DEF_LAMBDA_FUNCTOR2(>>=,,rightshift_assign_action)	DEF_LAMBDA_OP2(>>=,rightshift_assign_action)
	DEF_LAMBDA_FUNCTOR2(^=,,xor_assign_action)		DEF_LAMBDA_OP2(^=,xor_assign_action)

	DEF_LAMBDA_FUNCTOR2(||,,or_action)			DEF_LAMBDA_OP2(||,or_action)
	DEF_LAMBDA_FUNCTOR2(&&,,and_action)			DEF_LAMBDA_OP2(&&,and_action)

	// relational
	DEF_LAMBDA_FUNCTOR2(<,,less_action)			DEF_LAMBDA_OP2(<,less_action)
	DEF_LAMBDA_FUNCTOR2(>,,greater_action)			DEF_LAMBDA_OP2(>,greater_action)
	DEF_LAMBDA_FUNCTOR2(<=,,lessorequal_action)		DEF_LAMBDA_OP2(<=,lessorequal_action)
	DEF_LAMBDA_FUNCTOR2(>,,greaterorequal_action)		DEF_LAMBDA_OP2(>=,greaterorequal_action)
	DEF_LAMBDA_FUNCTOR2(==,,equal_action)			DEF_LAMBDA_OP2(==,equal_action)
	DEF_LAMBDA_FUNCTOR2(!=,,notequal_action)	       	DEF_LAMBDA_OP2(!=,notequal_action)




///////////////////////////////////////////////////////////////////////////////  TRAITS


				};	// namespace sto


				namespace std {
template<int N> 	struct 	is_placeholder<sto::ph<N>>		: std::integral_constant<int,N>  {};
				};	// namespace std


				#endif	// STO_LAMBDA_H
				// vim:ts=8
