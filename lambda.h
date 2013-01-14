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


	template<class Op, class Fr1, class Fr2> 	struct  fr2_t;
	template<class Op, class Fr> 			struct  fr1_t;
	template<class T> 				struct  var_t;
	template<class T> 				struct  constant_t;

		
/////////////////////////////////////////////////////////////////////////////////////////////////   OP CLASSES

#define DEF_OP1_CLASS(PREFIX,POSTFIX,OP_CLASS)	       								\
	struct  OP_CLASS{						                                        \
			template <class Fr>                                                                     \
			static auto                                                                             \
		eval (Fr&& fr)                                                                                  \
			-> decltype (PREFIX  FWD(Fr,fr)  POSTFIX)  {                                                      \
			return       PREFIX  FWD(Fr,fr)  POSTFIX;                                                         \
		};                                                                                              \
	};


#define DEF_OP2_CLASS(INFIX,POSTFIX,OP_CLASS)	       								\
	struct  OP_CLASS{                                                                                       \
			template <class Fr1, class Fr2>                                                         \
			static auto                                                                             \
		eval (Fr1&& fr1, Fr2&& fr2)                                                                     \
			-> decltype(FWD(Fr1,fr1)  INFIX  FWD(Fr2,fr2)  POSTFIX) {                                                    \
			return      FWD(Fr1,fr1)  INFIX  FWD(Fr2,fr2)  POSTFIX;                                                           \
		};                                                                                              \
	};


	/////  UNARY ------------------------------------

	DEF_OP1_CLASS(+,,plus1_op)
	DEF_OP1_CLASS(-,,minus1_op)
	// increment/decrement
	DEF_OP1_CLASS(++,,increment_op)
	DEF_OP1_CLASS(--,,decrement_op)
	DEF_OP1_CLASS(,++,postfix_increment_op)
	DEF_OP1_CLASS(,--,postfix_decrement_op)
	DEF_OP1_CLASS(!,,logical_not_op)
	DEF_OP1_CLASS(~,,bitwise_not_op)
	DEF_OP1_CLASS(&,,addressof_op)
	DEF_OP1_CLASS(*,,contentsof_op)

	// other
	// TODO
  	//class comma_op {}; // boost special
	//class member_pointer_op {}; // boost special

	/////  BINARY ------------------------------------
	
	DEF_OP2_CLASS(+,,plus_op)
       	DEF_OP2_CLASS(-,,minus_op)
       	DEF_OP2_CLASS(*,,multiply_op)
       	DEF_OP2_CLASS(/,,divide_op)
       	DEF_OP2_CLASS(%,,remainder_op)

       	DEF_OP2_CLASS(+=,,plus_assign_op)
       	DEF_OP2_CLASS(-=,,minus_assign_op)
       	DEF_OP2_CLASS(*=,,multiply_assign_op)
       	DEF_OP2_CLASS(/=,,divide_assign_op)
       	DEF_OP2_CLASS(%=,,remainder_assign_op)


       	DEF_OP2_CLASS(<<,,leftshift_op)
       	DEF_OP2_CLASS(>>,,rightshift_op)
       	DEF_OP2_CLASS(^,,xor_op)

       	DEF_OP2_CLASS(<<=,,leftshift_assign_op)
       	DEF_OP2_CLASS(>>=,,rightshift_assign_op)
       	DEF_OP2_CLASS(^=,,xor_assign_op)

       	DEF_OP2_CLASS(||,,logical_or_op)
       	DEF_OP2_CLASS(&&,,logical_and_op)

       	DEF_OP2_CLASS(|,,bitwise_or_op)
       	DEF_OP2_CLASS(&,,bitwise_and_op)
       	DEF_OP2_CLASS(|=,,bitwise_or_assign_op)
       	DEF_OP2_CLASS(&=,,bitwise_and_assign_op)


       	DEF_OP2_CLASS(<,,less_op)
       	DEF_OP2_CLASS(>,,greater_op)
       	DEF_OP2_CLASS(<=,,lessorequal_op)
       	DEF_OP2_CLASS(>=,,greaterorequal_op)
       	DEF_OP2_CLASS(==,,equal_op)
       	DEF_OP2_CLASS(!=,,notequal_op)
                                       

	//  MEMBERS ONLY -----------------------------------------
	DEF_OP2_CLASS(=,,assign_op)
	DEF_OP2_CLASS([,],subscript_op)
	//DEF_OP2_CLASS( TODO call_op)

/////////////////////////////////////////////////////////////////////////////////////////   MEMBER OP MACRO

