//  PLACEHOLDER EXPRESSIONS
				#ifndef  STO_EXP_H
				#define  STO_EXP_H

				#include <sto/meta.h>

				namespace sto {

// PREDICATE

	template<class T, class CmpOp> 
struct  ph_pred_t {
	typedef  T  argument_type;
	const argument_type   rhs;

	ph_pred_t (const argument_type& arg) : rhs(arg) {}; 
	bool operator()(const argument_type& lhs) { return  CmpOp()(lhs,rhs); }
 };

#define MK_PH_PRED_T(OP,OP_CLASS, SWAP_OP_CLASS)							\
											\
		template<class T, class Ph>						\
		eIF<std::is_placeholder<Ph>::value == 1, ph_pred_t<T,OP_CLASS<T>>>	\
	operator OP (Ph ph, T n) {							\
		return  ph_pred_t<T,OP_CLASS<T>>(n);				\
	};										\
											\
		template<class T, class Ph>						\
		eIF<std::is_placeholder<Ph>::value == 1, ph_pred_t<T,SWAP_OP_CLASS<T>>>	\
	operator OP (T n, Ph ph) {							\
		return  ph_pred_t<T,SWAP_OP_CLASS<T>>(n);				\
	};


MK_PH_PRED_T(< ,	std::less		,std::greater_equal	)
MK_PH_PRED_T(> ,	std::greater		,std::less_equal	)
MK_PH_PRED_T(>=,	std::greater_equal	,std::less		)
MK_PH_PRED_T(<=,	std::less_equal		,std::greater		)
MK_PH_PRED_T(==,	std::equal_to		,std::equal_to		)
MK_PH_PRED_T(!=,	std::not_equal_to	,std::not_equal_to	)

//// expression template

// 2-ary

		template<class T, class Exp1, class LogicOp,  class Exp2>
	struct logical_exp2_t {
		typedef  T  argument_type;
		Exp1 e1;
		Exp2 e2;
		logical_exp2_t (const Exp1& e1, const Exp2& e2) : e1(e1), e2(e2) {}; 
		bool operator()(const T& x) { return  LogicOp()(e1(x),e2(x)); }
	};


	template<class Exp1, class Exp2, class T=typename Exp1::argument_type>		// &&
	eIF<
		is_callable<Exp1, bool(T)>::value *	// '*' - we can't use && when overloading && (or will inf recurse)
		is_callable<Exp2, bool(T)>::value *
		std::is_same<typename Exp1::argument_type, typename Exp2::argument_type>::value 
		,
		logical_exp2_t<T, Exp1, std::logical_and<T>,  Exp2>
	>
operator && (Exp1 e1, Exp2 e2) {
	return   logical_exp2_t<T, Exp1, std::logical_and<T>,  Exp2>(e1,e2);
};

	template<class Exp1, class Exp2, class T=typename Exp1::argument_type>		// ||
	eIF<
		is_callable<Exp1, bool(T)>::value *	
		is_callable<Exp2, bool(T)>::value *
		std::is_same<typename Exp1::argument_type, typename Exp2::argument_type>::value 
		,
		logical_exp2_t<T, Exp1, std::logical_or<T>,  Exp2>
	>
operator || (Exp1 e1, Exp2 e2) {
	return   logical_exp2_t<T, Exp1, std::logical_or<T>,  Exp2>(e1,e2);
};


// 1-ary
		template<class T, class Exp1, class LogicOp>
	struct logical_exp1_t {
		typedef   T  argument_type;
		Exp1    e1;
		logical_exp1_t (const Exp1& e1) : e1(e1) {}; 
		bool operator()(const T& x) { return  LogicOp()(e1(x)); }
	};



	template<class Exp1, class T=typename Exp1::argument_type>		// ! 
	eIF<
		is_callable<Exp1, bool(T)>::value
		,
		logical_exp1_t<T, Exp1, std::logical_not<T>>
	>
operator ! (Exp1 e1) {
	return   logical_exp1_t<T, Exp1, std::logical_not<T>>(e1);
};

				};

				#endif
