//  LAMBDA EXPRESSIONS
				#ifndef  STO_LAMBDA_H
				#define  STO_LAMBDA_H


				#include <iostream>
				#include <functional>
				#include <sto/meta.h>
				#include <sto/stl.h>
				#include <sto/range.h>

				namespace sto {

////////////////////////////////////////////////////////////////////////////////////////////////   META+FORWARDS

	#define  IS_FR(F)  sto::is_lambda_functor<F>::value
	template<bool A, bool B, bool C=true, bool D=true>  struct  AND  { enum { value = A && B && C && D }; };


	template<class Op, class Fr1, class Fr2> 	struct  functor_t;
	template<class Op, class Fr1, class Fr2> 	struct  fr2_t;
	template<class Op, class Fr> 			struct  fr1_t;
	template<class T> 				struct  var_t;
	template<class T> 				struct  constant_t;

		
/////////////////////////////////////////////////////////////////////////////////////////////////   OP CLASSES

	/////  UNARY ------------------------------------

	// artihmetic
	struct  plus1_op{
		template <class Fr>
		static auto eval (Fr&& fr) -> decltype(+fr) { return +fr; };
	};

	struct  plus_op{
		template <class Fr1, class Fr2>
		static auto eval (Fr1&& fr1, Fr2&& fr2) -> decltype(fr1+fr2) { return fr1+fr2; };
	};

	//class plus1_op {};
	class minus1_op {};

	// increment/decrement
	class increment_op {};               
	class decrement_op {};               

	class postfix_increment_op {};
	class postfix_decrement_op {};

	// bitwise/logical
	class not_op {};

	// other
	class addressof_op {};
	class contentsof_op {};
  	class comma_op {}; // boost special
	class member_pointer_op {}; // boost special

	/////  BINARY ------------------------------------
	
	// artihmetic
	//class plus_op {};                    
	class minus_op {};                   
	class multiply_op {};                
	class divide_op {};                  
	class remainder_op {};               

	class plus_assign_op {};     
        class minus_assign_op {};    
        class multiply_assign_op {}; 
        class divide_assign_op {};   
        class remainder_assign_op {};

	// bitwise
	class leftshift_op {};                  
	class rightshift_op {};                 
	class xor_op {};                        
	class bitwise_and_op {};              
	class bitwise_or_op {};               

        class leftshift_assign_op {};  
        class rightshift_assign_op {}; 
        class xor_assign_op {};        
        class bitwise_and_assign_op {};
        class bitwise_or_assign_op {}; 

	// logical
	class logical_and_op {};              
	class logical_or_op {};               
	 //there are no logical assign

	//  relational
	class less_op {};                     
	class greater_op {};                  
	class equal_op {};
	class notequal_op {};
        class lessorequal_op {};   
        class greaterorequal_op {};
                                       

	/////  MEMBERS ONLY -----------------------------------------

	class assign_op {};
	class subscript_op {};
	class call_op {};

/////////////////////////////////////////////////////////////////////////////////////////   MEMBER OP MACRO

#define  newDECLARE_MEMBER_OP2(OP,OP_CLASS,THIS)					       			\
														\
		/* This OP Fr */	      									\
		template<class Arg2>	                                                                	\
		eIF<IS_FR(Arg2), fr2_t<OP_CLASS,THIS,Arg2&&>>							\
	operator OP(Arg2&& fr);                                                                      	\
														\
		/* This OP T */                                                                                 \
		template<class Arg2>                                   						\
		eIF<!IS_FR(Arg2), fr2_t<OP_CLASS,THIS,var_t<Arg2&&>>>                           		\
	operator OP(Arg2&& x);	                                                                      	\


#define  newDEFINE_MEMBER_OP2(OP,OP_CLASS,TMPL, THIS)					       				\
														\
	/* This OP Fr */		       									\
		TMPL												\
		template<class Arg2>	                                                                	\
		eIF<IS_FR(Arg2), fr2_t<OP_CLASS,THIS,Arg2&&>>						\
	THIS::operator OP(Arg2&& fr) {                                                                  	\
		return  fr2_t<OP_CLASS,THIS,Arg2&&> (FWD(THIS,*this), FWD(Arg2,fr));                        \
	 }                                                                                                      \
														\
														\
	/* This OP T */                                                                                 	\
		TMPL												\
		template<class Arg2>                                   						\
		eIF<!IS_FR(Arg2), fr2_t<OP_CLASS,THIS,var_t<Arg2&&>>>                           		\
	THIS::operator OP(Arg2&& x) {                                                                    	\
		return  fr2_t<OP_CLASS,THIS,var_t<Arg2&&>> (FWD(THIS,*this), var_t<Arg2&&>(FWD(Arg2,x)));   \
	 }                                                                                                      \

	/////////////////////////////////////////////////////////////////////////////////////////  FR_T

		template<class Op, class Fr>
	struct  fr1_t  : ref_container<Fr&&> {
		typedef void is_lambda_functor;
		explicit fr1_t(Fr&& fr) :
			ref_container <Fr&&>(FWD(Fr,fr))
		{};
			/*  Arity==2 */
			template<class Arg1, class Arg2>
			auto
		operator() (Arg1&& arg1, Arg2&& arg2)
			-> decltype(Op::eval(this->value (FWD(Arg1,arg1), FWD(Arg2,arg2)))) {
			return      Op::eval(this->value (FWD(Arg1,arg1), FWD(Arg2,arg2)));
		}

			/*  Arity==1 */
			template<class Arg>
			auto
		operator() (Arg&& arg)
			-> eIF<!is_tuple_or_pair<Arg&&>::value,
			decltype(Op::eval(this->value (FWD(Arg,arg))))> {
			return   Op::eval(this->value (FWD(Arg,arg))) ;
		}

			/*  Tuple    */
			template<class Arg>
			auto
		operator() (Arg&& arg)
			-> eIF<is_tuple_or_pair<Arg&&>::value,
			decltype(Op::eval(this->value(FWD(Arg,arg))))> {
			return   Op::eval(this->value(FWD(Arg,arg)));
		}

	 };
	

                                                                                 
		template<class Op, class Fr1, class Fr2>
	struct  fr2_t : ref_container<Fr1&&>, ref_container2<Fr2&&> {
		typedef void is_lambda_functor;
		explicit fr2_t(Fr1&& fr1, Fr2&& fr2) :
			ref_container <Fr1&&>(FWD(Fr1,fr1)),
			ref_container2<Fr2&&>(FWD(Fr2,fr2))
		{};
			/*  Arity==2 */
			template<class Arg1, class Arg2>
			auto
		operator() (Arg1&& arg1, Arg2&& arg2)
			-> decltype(Op::eval(this->value (FWD(Arg1,arg1), FWD(Arg2,arg2)), this->value2(FWD(Arg1,arg1),FWD(Arg2,arg2)))) {
			return      Op::eval(this->value (FWD(Arg1,arg1), FWD(Arg2,arg2)), this->value2(FWD(Arg1,arg1),FWD(Arg2,arg2)));
		}

			/*  Arity==1 */
			template<class Arg>
			auto
		operator() (Arg&& arg)
			-> eIF<!is_tuple_or_pair<Arg&&>::value,
			decltype(Op::eval(this->value (FWD(Arg,arg)), this->value2(FWD(Arg,arg))))> {
			return   Op::eval(this->value (FWD(Arg,arg)), this->value2(FWD(Arg,arg))) ;
		}

			/*  Tuple    */
			template<class Arg>
			auto
		operator() (Arg&& arg)
			-> eIF<is_tuple_or_pair<Arg&&>::value,
			decltype(Op::eval(this->value(FWD(Arg,arg)),this->value2(FWD(Arg,arg))))> {
			return   Op::eval(this->value(FWD(Arg,arg)),this->value2(FWD(Arg,arg)));
		}

		/*  MEMBER-ONLY OVERLOADS */
		typedef 	 fr2_t<Op,Fr1,Fr2> 	self_type;
		newDECLARE_MEMBER_OP2(=, 	assign_action, self_type)
		newDECLARE_MEMBER_OP2([],	subscript_action, self_type)

	 };
	


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define  DECLARE_MEMBER_OP2(OP,OP_CLASS,THIS)					       			\
														\
		/* This OP Fr */	      									\
		template<class Arg2>	                                                                	\
		eIF<IS_FR(Arg2), functor_t<OP_CLASS,THIS,Arg2&&>>							\
	operator OP(Arg2&& fr);                                                                      	\
														\
		/* This OP T */                                                                                 \
		template<class Arg2>                                   						\
		eIF<!IS_FR(Arg2), functor_t<OP_CLASS,THIS,var_t<Arg2&&>>>                           		\
	operator OP(Arg2&& x);	                                                                      	\


#define  DEFINE_MEMBER_OP2(OP,OP_CLASS,TMPL, THIS)					       				\
														\
	/* This OP Fr */		       									\
		TMPL												\
		template<class Arg2>	                                                                	\
		eIF<IS_FR(Arg2), functor_t<OP_CLASS,THIS,Arg2&&>>						\
	THIS::operator OP(Arg2&& fr) {                                                                  	\
		return  functor_t<OP_CLASS,THIS,Arg2&&> (FWD(THIS,*this), FWD(Arg2,fr));                        \
	 }                                                                                                      \
														\
														\
	/* This OP T */                                                                                 	\
		TMPL												\
		template<class Arg2>                                   						\
		eIF<!IS_FR(Arg2), functor_t<OP_CLASS,THIS,var_t<Arg2&&>>>                           		\
	THIS::operator OP(Arg2&& x) {                                                                    	\
		return  functor_t<OP_CLASS,THIS,var_t<Arg2&&>> (FWD(THIS,*this), var_t<Arg2&&>(FWD(Arg2,x)));   \
	 }                                                                                                      \






	/*
		template<class Arg2>	                                                                	\
		eIF<IS_FR(Arg2), functor_t<OP_CLASS,THIS,Arg2&&>>							\
	THIS::operator OP(Arg2&& fr) {                                                                  	\
		return  functor_t<OP_CLASS,THIS,Arg2&&> (FWD(THIS,*this), FWD(Arg2,fr));                           	\
	 }                                                                                                      \
														\
														\
		template<class Arg2>                                   						\
		eIF<!IS_FR(Arg2), functor_t<OP_CLASS,THIS,var_t<Arg2&&>>>                           		\
	THIS::operator OP(Arg2&& x) {                                                                    	\
		return  functor_t<OP_CLASS,THIS,var_t<Arg2&&>> (FWD(THIS,*this), var_t<Arg2&&>(FWD(Arg2,x)));         	\
	 }                                                                                                      \
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
		eIF<!is_tuple_or_pair<Arg>::value && N==1, Arg&&>
	operator() (Arg&& arg) const { return FWD(Arg,arg); }

		// tuple                                                     // FIXME: ref-correctness
		template<class Arg>
		eIF<is_tuple_or_pair<Arg>::value, typename std::tuple_element<N,Arg>::type >
	operator() (Arg arg) const { return std::get<N>(arg); }

	////// 2-ARG
	
		//  N==1
		template<class Arg1, class Arg2>
		//eIF<(sizeof(Arg1),N==1), Arg1>			// this dosn't work, gcc bug
		typename std::enable_if<(sizeof(Arg1),N==1), Arg1&&>::type
	operator() (Arg1&& arg1, Arg2&& arg2) const { return FWD(Arg1,arg1); }

		//  N==2
		template<class Arg1, class Arg2>
		//eIF<(sizeof(Arg1), N==2), Arg2>			// this dosn't work, gcc bug
		typename std::enable_if<(sizeof(Arg1),N==2), Arg2&&>::type
	operator() (Arg1&& arg1, Arg2&& arg2) const { return FWD(Arg2,arg2); }


	///////  Convertion to std:placeholder::...
	operator typename std::_Placeholder<N> const () const { return std::_Placeholder<N>(); }	// non portable(?)

	/*  MEMBER-ONLY OVERLOADS */
	typedef 	 ph<N>	self_type;
	DECLARE_MEMBER_OP2(=, assign_action, self_type)
	DECLARE_MEMBER_OP2([],subscript_action, self_type)

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

	/*  MEMBER-ONLY OVERLOADS */
	typedef 	 var_t<T>	self_type;
	DECLARE_MEMBER_OP2(=, assign_action, self_type)
	DECLARE_MEMBER_OP2([],subscript_action, self_type)

};

	template<class T>
