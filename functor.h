				#ifndef  STO_FUNCTOR_H
				#define  STO_FUNCTOR_H

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
						
// UTILS


	template<class Rg>
	eIF<is_range<Rg>::value, size_t>
eval(Rg&& rg) {
	size_t cnt=0;  
	for(auto i=rg.begin();  i!=rg.end();  ++i)  {
		++cnt;
		*i;
	}
	return cnt;
 }
/*
*/
struct nop_t {
	template<class T>  size_t operator()(T&& a, T&& b) { return 0; };
	template<class T> T fold_init_value(T) { return T(0); }	// indicator for folder
 } nop   __attribute__((unused));


				};

				#endif
