
					#ifndef  SCC_IO_H
					#define  SCC_IO_H
					//#include <x86_64-pc-linux-gnu/bits/stdc++.h>

					#include <type_traits>
					#include <iterator>

					#include <utility>	// pair
					#include <tuple>

					#include <iostream>
					#include <iomanip>
					#include <fstream>
						using	std::cout;
						using	std::cout;
						using	std::endl;
						using	std::ostream;

					#include <ro/range.h>

					namespace    {
					namespace ro {

	std::filebuf cout_fb;

bool set_out_file(const char* path) { 
	if (cout_fb.is_open())  cout_fb.close();
	cout_fb.open (path, std::ios::out);
	cout.rdbuf(&cout_fb);
	return  cout_fb.is_open();
}



// Iterators
#ifndef __clang__
#endif

	template<typename IT, typename Unused = typename IT::iterator_category >  // for stl::containers::iterator
	std::ostream&
operator<<      (ostream& os, const IT& it) { std::cout << &*it << " "; return os; };

//	template<typename IT, typename Unused = typename IT::iterator_category >  // for stl::containers::iterator
//	eIF<is_input_iterator<rm_ref<IT>>::value, std::ostream&>
//operator<<      (ostream& os, const IT&) { std::cout << " <stl iterator> "; return os; };



#undef		_
#define		_    ro::out()   ,
#undef		__
#define		__   ro::outln() ,

///////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename Rg >
	eIF<is_range<Rg>::value  &&  !is_ioable<Rg>::value, std::ostream&>
operator<<      (ostream& os, const Rg& rg);

	template<typename Rg >
	eIF <is_stack<Rg>::value, std::ostream> &
operator<<      (ostream& os, Rg rg);

	template<typename Rg >
	eIF <is_queue<Rg>::value, std::ostream> &
operator<<      (ostream& os, Rg rg);

		template<typename T, typename U>
		std::ostream&
operator<<      (ostream& os, const std::pair<T,U>& p);

		template<typename... TT>
		std::ostream&
operator<<      (ostream& os, const std::tuple<TT...>& tup);
///////////////////////////////////////////////////////////////////////////////////////////////////////////

// RANGE
	template<typename Rg>
	eIF<is_range<Rg>::value  &&  !is_ioable<Rg>::value, std::ostream&>
operator<<      (ostream& os, Rg&& rg) {

	if (is_elem_of<char,Rg>::value)  {
		auto I=std::begin(rg);
		while (I != endz(rg)) {
			os  << *I;
			if (++I == endz(rg))  break;
		}
	} else {
		cout << "{";
			auto I=std::begin(rg);
			while (I != endz(rg)) {
				os  << *I;
				if (++I == endz(rg))  break;
				os << ", ";
			}
		cout << "}";
	}
        return os;
};


// STACK
	template<typename Rg >
	eIF <is_stack<Rg>::value, std::ostream> &
operator<<      (ostream& os, Rg rg) {

	Rg CC; 
	while(!rg.empty()) { auto x = rg.top();  rg.pop();  CC.push(std::move(x)); }

	cout << "[";
		if    ( !CC.empty() )  { os         << CC.top();  CC.pop(); }
		while ( !CC.empty() )  { os << ", " << CC.top();  CC.pop(); }
	cout << "]";
        return os;
};

// QUEUE
	template<typename Rg >
	eIF <is_queue<Rg>::value, std::ostream> &
operator<<      (ostream& os, Rg rg) {
	cout << "[";
		if    ( !rg.empty() )  { os         << rg.front();  rg.pop(); }
		while ( !rg.empty() )  { os << ", " << rg.front();  rg.pop(); }
	cout << "]";
        return os;
};



// PAIR -- print any std::pair<printable1, printable2>

		template<typename T, typename U>
		std::ostream&
operator<<      (ostream& os, const std::pair<T,U>& p) {
	os << u8"⟨" << p.first << "," << p.second << u8"⟩";
	return os;
};


// TUPLE -- print any std::tuple<printable ...>

			template< int RI, typename... TT>
	struct	print_tuple_elem  {
		print_tuple_elem (ostream& os, const std::tuple<TT...>& tup)  {
			const size_t  tsize = std::tuple_size<std::tuple<TT...>>::value;
			const size_t  i = tsize - RI;
			os <<  std::get<i>(tup);
			if  (i != tsize-1)   cout << ", ";
			print_tuple_elem<RI-1, TT...>(os, tup);
		};
	};

			template<typename... TT>
	struct	print_tuple_elem <0, TT...> {
		print_tuple_elem (ostream& os, const std::tuple<TT...>& tup)  {};
	};


		template<typename... TT>
		std::ostream&
operator<<      (ostream& os, const std::tuple<TT...>& tup) {
	const size_t  tsize = std::tuple_size<std::tuple<TT...>>::value;
	os << u8"⟨";     print_tuple_elem<tsize, TT...>(os, tup);    os << u8"⟩";
	return os;
};
	// Using C++0x Variadic Templates to Pretty-Print Types
	//	-- http://mlangc.wordpress.com/2010/04/18/using-c0x-variadic-templates-to-pretty-print-types/

///////////////////////////////////////////////////////////////////////////////////////////////////////////  OUTI
// OUTI -- Output Iterator
//	similar to ostream_iterator<T>(cout, " ")
//	used as:	copy(V.begin(), V.end(), oi);

	template<typename U, typename V, template<typename, typename> class CL>  void
oi_print(const CL<U,V>& v) { std::cout << v; };

namespace oi_space {

		struct oi_t;

		struct	oi_any_t {
				template<typename T>								// POD
			oi_any_t (const T& v)				{ std::cout << v; };

				template<typename U, typename V, template<typename, typename> class CL>	// std::pair
			oi_any_t (const CL<U,V>& v)	{ oi_print<U,V>(v); };
		};

	struct	oi_t  : std::ostream_iterator<oi_any_t> {
		oi_t(): std::ostream_iterator<oi_any_t>(std::cout, " ") {};

			template<typename Rg >
			eIF<is_range<Rg>::value,  void>
		operator=(const Rg& rg)  {
			copy(std::begin(rg),  endz(rg),  *this);
		}
	};

	std::ostream& operator<<      (std::ostream& os, const oi_space::oi_t &)	{return os; };
	std::ostream& operator<<      (std::ostream& os, const oi_space::oi_any_t& s) { return os; };

};

static oi_space::oi_t  oi;


///////////////////////////////////////////////////////////////////////////////  INPUT


struct in_t {
	in_t (): n(0) {};

		// set container size
		size_t n;
	in_t& operator() (long N) { n=N;  return *this; }


		template<typename T>		// primary
	operator T() {
		T x;
		input<T>(x); 
		n = 0;
		return x;
	};


	// PRIMARY (directly inputable)
		template<typename T>	
		eIF< is_ioable<T>::value, void >
	input(T& x)	{ std::cin >> x; }

	// SEQ-CONT
		template<typename Rg>
		eIF<is_range<Rg>::value  &&  has_push_back<Rg>::value  &&  !is_ioable<Rg>::value,  void>
	input(Rg& rg)	{
		typename Rg::value_type t;
		if (n>0) rg.resize(n);
		if (!rg.empty())		for (typename Rg::value_type&x : rg)  { std::cin >> t;   if(!std::cin || n-- <= 0)  break;  x=t;}
		else			{ rg.clear();  while  (std::cin >> t, std::cin)  rg.push_back(t);}
	}

	// SET
		template<typename Rg>
		eIF<is_range<Rg>::value  &&  has_1arg_insert<Rg>::value  &&  !is_map<Rg>::value,  void>
	input(Rg& rg)	{
		typename Rg::value_type t;
		rg.clear(); 
		n = n ? n : -1;
		while  (std::cin >> t,  std::cin && n--)   rg.insert(t);
	}

	// MAP
		template<typename Rg>
		eIF<is_range<Rg>::value  &&  has_1arg_insert<Rg>::value  &&  is_map<Rg>::value,  void>
	input(Rg& rg)	{
		typename Rg::key_type	  k;
		typename Rg::mapped_type  m;
		rg.clear(); 
		n = n ? n : -1;
		while(std::cin >> k >> m  && n--)  rg[k] = m;
	}


	// C-ARRAY
	// impossible to return C-array 

	// STD::ARRAY
	// STD::PAIR
	// STD::TUPLE

};

static in_t in;


// SEQ-CONTAINER

		template<typename Rg>
		eIF<is_range<Rg>::value  &&  has_push_back<Rg>::value  &&  !is_ioable<Rg>::value, std::istream& >
operator>>      (std::istream& is, Rg& rg)    {
	int n = rg.size() ? rg.size() : -1;
	rg.clear(); 
	typename Rg::value_type c;
	while(is>>c && n--)  rg.push_back(c);
	return is;
};



		template<typename T, size_t N>
		eIF<!std::is_same<T,char>::value,  std::istream&>
operator>>      (std::istream& is, T(&A)[N])    {
	T t;
	for(size_t i=0;  i<N  &&  std::cin;  i++)  {
		if(!(is>>t)) break;
		A[i] = t;
	}
	return is;
};



// STD::ARRAY
		template<typename T, size_t N>
		std::istream&
operator>>      (std::istream& is, std::array<T,N>& A) {
	T t;
	for (size_t i=0;  i<N;  i++)  {
		is >> t; if (!is) break;
		A[i] = t;
	}
	return is;
};

// PAIR
		template<typename T, typename U>
		std::istream&
operator>>      (std::istream& is, std::pair<T,U>& p) {
	is >> p.first >>  p.second;
	return is;
};


// TUPLE

			template< int RI, typename... TT>
	struct	input_tuple_elem  {
		input_tuple_elem (std::istream& is, std::tuple<TT...>& tup)  {
			const size_t  tsize = std::tuple_size<std::tuple<TT...>>::value;
			const size_t  i = tsize - RI;
			is >>  std::get<i>(tup);
			input_tuple_elem<RI-1, TT...>(is, tup);
		};
	};

			template<typename... TT>
	struct	input_tuple_elem <0, TT...> {
		input_tuple_elem (std::istream& is, std::tuple<TT...>& tup)  {};
	};


		template<typename... TT>
		std::istream&
operator>>      (std::istream& is, std::tuple<TT...>& tup) {
	const size_t  tsize = std::tuple_size<std::tuple<TT...>>::value;
	input_tuple_elem<tsize, TT...>(is, tup);
	return is;
};


// SET
		template<typename Rg>
		eIF<is_range<Rg>::value  &&  has_1arg_insert<Rg>::value && !is_map<Rg>::value, std::istream& >
operator>>      (std::istream& is, Rg& rg)    {
	typename Rg::value_type c;
	while(is>>c)  rg.insert(c);
	return is;
};


// MAP
		template<typename Rg>
		eIF<is_range<Rg>::value  &&  has_1arg_insert<Rg>::value && is_map<Rg>::value, std::istream& >
operator>>      (std::istream& is, Rg& rg)    {
	typename Rg::key_type	  k;
	typename Rg::mapped_type  m;
	while(is >> k >> m)  rg[k] = m;
	return is;
};

// NOP op<<  (so if last semicolon is fogoten, it is not an error)
struct out;
std::ostream& operator<<      (std::ostream& os, const  ostream& )		{return os; };
std::ostream& operator<<      (std::ostream& os, const out&)			{return os; };

struct  out {

	bool need_blank = false;
	/*
	bool  first_use;
	const char*	sep;
	const char*	paren;

	out ()					      : first_use(true), sep(0),   paren(0)     {init(); };
	out (const char* sep, const char* paren=0) : first_use(true), sep(sep), paren(paren) {init();};
	void init() { if (sep   && !*sep)	sep   = 0; }

	void send_sep() { if (sep && !first_use) cout << sep;  first_use = false; };
	*/

	
	
		template<typename T>	
		out& 

	//// TOFIX: error in: scc 'int i = 1;  ((_1 = 2, _1 + i)(i))'  -- rvalue lost prematurely
	operator,   (T&& x)	{ 
       
	//// linker error on units test (scc -f u-print.h)  in  GCC and CLANG
	//operator,   (const T x)	{      

	//// works correctly with GCC, spits out garbage on unit test with CLANG
	//operator,   (T x)	{	// replaced 'T&&' with  'const T' to fix: 

		if (need_blank  &&  !is_string<T>::value)  std::cout << ' '; 
		std::cout << x;  
		need_blank =  ! is_string<T>::value; 
		return *this;
	};


		template<typename T, size_t N> // we need this because of array decays to pointer
		out& 
	operator,   (T (&x)[N])	{
		if (need_blank  &&  !is_string<T(&)[N]>::value)  std::cout << ' '; 
		std::cout << x;
		need_blank =  ! is_string<T(&)[N]>::value; 
		return *this;
	};


	/* endl, hex, ..  */   
	out&  operator,  (std::ostream&  (*x) (std::ostream& ))	{ std::cout << x;  return *this; };
	out&  operator,  (std::ios&      (*x) (std::ios&     ))	{ std::cout << x;  return *this; };
	out&  operator,  (std::ios_base& (*x) (std::ios_base&))	{ std::cout << x;  return *this; };

	operator bool   () {  return true; }
 };

struct  outln : out  {
	//outln(const char* sep=0, const char* paren=0)	:out(sep, paren)	{};
	~outln()				{std::cout  << '\n'; }
 };



					};
					};
					#endif	// SCC_IO_H