var_t<T&&> var(T&& t) { return var_t<T&&>(FWD(T,t)); }


/////////////////////////////////////////////////////////////////////////////////////////////////   CONSTANT

	template<class T>
struct  constant_t {
		typedef void is_lambda_functor;
		const T&  value_cref;

	explicit constant_t(const T& t)  : value_cref(t)   {};

		template<class Arg>
	const T& operator() (Arg&& arg) const { return value_cref; }

		template<class Arg1, class Arg2>
	const T& operator() (Arg1&& arg1, Arg2&& arg2) const { return value_cref; }

	/*  MEMBER-ONLY OVERLOADS */
	typedef 	 constant_t<T>	self_type;
	DECLARE_MEMBER_OP2(=, assign_action, self_type)
	DECLARE_MEMBER_OP2([],subscript_action, self_type)

};

	template<class T>
constant_t<T>  constant(const T& t) { return constant_t<T>(t); }


////////////////////////////////////////////////////////////////////////////////////////////////////   FUNCTOR_T

#define  FUNCTOR2(OP,OP2,OP_CLASS) 		      							\
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
		operator() (Arg1&& arg1, Arg2&& arg2) -> decltype(FWD(Arg1,arg1) OP FWD(Arg2,arg2) OP2 ) {      \
			return      this->value (FWD(Arg1,arg1),FWD(Arg2,arg2))     	   			\
				OP  this->value2(FWD(Arg1,arg1),FWD(Arg2,arg2)) OP2;          			\
		}                                                                                               \
                                                                                                                \
			/*  Arity==1 */                                                                         \
			template<class Arg>                                                                     \
			auto                                                                                    \
		operator() (Arg&& arg) 	\
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
														\
		/*  MEMBER-ONLY OVERLOADS */                                                                   \
		typedef 	 functor_t<OP_CLASS,Fr1,Fr2> 	self_type;						\
		DECLARE_MEMBER_OP2(=, 	assign_action, self_type)                                                          \
		DECLARE_MEMBER_OP2([],	subscript_action, self_type)                                                      \
                                                                                                                \
	 };                                                                                                     \
	


