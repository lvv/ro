				#ifndef  RO_FUNCTOR_H
				#define  RO_FUNCTOR_H

				#include <cctype>
				#include <ro/meta.h>

				namespace    {
				namespace ro {



//**** THIS WILL BE DEPRECATED SOON ****
///////  CTYPE.H
//  TODO locale ctype: http://www.boost.org/doc/libs/1_52_0/libs/bind/bind.html#err_modeling_stl_function_object_concepts
//  	http://stackoverflow.com/questions/13676507/how-do-i-display-special-characters-in-c-with-linux
__attribute__((unused))	char	toupper (char c) { return std::toupper((int)c); }
__attribute__((unused))	char	tolower (char c) { return std::tolower((int)c); }
                       	
__attribute__((unused))	bool	isalnum (char c) { return std::isalnum ((int)c); }
__attribute__((unused))	bool	isalpha (char c) { return std::isalpha ((int)c); }
__attribute__((unused))	bool	iscntrl (char c) { return std::iscntrl ((int)c); }
__attribute__((unused))	bool	isdigit (char c) { return std::isdigit ((int)c); }
__attribute__((unused))	bool	isgraph (char c) { return std::isgraph ((int)c); }
__attribute__((unused))	bool	islower (char c) { return std::islower ((int)c); }
__attribute__((unused))	bool	isprint (char c) { return std::isprint ((int)c); }
__attribute__((unused))	bool	ispunct (char c) { return std::ispunct ((int)c); }
__attribute__((unused))	bool	isspace (char c) { return std::isspace ((int)c); }
__attribute__((unused))	bool	isupper (char c) { return std::isupper ((int)c); }
__attribute__((unused))	bool	isxdigit(char c) { return std::isxdigit((int)c); }
//******************************************


////// MATH
template<class T> 	T abs(T t) { return  t > 0 ?  t : -t; }	// cstdlib is bad 


////// FOLDERS

			// initial value for folding -- http://en.wikipedia.org/wiki/Identity_element

struct  plus_t {
	template<class T> auto operator()(T a, T b) -> decltype(a+b)  { return  a+b; }
	template<class T> T fold_init_value(T) { return T(); }	// T() - correct value for std::string too.
 } plus_  __attribute__((unused));



struct  minus_t {
	template<class T> auto operator()(T a, T b) -> decltype(a-b)  { return  a-b; }
	template<class T> T fold_init_value(T) { return T(); }
 } minus_  __attribute__((unused));


struct multiplies_t {
	template<class T> auto operator()(T a, T b) -> decltype(a*b)  { return  a*b; }
	template<class T> T fold_init_value(T) { return T(1); }
 } multiplies_ __attribute__((unused));

struct divides_t {
	template<class T> auto operator()(T a, T b) -> decltype(a/b)  { return  a/b; }
	template<class T> T fold_init_value(T) { return T(1); }
 } divides_  __attribute__((unused));		// conflicts with <cstdlib>::div  (included with <string>)
 						// http://stackoverflow.com/questions/10445586/c11-includes-cstdlib-at-times-when-c03-will-not

						
struct  count_t {						// conflicts with std::count, shoudl be used only with ro::
	template<class T> size_t operator()(size_t n, T b)  { return  n+1; }
	template<class T> size_t fold_init_value(T) { return 0; }    
 } count_  __attribute__((unused));


////// UTILS

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


struct nop_t {
	template<class T>  size_t operator()(T&& a, T&& b) { return 0; };
	template<class T> T fold_init_value(T) { return T(0); }	// indicator for folder
 } nop   __attribute__((unused));


/////// SEQUENCES


				};
				};

				#endif
