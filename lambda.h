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

	
	
	template<class Arg>
struct  functor_t;

	template<>
struct  functor_t <unary_arithmetic_action<plus_action>> {
	//functor_t(T t) : t(t) {};
	//T t;
	template<class Arg>
	Arg operator() (Arg arg) { return arg; }
 };

	
	template<class Ph>
	eIF<std::is_placeholder<Ph>::value, functor_t<unary_arithmetic_action<plus_action>>>
operator+(Ph ph) {
	return functor_t<unary_arithmetic_action<plus_action>>();
 }






				};	// namespace sto


				#endif	// STO_LAMBDA_H