#define  FUNCTOR1(OP,OP2,OP_CLASS) 		      							\
                                                                                                                \
		template<class Fr>                                                                              \
	struct  functor_t <OP_CLASS,Fr,void> : ref_container<Fr&&> {                                            \
			typedef void is_lambda_functor;                                                         \
			using typename ref_container<Fr&&>::value_type;                                         \
		explicit functor_t(Fr&& fr) :  ref_container<Fr&&>(FWD(Fr,fr)) {};                              \
		template<class Arg>                                                                             \
		auto operator() (Arg&& arg) -> decltype(OP arg OP2) { return  OP (this->value(FWD(Arg,arg))) OP2 ;} \
														\
		/*  MEMBER-ONLY OVERLOADS */                                                                   \
		typedef 	 functor_t<OP_CLASS,Fr,void> 	self_type;						\
		DECLARE_MEMBER_OP2(=, 	assign_action, self_type)                                                          \
		DECLARE_MEMBER_OP2([],	subscript_action, self_type)                                                      \
                                                                                                                \
	 };


	FUNCTOR1(+,,plus1_action)
	FUNCTOR1(-,,minus1_action)
	FUNCTOR1(++,,increment_action)
	FUNCTOR1(--,,decrement_action)
	FUNCTOR1(,++,postfix_increment_action)
	FUNCTOR1(,--,postfix_decrement_action)

	FUNCTOR1(!,,not_action)
	FUNCTOR1(&,,addressof_action)

	FUNCTOR1(*,,contentsof_action) 		// TOFIX


