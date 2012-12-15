				#ifndef  SCC_FUNCTOR_H
				#define  SCC_FUNCTOR_H

				#include <cctype>
				#include <sto/meta.h>

				namespace sto {


//  CTYPE.H
//  TODO locale ctype: http://www.boost.org/doc/libs/1_52_0/libs/bind/bind.html#err_modeling_stl_function_object_concepts
//  	http://stackoverflow.com/questions/13676507/how-do-i-display-special-characters-in-c-with-linux
char	toupper (char c) { return std::toupper((int)c); }
char	tolower (char c) { return std::tolower((int)c); }

bool	isalnum (char c) { return std::isalnum ((int)c); }
bool	isalpha (char c) { return std::isalpha ((int)c); }
bool	iscntrl (char c) { return std::iscntrl ((int)c); }
bool	isdigit (char c) { return std::isdigit ((int)c); }
bool	isgraph (char c) { return std::isgraph ((int)c); }
bool	islower (char c) { return std::islower ((int)c); }
bool	isprint (char c) { return std::isprint ((int)c); }
bool	ispunct (char c) { return std::ispunct ((int)c); }
bool	isspace (char c) { return std::isspace ((int)c); }
bool	isupper (char c) { return std::isupper ((int)c); }
bool	isxdigit(char c) { return std::isxdigit((int)c); }


// MATH
template<class T> 	T abs(T t) { return  t > 0 ?  t : -t; }	// cstdlib is bad 


// FOLDERS

			// initial value for folding -- http://en.wikipedia.org/wiki/Identity_element

struct  add_t {
	template<class T> auto operator()(T a, T b) -> decltype(a+b)  { return  a+b; }
	template<class T> T fold_init_value(T) { return T(); }	// T() - correct value for std::string too.
 } add  __attribute__((unused));


struct  sub_t {
	template<class T> auto operator()(T a, T b) -> decltype(a-b)  { return  a-b; }
	template<class T> T fold_init_value(T) { return T(); }
 } sub  __attribute__((unused));


struct mul_t {
	template<class T> auto operator()(T a, T b) -> decltype(a*b)  { return  a*b; }
	template<class T> T fold_init_value(T) { return T(1); }
 } mul  __attribute__((unused));

struct div_t {
	template<class T> auto operator()(T a, T b) -> decltype(a/b)  { return  a/b; }
	template<class T> T fold_init_value(T) { return T(1); }
 } div  __attribute__((unused));		// conflicts with <cstdlib>::div  (included with <string>)
 						// http://stackoverflow.com/questions/10445586/c11-includes-cstdlib-at-times-when-c03-will-not



// PREDICATE
template<class T> 	bool is_odd (const T& t) { return  t%2; }
template<class T> 	bool is_even(const T& t) { return  !bool(t%2); }

// equal, less, greater  (will be depricated)
template<class T> 	struct  eq_t { T t; eq_t (const T& t):t(t){};  bool operator()(const T& x) { return  x==t; } };
template<class T> 	struct  gt_t { T t; gt_t (const T& t):t(t){};  bool operator()(const T& x) { return  x>t; } };
template<class T> 	struct  ls_t { T t; ls_t (const T& t):t(t){};  bool operator()(const T& x) { return  x<t; } };
template<class T> 	eq_t<T> eq(T t)  { return eq_t<T>(t); } 
template<class T> 	gt_t<T> gt(T t)  { return gt_t<T>(t); } 
template<class T> 	ls_t<T> ls(T t)  { return ls_t<T>(t); } 



//  PLACEHOLDER PREDICATE

//struct is_pred_tag {};


	template<class T, class CmpOp> 
struct  ph_pred_t {

	typedef 	T		value_type;
	typedef		int		is_pred; 
	const T 	t;

	ph_pred_t (const T& t) : t(t) {}; 
	bool operator()(const T& x) { return  CmpOp()(x,t); }
 };

#define MK_PH_PRED_T(OP,CMP_OP_CLASS)							\
											\
		template<class T, class Ph>						\
		eIF<std::is_placeholder<Ph>::value == 1, ph_pred_t<T,CMP_OP_CLASS<T>>>	\
	operator OP (Ph ph, T n) {							\
		return  ph_pred_t<T,CMP_OP_CLASS<T>>(n);				\
	};


MK_PH_PRED_T(<, std::less)
MK_PH_PRED_T(>, std::greater)
MK_PH_PRED_T(>=,std::greater_equal)
MK_PH_PRED_T(<=,std::less_equal)
MK_PH_PRED_T(==,std::equal_to)
MK_PH_PRED_T(!=,std::not_equal_to)

//// expression template

	template<class T, class Exp1, class LogicOp,  class Exp2>
struct logical_exp_t {
	Exp1 e1;
	Exp2 e2;
	logical_exp_t (const Exp1& e1, const Exp2& e2) : e1(e1), e2(e2) {}; 
	bool operator()(const T& x) { return  LogicOp()(e1(x),e2(x)); }
};


	//template<class Exp1, class Exp2, class T=int>
	template<class Exp1, class Exp2, class T=typename Exp1::value_type, class=typename Exp1::is_pred, class=typename Exp2::is_pred >
	eIF<
		//is_callable<Exp1, bool(T)>::value &&
		//is_callable<Exp2, bool(T)>::value &&
		std::is_same<typename Exp1::value_type, typename Exp2::value_type>::value 
		,
		logical_exp_t<T, Exp1, std::logical_and<T>,  Exp2>
	>
operator && (Exp1 e1, Exp2 e2) {
	return   logical_exp_t<T, Exp1, std::logical_and<T>,  Exp2>(e1,e2);
};
/*
*/


				};

				#endif
