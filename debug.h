				#ifndef  STO_DEBUG_H
				#define  STO_DEBUG_H

				#include <iostream>
				#include <vector>
				#include <deque>
				#include <stack>
				#include <set>
				#include <map>
				#include <tuple>
				#include <list>
				#include <forward_list>

				#include <cxxabi.h>

				#ifdef scc_STO
				#include <sto/range.h>
				#endif


				namespace sto {

//////////////////////////////////////////////////////////////////////////////////////////////  PTYPE / PVTYPE

////  TYPE2NAME

	template <typename T>
const char* type2name() {
	const char* t_name = typeid(T).name();
	int status;
	return abi::__cxa_demangle(t_name, NULL, NULL, &status);
}

/////  REF 2 NAME

template <typename T>	struct ref2name                      { static constexpr const char* value =  "";};
template <typename T>	struct ref2name <const T>            { static constexpr const char* value =  "const";};
template <typename T>	struct ref2name <volatile T>         { static constexpr const char* value =  "volatile";};
template <typename T>	struct ref2name <volatile const T>   { static constexpr const char* value =  "volatile const";};

template <typename T>	struct ref2name <T&>                 { static constexpr const char* value =  "&";};
template <typename T>	struct ref2name <const T&>           { static constexpr const char* value =  "const &";};
template <typename T>	struct ref2name <volatile T&>        { static constexpr const char* value =  "volatile &";};
template <typename T>	struct ref2name <volatile const  T&> { static constexpr const char* value =  "volatile const &";};

template <typename T>	struct ref2name <T&&>                { static constexpr const char* value =  "&&";};
template <typename T>	struct ref2name <const T&&>          { static constexpr const char* value =  "const &&";};
template <typename T>	struct ref2name <volatile T&&>       { static constexpr const char* value =  "volatile &&";};
template <typename T>	struct ref2name <volatile const T&&> { static constexpr const char* value =  "volatile const &&";};


#define ptype(T)  std::cout <<  type2name<T>() << " " << ref2name<T>::value <<  std::endl;
#define pvtype(V)  ptype(decltype(V));

//// Template Type Dumper

template<typename> void ttd();


//////////////////////////////////////////////////////////////////////////////////////////////  TRACE_OBJ

//  to - debug Temporary Object
//  neg id - moved object;  id+1000 - id of moved out object 
		template <typename T>
	struct counter {
		static int    created,   alive;
		counter()  {++created; ++alive; }
		~counter() {           --alive; }
	};
	template <typename T> int counter<T>::created (0);
	template <typename T> int counter<T>::alive   (0);


struct  to : counter<to> {
	to(int id) 	: id(id)		{ 						std::cout << "ctor (id)   " << "inst:"  << this->created  << "\tid:" << id << '\n'; }
	to() 			 		{ 						std::cout << "ctor ()    *" << "inst:"  << this->created                << '\n'; }
	to(const to& o): id(o.id ? o.id+1000: 0){ 						std::cout << "ctor (cT&)  " << "inst:"  << this->created  << "\tid:" << id << '\n'; }
	to&  operator=(const to& o)		{ id=(o.id ? o.id+1000: id); 			std::cout << "= cp        " << "inst:"  << this->created  << "\tid:" << id << '\n';  return *this; }
	~to()					{ 						std::cout << "dtor       ~" << "inst:"  << this->created  << "\tid:" << id << '\n'; }
	to(to&& o): id(o.id ? o.id+1000: 0)	{ id=(o.id ? o.id+1000: id); 	o.id = -o.id; 	std::cout << "ctor (T&&)  " << "inst:"  << this->created  << "\tid:" << id << '\n';}
	to&  operator=(to&& o)			{ id=(o.id ? o.id+1000: id); 	o.id = -o.id;	std::cout << "= mv        " << "inst:"  << this->created  << "\tid:" << id << '\n';  return *this; }
	int id = 0;
};

	std::ostream&
operator<<      (std::ostream& os, const to& o) { std::cout << "to{" << o.id << "}";  return os; };


//////////////////////////////////////////////////////////////////////////////////////////////// DEBUG VARS

std::vector<int>	__attribute__((unused)) 	v9      {0,1,2,3,4,5,6,7,8,9},  v0, v23{2,3};
std::deque<int>		__attribute__((unused)) 	d9      {0,1,2,3,4,5,6,7,8,9},  d0, d23{2,3};
std::list<int>		__attribute__((unused)) 	l9      {0,1,2,3,4,5,6,7,8,9},  l0, l23{2,3};
int			__attribute__((unused))		a9[]    {0,1,2,3,4,5,6,7,8,9},  a23[]{2,3};
char			__attribute__((unused))		c9[99]	{"abc-123"},   c23[]{"BC"},   c1='A',  c0='0', cz='\0', ca='a';
const char		__attribute__((unused))		cc9[99]	{"abc-123"},  cc23[]{"bc"},  cc1='A', cc0='0';
std::string		__attribute__((unused))		s9	{"abc-123"},  s0, s23{"bc"},  s1="a";
std::vector<std::string>__attribute__((unused))		vs9	{"", "a", "bb", "ccc", "dddd", "eeeee", "ffffff", "ggggggg", "hhhhhhhh", "kkkkkkkkk"};
std::vector<const char*>__attribute__((unused))		vc9	{"", "a", "bb", "ccc", "dddd", "eeeee", "ffffff", "ggggggg", "hhhhhhhh", "kkkkkkkkk"};
std::vector<std::tuple<int,std::string>>  __attribute__((unused))		vt9	{std::tuple<int,std::string>(1, "a"), std::tuple<int,std::string>(2,"bb"), std::tuple<int,std::string>(3,"ccc"), std::tuple<int,std::string>(4,"dddd"), std::tuple<int,std::string>(5,"eeeee"), std::tuple<int,std::string>(6,"ffffff"), std::tuple<int,std::string>(7,"ggggggg"), std::tuple<int,std::string>(8,"hhhhhhhh"), std::tuple<int,std::string>(9,"kkkkkkkkk")};
std::map<int,std::string>  __attribute__((unused))		m9	{{1, "a"}, {2,"bb"}, {3,"ccc"}, {4,"dddd"}, {5,"eeeee"}, {6,"ffffff"}, {7,"ggggggg"}, {8,"hhhhhhhh"}, {9,"kkkkkkkkk"}};
std::set<int>  __attribute__((unused))			st9	{0,1,2,3,4,5,6,7,8,9};
static std::vector<int>	__attribute__((unused))		mk_v9()	{ return  std::vector<int>{0,1,2,3,4,5,6,7,8,9}; };
static std::vector<int>	__attribute__((unused))		mk_v23(){ return  v23; };
static std::vector<int>	__attribute__((unused))		mk_v0()	{ return  v0; };
static std::deque<int>	__attribute__((unused))		mk_d9()	{ return  d9; };

#ifdef scc_STO
std::forward_list<int>	__attribute__((unused)) 	fl9	{0,1,2,3,4,5,6,7,8,9},  fl0, fl23{2,3};

//  range
auto __attribute__((unused))		rv9	= range(v9); 
auto __attribute__((unused))		rd9	= range(d9); 
auto __attribute__((unused))		rl9	= range(l9); 
auto __attribute__((unused))		rd9r	= range(mk_d9()); 
auto __attribute__((unused))		rrd9	= range(rd9); 
auto __attribute__((unused))		n9	= range(9); 
#endif

				};	// namespace sto
				#endif  // STO_DEBUG_H
				// vim:ts=8 smarttab