#define  newOP1(OP,OP_CLASS)				       	\
									\
		template<class Fr>					\
		eIF<IS_FR(Fr), fr1_t<OP_CLASS,Fr&&>>		\
	operator OP(Fr&& fr) {						\
		return  fr1_t<OP_CLASS,Fr&&>(FWD(Fr,fr));	\
	 }

#define  newPOSTFIX_OP1(OP,OP_CLASS)				\
									\
		template<class Fr>					\
		eIF<IS_FR(Fr), fr1_t<OP_CLASS,Fr&&>>		\
	operator OP(Fr&& fr,int) {					\
		return  fr1_t<OP_CLASS,Fr&&>(FWD(Fr,fr));	\
	 }


#define  OP1(OP,OP_CLASS)				       	\
									\
		template<class Fr>					\
		eIF<IS_FR(Fr), functor_t<OP_CLASS,Fr&&,void>>		\
	operator OP(Fr&& fr) {						\
		return  functor_t<OP_CLASS,Fr&&,void>(FWD(Fr,fr));	\
	 }

#define  POSTFIX_OP1(OP,OP_CLASS)				\
									\
		template<class Fr>					\
		eIF<IS_FR(Fr), functor_t<OP_CLASS,Fr&&,void>>		\
	operator OP(Fr&& fr,int) {					\
		return  functor_t<OP_CLASS,Fr&&,void>(FWD(Fr,fr));	\
	 }

	newOP1(+,plus1_op)
	//OP1(+,plus1_action)
	OP1(-,minus1_action)
	OP1(++,increment_action)
	OP1(--,decrement_action)
	POSTFIX_OP1(++,postfix_increment_action)
	POSTFIX_OP1(--,postfix_decrement_action)

	OP1(!,not_action)
	OP1(&,addressof_action)

	OP1(*,contentsof_action)