#define  newDECLARE_MEMBER_OP2(OP,OP_CLASS,THIS)					       			\
														\
		/* This OP Fr */	      									\
		template<class Arg2>	                                                                	\
		eIF<IS_FR(Arg2), fr2_t<OP_CLASS,THIS,Arg2&&>>							\
	operator OP(Arg2&& fr) {                                                                                \
		return  fr2_t<OP_CLASS,THIS,Arg2&&> (FWD(THIS,*this), FWD(Arg2,fr));                        	\
	 }                                                                                                      \
														\
		/* This OP T */                                                                                 \
		template<class Arg2>                                   						\
		eIF<!IS_FR(Arg2), fr2_t<OP_CLASS,THIS,var_t<Arg2&&>>>                           		\
        operator OP(Arg2&& x) {	                                                                      		\
		return  fr2_t<OP_CLASS,THIS,var_t<Arg2&&>> (FWD(THIS,*this), var_t<Arg2&&>(FWD(Arg2,x)));   	\
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
		typedef 	 fr1_t<Op,Fr> 	self_type;
		newDECLARE_MEMBER_OP2(=, 	assign_op, self_type)
		newDECLARE_MEMBER_OP2([],	subscript_op, self_type)

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
		newDECLARE_MEMBER_OP2(=, 	assign_op, self_type)
		newDECLARE_MEMBER_OP2([],	subscript_op, self_type)

	 };
	




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
	newDECLARE_MEMBER_OP2(=, assign_op, self_type)
	newDECLARE_MEMBER_OP2([],subscript_op, self_type)

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
	newDECLARE_MEMBER_OP2(=, assign_op, self_type)
	newDECLARE_MEMBER_OP2([],subscript_op, self_type)

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
	newDECLARE_MEMBER_OP2(=, assign_op, self_type)
	newDECLARE_MEMBER_OP2([],subscript_op, self_type)

};

	template<class T>
constant_t<T>  constant(const T& t) { return constant_t<T>(t); }


////////////////////////////////////////////////////////////////////////////////////////////////////   FUNCTOR_T

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



	newOP1(+,plus1_op)
	newOP1(-,minus1_op)
	newOP1(++,increment_op)
	newOP1(--,decrement_op)
	newPOSTFIX_OP1(++,postfix_increment_op)
	newPOSTFIX_OP1(--,postfix_decrement_op)

	newOP1(!,logical_not_op)
	newOP1(~,bitwise_not_op)
	newOP1(&,addressof_op)
	newOP1(*,contentsof_op)




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
//
//      range	<<	elem	=> 	range
//      elem	<< 	range	=> 	range
//      range	<< 	range	=> 	range
//      cout	<<	ioable	=> 	cout
//      cout	<<	!ioable	=> 	cout
//      cout	<<	λ	=> 	λ
//
//      out_t	,	X	=> 	out_t
//      out_t	,	λ	=> 	λ

template<class Op, class Arg1>	struct  is_range_op   			        { enum {value=false}; };
template<class Arg1>		struct  is_range_op<logical_or_op ,Arg1>	{ enum {value=is_range<Arg1>::value}; };
template<class Arg1>		struct  is_range_op<bitwise_or_op ,Arg1>	{ enum {value=is_range<Arg1>::value}; };
template<class Arg1>		struct  is_range_op<multiply_op   ,Arg1>	{ enum {value=is_range<Arg1>::value}; };
	

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

       
	/*
	 * Attempt to fix: scc '(cout <<  _1)(vint{1});' for clang
	 *
		//  cout  <<  Fr
		template<class Fr2>
		eIF<IS_FR(Fr2), fr2_t<leftshift_op,var_t<std::basic_ostream<char>&>,Fr2&&>>
	operator <<(std::basic_ostream<char>& os, Fr2&& fr2) {
		return  fr2_t<leftshift_op,var_t<std::basic_ostream<char>&>,Fr2&&> (var(os), FWD(Fr2,fr2));
	 }

		//  cout  <<  _1
		        fr2_t<leftshift_op,var_t<std::basic_ostream<char>&>,ph<1>>
	operator <<(std::basic_ostream<char>& os, ph<1> ph1) {
		return  fr2_t<leftshift_op,var_t<std::basic_ostream<char>&>,ph<1>> (var(os), FWD(ph<1>,ph1));
	 }
	*/

	newOP2(+,plus_op)
       	newOP2(-,minus_op)
       	newOP2(*,multiply_op)
       	newOP2(/,divide_op)
       	newOP2(%,remainder_op)

       	newOP2(+=,plus_assign_op)
       	newOP2(-=,minus_assign_op)
       	newOP2(*=,multiply_assign_op)
       	newOP2(/=,divide_assign_op)
       	newOP2(%=,remainder_assign_op)


       	newOP2(<<,leftshift_op)
       	newOP2(>>,rightshift_op)
       	newOP2(^,xor_op)

       	newOP2(<<=,leftshift_assign_op)
       	newOP2(>>=,rightshift_assign_op)
       	newOP2(^=,xor_assign_op)

       	newOP2(||,logical_or_op)
       	newOP2(&&,logical_and_op)

       	newOP2(|,bitwise_or_op)
       	newOP2(&,bitwise_and_op)
       	newOP2(|=,bitwise_or_assign_op)
       	newOP2(&=,bitwise_and_assign_op)


       	newOP2(<,less_op)
       	newOP2(>,greater_op)
       	newOP2(<=,lessorequal_op)
       	newOP2(>=,greaterorequal_op)
       	newOP2(==,equal_op)
       	newOP2(!=,notequal_op)


	
	//#define COMMA ,
	//DEFINE_MEMBER_OP2(=,  	assign_op,    	template<class Fr1 COMMA class Fr2>,		functor_t<assign_op COMMA Fr1 COMMA Fr2>)
	//DEFINE_MEMBER_OP2([], 	subscript_op, 	template<class Fr1 COMMA class Fr2>,		functor_t<assign_op COMMA Fr1 COMMA Fr2>)


///////////////////////////////////////////////////////////////////////////////  TRAITS


				};	// namespace sto


				namespace std {
template<int N> 	struct 	is_placeholder<sto::ph<N>>		: std::integral_constant<int,N>  {};
				};	// namespace std


				#endif	// STO_LAMBDA_H
				// vim:ts=8