//      λ	ANY	T	=>	λ
//      T	ANY	λ	=>	λ	// default rules
//
//      range	*	λ	=> 	map-range
//      !range	*	λ	=> 	λ
//      λ	*	λ	=> 	λ o λ
//
//      range	|	algo	=> 	basic-range
//      range	|	pred	=> 	predicated-range
//      range	|	λ	=> 	predicated-range
//      !	|	λ	=> 	λ
//
//      range	||	λ	=> 	fold-range
//      !	||	λ	=> 	λ

template<class Op, class Arg1>	struct  is_range_op   			        { enum {value=false}; };
template<class Arg1>		struct  is_range_op<logical_or_action ,Arg1>	{ enum {value=is_range<Arg1>::value}; };
template<class Arg1>		struct  is_range_op<bitwise_or_action ,Arg1>	{ enum {value=is_range<Arg1>::value}; };
template<class Arg1>		struct  is_range_op<multiply_action   ,Arg1>	{ enum {value=is_range<Arg1>::value}; };
	

#define  newOP2(OP,OP_CLASS)										\
                                                                                                                \
		/*  Fr  OP  Fr  */											\
		template<class Fr1, class Fr2>	                                                                \
		eIF<AND<IS_FR(Fr1), IS_FR(Fr2)>::value, fr2_t<OP_CLASS,Fr1&&,Fr2&&>>					\
	operator OP(Fr1&& fr1, Fr2&& fr2) {                                                                     \
		return  fr2_t<OP_CLASS,Fr1&&,Fr2&&> (FWD(Fr1,fr1), FWD(Fr2,fr2));                           \
	 }                                                                                                      \
                                                                                                                \
		/*  Fr  OP  T  */                                                                                   \
		template<class Fr1, class T2>                                   				\
		eIF<AND<IS_FR(Fr1), !IS_FR(T2)>::value, fr2_t<OP_CLASS,Fr1&&,var_t<T2&&>>>                           	\
	operator OP(Fr1&& fr1, T2&& t2) {                                                                      	\
		return  fr2_t<OP_CLASS,Fr1&&,var_t<T2&&>> (FWD(Fr1,fr1), var_t<T2&&>(FWD(T2,t2)));         	\
	 }                                                                                                      \
                                                                                                                \
		/*  T  OP  Fr  */											\
		template<class T1, class Fr2>									\
		eIF<AND<!IS_FR(T1), IS_FR(Fr2), !is_range_op<OP_CLASS,T1>::value>::value, fr2_t<OP_CLASS,var_t<T1&&>,Fr2&&>>                           	\
	operator OP(T1&& t1, Fr2&& fr2) {                                                                      	\
		return  fr2_t<OP_CLASS,var_t<T1&&>,Fr2&&> (var_t<T1&&>(FWD(T1,t1)), FWD(Fr2,fr2));         	\
	 }

#define  OP2(OP,OP_CLASS)										\
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
		eIF<AND<!IS_FR(T1), IS_FR(Fr2), !is_range_op<OP_CLASS,T1>::value>::value, functor_t<OP_CLASS,var_t<T1&&>,Fr2&&>>                           	\
	operator OP(T1&& t1, Fr2&& fr2) {                                                                      	\
		return  functor_t<OP_CLASS,var_t<T1&&>,Fr2&&> (var_t<T1&&>(FWD(T1,t1)), FWD(Fr2,fr2));         	\
	 }

	                                        	newOP2(+,plus_op)
	//FUNCTOR2(+,,plus_action)                	OP2(+,plus_action)
	FUNCTOR2(-,,minus_action)               	OP2(-,minus_action)
	FUNCTOR2(*,,multiply_action)            	OP2(*,multiply_action)
	FUNCTOR2(/,,divide_action)              	OP2(/,divide_action)
	FUNCTOR2(%,,remainder_action)           	OP2(%,remainder_action)
                                                          
	FUNCTOR2(+=,,plus_assign_action)        	OP2(+=,plus_assign_action)
	FUNCTOR2(-=,,minus_assign_action)       	OP2(-=,minus_assign_action)
	FUNCTOR2(*=,,multiply_assign_action)    	OP2(*=,multiply_assign_action)
	FUNCTOR2(/=,,divide_assign_action)      	OP2(/=,divide_assign_action)
	FUNCTOR2(%=,,remainder_assign_action)   	OP2(%=,remainder_assign_action)


	FUNCTOR2(<<,,leftshift_action)			OP2(<<,leftshift_action)
	FUNCTOR2(>>,,rightshift_action)			OP2(>>,rightshift_action)
	FUNCTOR2(^,,xor_action)				OP2(^,xor_action)

	FUNCTOR2(<<=,,leftshift_assign_action)		OP2(<<=,leftshift_assign_action)
	FUNCTOR2(>>=,,rightshift_assign_action)		OP2(>>=,rightshift_assign_action)
	FUNCTOR2(^=,,xor_assign_action)			OP2(^=,xor_assign_action)

	FUNCTOR2(||,,logical_or_action)    		OP2(||,logical_or_action)
	FUNCTOR2(&&,,logical_and_action)	       	OP2(&&,logical_and_action)
	//FUNCTOR2(||=,,logical_or_assign_action)    	OP2(||=,logical_or_assign_action)
	//FUNCTOR2(&&=,,logical_and_assign_action)    	OP2(&&=,logical_and_assign_action)

	FUNCTOR2(|,,bitwise_or_action)    		OP2(|,bitwise_or_action)
	FUNCTOR2(&,,bitwise_and_action)    		OP2(&,bitwise_and_action)
	FUNCTOR2(|=,,bitwise_or_assign_action)    	OP2(|=,bitwise_or_assign_action)
	FUNCTOR2(&=,,bitwise_and_assign_action)    	OP2(&=,bitwise_and_assign_action)

	// relational
	FUNCTOR2(<,,less_action)			OP2(<,less_action)
	FUNCTOR2(>,,greater_action)			OP2(>,greater_action)
	FUNCTOR2(<=,,lessorequal_action)		OP2(<=,lessorequal_action)
	FUNCTOR2(>,,greaterorequal_action)		OP2(>=,greaterorequal_action)
	FUNCTOR2(==,,equal_action)			OP2(==,equal_action)
	FUNCTOR2(!=,,notequal_action)	       		OP2(!=,notequal_action)


/////  MEMBER-ONLY OVERLOADS
	DEFINE_MEMBER_OP2(=,	assign_action, 	    	template<class T>,	var_t<T>)
	DEFINE_MEMBER_OP2([],	subscript_action,	template<class T>,	var_t<T>)

	DEFINE_MEMBER_OP2(=,	assign_action, 		template<class T>,	constant_t<T>)
	DEFINE_MEMBER_OP2([],	subscript_action,	template<class T>,	constant_t<T>)
                                                                                
	DEFINE_MEMBER_OP2(=,  	assign_action,    	template<int N>,	ph<N>)
	DEFINE_MEMBER_OP2([], 	subscript_action, 	template<int N>,	ph<N>)

	
	//#define COMMA ,
	//DEFINE_MEMBER_OP2(=,  	assign_action,    	template<class Fr1 COMMA class Fr2>,		functor_t<assign_action COMMA Fr1 COMMA Fr2>)
	//DEFINE_MEMBER_OP2([], 	subscript_action, 	template<class Fr1 COMMA class Fr2>,		functor_t<assign_action COMMA Fr1 COMMA Fr2>)


///////////////////////////////////////////////////////////////////////////////  TRAITS


				};	// namespace sto


				namespace std {
template<int N> 	struct 	is_placeholder<sto::ph<N>>		: std::integral_constant<int,N>  {};
				};	// namespace std


				#endif	// STO_LAMBDA_H
				// vim:ts=8
