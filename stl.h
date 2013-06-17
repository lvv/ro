
					#ifndef  RO_STL_H
					#define  RO_STL_H

					#include <stddef.h>

					#include <algorithm>
					#include <iterator>
					#include <string>
					#include <deque>
					#include <tuple>
					#include <cassert>

					#include <ro/meta.h>

					namespace ro {

//##########################################################################  MEMBERS FUNC SHORTCUTS

/////  +Rg   ---   begin(),  	(n/a for c-arrays, use std::begin)

	template<typename Rg>		// do we need to care about r-value-ness here?
	eIF <is_range<Rg>::value  &&  !std::is_array<Rg>::value,  rg_iterator<Rg>>
 operator+      (Rg&& rg) { return std::begin(rg); };	// does not work with r-values


/////  -Rg   ---   end(),  	(n/a for c-arrays, use std::end)
	template<typename Rg>
	eIF <is_range<Rg>::value  &&  !std::is_array<Rg>::value,  rg_iterator<Rg>>
 operator-      (Rg&& rg) { return  std::end(rg); };


 /*
	template<typename Rg>
	eIF <has_size<Rg>::value, size_t>
 operator~      (const Rg& rg) { return size(rg); };
 */


/////  bool(Rg)
			//  if(!Rg)  --- (!Rg.empty())
			//  if(Rg)   --- not implemented,  use  !!Rg instead
	template<typename Rg>
	eIF <has_empty<Rg>::value, bool>
 operator!      (const Rg& rg) { return rg.empty(); };


/////  ++T, T++  ---  front()/back()/.first/.second  (n/a for c-arrays)

	template<typename Rg>
	auto
 front    (Rg&& rg) 
	-> decltype(*std::begin(rg))  {
	return      *std::begin(rg);
 }


	template<class Rg>
	auto
 back      (Rg&& rg)
	-> decltype(*endz(rg))  {
	return *std::prev(ro::endz(rg));
 };



/////  x << Rg >> x   ---  remove head / tail
	template<typename Rg, typename T>
	eIF <is_elem_of<T, Rg>::value &&   has_pop_back<Rg>::value,   Rg&&>
 operator>>      (Rg&& rg, T&& x)    { x = rg.back();   rg.pop_back();   return  std::forward<Rg>(rg); };


	template<typename Rg, typename T>
	eIF <is_elem_of<T,Rg>::value  &&   has_pop_front<Rg>::value,   Rg&&>
 operator<<      (T& x, Rg&& rg)    { x = rg.front();  rg.pop_front();  return  std::forward<Rg>(rg); };



/////  --Rg, Rg--  ---  pop_back/pop_front
	template<typename Rg>
	eIF <is_range<Rg>::value, Rg>
 operator--      (Rg&& rg)         { rg.pop_front();   return  std::forward<Rg>(rg); };


	template<typename Rg>
	eIF <is_range<Rg>::value, Rg>
 operator--      (Rg&& rg, int)    { rg.pop_back();    return  std::forward<Rg>(rg); };


/////  X >> Rg << X

	namespace detail {
		template<class Rg, class X>  eIF<has_push_back  <Rg>::value, Rg&&>	append_elem(Rg&& rg1, X&& x)   { rg1.push_back (std::forward<X>(x));  return std::forward<Rg>(rg1); };
		template<class Rg, class X>  eIF<has_push       <Rg>::value, Rg&&>	append_elem(Rg&& rg1, X&& x)   { rg1.push      (std::forward<X>(x));  return std::forward<Rg>(rg1); };
		template<class Rg, class X>  eIF<has_1arg_insert<Rg>::value, Rg&&>	append_elem(Rg&& rg1, X&& x)   { rg1.insert    (std::forward<X>(x));  return std::forward<Rg>(rg1); };
		//template<class Rg, class X>  eIF<is_cstr<Rg>::value, Rg&&>		append_elem(Rg&& rg1, X&& x)   { auto e=endz(rg1); *e=x; *(e+1)='\0'; return std::forward<Rg>(rg1); };
		//template<size_t N>                           char (&append_elem(char (&S)[N], char c))[N]  { char* e=endz(S);  assert((e-S)<N);  *e=c;  *++e='\0';  return S; };

		template<class Rg, class X>  eIF<has_push_front <Rg>::value, Rg&&>	prepend_elem(Rg&& rg1, X&& x)   { rg1.push_front(std::forward<X>(x));  return std::forward<Rg>(rg1); };
		template<class Rg, class X>  eIF<has_push       <Rg>::value, Rg&&>	prepend_elem(Rg&& rg1, X&& x)   { rg1.push      (std::forward<X>(x));  return std::forward<Rg>(rg1); };
		template<class Rg, class X>  eIF<has_1arg_insert<Rg>::value, Rg&&>	prepend_elem(Rg&& rg1, X&& x)   { rg1.insert    (std::forward<X>(x));  return std::forward<Rg>(rg1); };
		template<class Rg, class X>  eIF<!has_push_front<Rg>::value && has_insert<Rg>::value, Rg&&>
											prepend_elem(Rg&& rg1, X&& x)   { rg1.insert    (std::begin(rg1), std::forward<X>(x));  return std::forward<Rg>(rg1); };
		//template<size_t N>                                               char (&prepend_elem(char (&S)[N], char c))[N]  { char* e=endz(S);  assert((e-S)<N);  std::copy(S,e+1, S+1); *S=c;  return S; };

	}

	// Rg << x
	template<class Rg, class X>
	eIF <is_elem_of<X,Rg>::value,  Rg&&>
 operator << (Rg&& rg1, X&& x)            {  detail::append_elem(std::forward<Rg>(rg1),  std::forward<X>(x));   return  std::forward<Rg>(rg1); };


	// Rg << Cl2
	template<class Rg, class Rg2> 
	eIF <have_same_elem<Rg,Rg2>::value,  Rg&&>
 operator <<  (Rg&& rg1, Rg2&& rg2)         {  for (auto i=std::begin(rg2);  i!=endz(rg2);  ++i)  detail::append_elem(std::forward<Rg>(rg1), std::move(*i));   return  std::forward<Rg>(rg1); };


/////  T >> Rg 
	// x >> Rg 
	template<class X, class Rg>
	eIF <is_elem_of<X,Rg>::value,  Rg&&>
 operator >> (X&& x, Rg&& rg1)            {  detail::prepend_elem(std::forward<Rg>(rg1),  std::forward<X>(x));   return  std::forward<Rg>(rg1); };


	// sRn >> tRn
	template<class sRn, class tRn> 
	eIF <have_same_elem<tRn,sRn>::value,  tRn&&>
 operator >>  (sRn&& src, tRn&& trg)  {

	if (std::begin(src) == endz(src)) 
		return  std::forward<tRn>(trg);

	auto it = endz(src);
	do {
		detail::prepend_elem(std::forward<tRn>(trg), rg_elem_fwd<sRn>(*(it = std::prev(it))));
	} while ( it != std::begin(src));

	return  std::forward<tRn>(trg);
 };



//##############################################################################################  RG - X

////////////////////////////////////////////////////////////////////////////////////////////////  RG - VALUE
	
/////  IMPL

	// vector - value
		template<typename Rg>
		Rg&&
	erase_value_impl (Rg&& rg, rg_elem_type<Rg> value, vector_erasable) {
		rg.erase(std::remove(rg.begin(), rg.end(), value), rg.end());
		return  std::forward<Rg>(rg);
	 };

	// list - value
		template<typename Rg>
		Rg&&
	erase_value_impl (Rg&& rg, rg_elem_type<Rg> value, list_erasable) {
		rg.remove(value);
		return  std::forward<Rg>(rg);
	 };

	// set - value
		template<typename Rg>
		Rg&&
	erase_value_impl (Rg&& rg, rg_elem_type<Rg> value, map_erasable) {
		rg.erase(value);
		return  std::forward<Rg>(rg);
	 };


	// cstr - value
			template<typename Cstr_ptr>
			void
		cstr2_erase_value_impl (Cstr_ptr b,  Cstr_ptr e,  decltype(*Cstr_ptr()) c) {
			auto p=std::find(b,e,c); 
			if (p!=e)   std::copy(p+1,e+1, p);
		 };

		template<typename Cstr>
		Cstr&&
	erase_value_impl (Cstr&& s, rg_elem_type<Cstr> c, cstr_erasable) {
		cstr2_erase_value_impl(s,endz(s),c);
		return std::forward<Cstr>(s);
	 };


	// basic_range - value
		template<typename Rg>
		Rg&&
	erase_value_impl (Rg&& rg, rg_elem_type<Rg> elem, basic_range_erasable) {
		erase_value_impl(rg.value, elem,  erasable_category(rg.value));
		return std::forward<Rg>(rg);
	 };

	// iterator_range - value
		template<typename IR>							// TODO specialize for IR<char>
		IR&&
	erase_value_impl (IR&& ir, rg_elem_type<IR> c, iterator_range_erasable) {
		cstr2_erase_value_impl(ir.b_, ir.e_, c);
		return std::forward<IR>(ir);
	 };



/////  rg - value
	template<class Rg>
	eIF <is_range<Rg>::value, Rg>
 operator- (Rg&& rg, rg_elem_type<Rg> value)    {
	return  erase_value_impl(std::forward<Rg>(rg), value, erasable_category(rg));
 };

	
		// map - key
		template<class Rg>
		eIF <is_range<Rg>::value   &&  is_map<Rg>::value, Rg>
	operator- (Rg&& rg, typename rm_qualifier<Rg>::key_type value)    {
		rg.erase(value);
		return  std::forward<Rg>(rg);
	 };


//////////////////////////////////////////////////////////////////////////////////////////////// RG - PRED


/////  IMPL

	// vector - pred
		template<class Rg, class F>
		Rg&&
	erase_predicate_impl(Rg&& rg,  F&& f, vector_erasable) {
		rg.erase(std::remove_if(rg.begin(), rg.end(), f), rg.end());
		return  std::forward<Rg>(rg);
	 };

	// list - pred
		template<class Rg, class F>
		Rg&&
	erase_predicate_impl(Rg&& rg,  F&& f, list_erasable) {
		rg.remove_if(f);
		return  std::forward<Rg>(rg);
	 };


	/*
	///// Rg - pred
		template<class Rg>
		eIF<is_range<Rg>::value  &&  ! is_map<Rg>::value, Rg>
	operator-(Rg&& rg, bool(*f)(const rg_elem_type<Rg>&)) {
		return  erase_predicate_impl(std::forward<Rg>(rg), f, erasable_category(rg));
	 };

	 */


/////  Rg - pred
	template<class Rg, class F>
	eIF<is_range<Rg>::value  &&  ! is_map<Rg>::value  && is_callable<F, bool(rg_elem_type<Rg>)>::value, Rg>
 operator-(Rg&& rg, F&& f) {
	return  erase_predicate_impl(std::forward<Rg>(rg), f, erasable_category(rg));
 };


	// map - pred
		template<class Rg, class F>
		eIF<is_range<Rg>::value  &&  is_map<Rg>::value && is_callable<F, bool(typename rm_qualifier<Rg>::key_type)>::value, Rg>
	operator-(Rg&& rg, F&& f) {
		for (auto i = rg.begin();  i != rg.end(); ) {
			if (f(i->first))	rg.erase(i++);
			else		++i;
		}
		return  std::forward<Rg>(rg);
	 };


//////////////////////////////////////////////////////////////////////////////////////////////// RG - IT

		// cstr - it
			template<typename Cstr>
			Cstr&&
		erase_it_impl (Cstr&& s, rg_iterator<Cstr> p, cstr_erasable) {
			if (p!=endz(s))   std::copy(p+1,endz(s)+1, p);
			return std::forward<Cstr>(s);
		 };

	// basic_range - it
		template<typename Rg>
		Rg&&
	erase_it_impl (Rg&& rg, rg_iterator<Rg> it,  basic_range_erasable) {
		erase_it_impl(rg.value, it.current,  erasable_category(rg.value));
		return std::forward<Rg>(rg);
	 };


	// iterator_range - it
		template<typename Rg>		       		// implmented for Cstr only, TODO all other
		Rg&&
	erase_it_impl (Rg&& rg, rg_iterator<Rg> it,  iterator_range_erasable) {
		if (it!=rg.e_)   std::copy(it+1, rg.e_+1, it);
		return std::forward<Rg>(rg);
	 };




/////  Rg - it

	template<typename Rg>
	eIF<is_range<Rg>::value  &&  !has_erase1<Rg>::value,  Rg&&>
 operator-(Rg&& rg,  rg_iterator<Rg> it) {
	return  erase_it_impl(std::forward<Rg>(rg), it, erasable_category(rg));
 };


	template<typename Rg>
	eIF<is_range<Rg>::value  &&  has_erase1<Rg>::value,  Rg&&>
 operator-(Rg&& rg,  rg_iterator<Rg> it) {
	rg.erase(it);
	return  rg;
 };



//////////////////////////////////////////////////////////////////////////////////////////////// RG - SUB_RG

/////  rg - sub_rg
	template<class Rg, class sub_Rg>
	eIF<is_range<Rg>::value  &&  !is_cstr<Rg>::value &&  is_range<sub_Rg>::value  &&  have_same_elem<Rg,sub_Rg>::value,  Rg&&>
 operator-(Rg&& rg,  sub_Rg&& sub_rg) {
	auto rg_e = endz(rg);
	auto b    = std::search(std::begin(rg), rg_e, std::begin(sub_rg), endz(sub_rg));
	if (b==rg_e)  return  std::forward<Rg>(rg);
	auto e = b;
	std::advance(e,size(sub_rg));
	rg.erase(b,e); 
	return  std::forward<Rg>(rg);
 };


/////  cstr - sub_rg
	template<class Rg, class sub_Rg>
	eIF<is_cstr<Rg>::value &&  is_range<sub_Rg>::value  &&  have_same_elem<Rg,sub_Rg>::value,  Rg&&>
 operator-(Rg&& rg,  sub_Rg&& sub_rg) {
	auto rg_e = endz(rg);
	auto b    = std::search(std::begin(rg), rg_e, std::begin(sub_rg), endz(sub_rg));
	if (b==rg_e)  return  std::forward<Rg>(rg);
	auto e = b;
	//rg.erase(b,e); 
	std::copy(e,rg_e+1,b);
	return  std::forward<Rg>(rg);
 };



//############################################################################################  Rg / X

/////  IMPL
	namespace detail {


		// Rg / x
			template<typename Rg, typename X>
			eIF <is_elem_of<X, Rg>::value, rg_iterator<Rg>>
		find_elem(Rg&& rg, const X& x)   { return std::find(std::begin(rg), endz(rg), x); };



		// Rg / f
			template<typename Rg, typename F>
			eIF <is_callable<F, bool(rg_elem_type<Rg>)>::value, rg_iterator<Rg>>
		find_elem(Rg&& rg, const F& pred)  { return  std::find_if(std::begin(rg), endz(rg), pred); };

			
		//  Rg1 / Rg2
			template<typename Rg1, typename Rg2>
			eIF <have_same_elem<Rg1, Rg2>::value, rg_iterator<Rg1>>
		find_elem(Rg1&& rg1, const Rg2& rg2)    {  return std::search(std::begin(rg1), endz(rg1), std::begin(rg2), endz(rg2)); };
	}


/////  Rg / X  
	
	// ---  non callable
	template<typename Rg, typename T>
	eIF <is_range<Rg>::value , rg_iterator<Rg>>
 operator / (Rg&& rg, const T& t)                                { return  detail::find_elem(std::forward<Rg>(rg), t); };

	// ---  plain func
	template<typename Rg>
	eIF <is_range<Rg>::value , rg_iterator<Rg>>
 operator / (Rg&& rg, bool(*t)(rg_elem_type<Rg>))                { return  detail::find_elem(std::forward<Rg>(rg), t); };

	// ---  func obj, lambda
	template<typename Rg>
	eIF <is_range<Rg>::value , rg_iterator<Rg>>
 operator / (Rg&& rg, std::function<bool(rg_elem_type<Rg>)> t)   { return  detail::find_elem(std::forward<Rg>(rg), t); };



/////  Rg % T

	//  ---  non callable
	template<typename Rg, typename T>
	eIF <is_range<Rg>::value, bool>
 operator % (Rg&& rg, const T& t)                               { return  endz(rg) != detail::find_elem(std::forward<Rg>(rg), t); };

	//  ---  plain func
	template<typename Rg>
	eIF <is_range<Rg>::value, bool>
 operator % (Rg&& rg, bool(*t)(rg_elem_type<Rg>))               { return  endz(rg) != detail::find_elem(std::forward<Rg>(rg), t); };

	//  ---  func obj, lambda
	template<typename Rg>
	eIF <is_range<Rg>::value, bool>
 operator % (Rg&& rg, std::function<bool(rg_elem_type<Rg>)> t)  { return  endz(rg) != detail::find_elem(std::forward<Rg>(rg), t); };


///////////////////////////////////////////////////////////////////////////////////////////////////  TUPLE / PAIR
template<class U, class V>   U&    front  (std::pair<U,V>&  P)		{ return           P.first;   };
template<class U, class V>   U&&   front  (std::pair<U,V>&& P)		{ return std::move(P.first);  };
template<class U, class V>   V&    back   (std::pair<U,V>&  P)		{ return           P.second;  };
template<class U, class V>   V&&   back   (std::pair<U,V>&& P)		{ return std::move(P.second); };


/////  ++Tpl 
	template <class... Types>
	typename std::tuple_element<std::tuple_size<std::tuple<Types...> >::value-1, typename std::tuple<Types...> >::type&
 back	(typename std::tuple<Types...>& Tpl)  {  return  std::get<std::tuple_size<std::tuple<Types...> >::value-1>(Tpl); };


/////  ++Tpl  (const)
	template <class... Types>
	const typename std::tuple_element<std::tuple_size<std::tuple<Types...> >::value-1, typename std::tuple<Types...> >::type&
 back	(const typename std::tuple<Types...>& Tpl)  {  return  std::get<std::tuple_size<std::tuple<Types...> >::value-1>(Tpl); };


/////  Tpl++ 
	template <class... Types>
	typename std::tuple_element<0, std::tuple<Types...> >::type&
 front	(typename std::tuple<Types...>& Tpl)  {  return  std::get<0>(Tpl); };


/////  Tpl++  (const) 
	template <class... Types>
	const typename std::tuple_element<0, std::tuple<Types...> >::type&
 front	(const typename std::tuple<Types...>& Tpl)  {  return  std::get<0>(Tpl); };


/////////////////////////////////////////////////////////////////////////////////////////////////////  STACK


/////  Stack--
	template<typename Rg>
	eIF <is_stack<Rg>::value, Rg>
 operator--      (Rg&& rg, int)    { rg.pop();   return std::forward<Rg>(rg); };

/////  Stack >> x
	template<typename Rg, typename Xt>
	eIF <has_pop<Rg>::value  &&  is_elem_of<Xt,Rg>::value,  Rg>
 operator>>      (Rg&& rg, Xt&& x)    { x = rg.top();  rg.pop();   return std::forward<Rg>(rg); };


/////  Stack++
	template<typename Rg>
 	eIF <has_top<Rg>::value, rg_elem_type<Rg>>
 back      (Rg&& rg)    { return rg.top(); };



/////////////////////////////////////////////////////////////////////////////////////////////////////  QUEUE


/////  --Queue
	template<class Rg, class Xt>
	eIF <is_queue<Rg>::value, Rg&&> 
 operator--      (Rg&& rg)    { rg.pop();   return std::forward<Rg>(rg); };


/////  x << Queue
	template<class Rg, class Xt>
	eIF <has_pop<Rg>::value  &&  is_elem_of<Xt,Rg>::value, Rg&&> &
 operator<<      (Xt& x, Rg&& rg)    { x = rg.front();  rg.pop();   return std::forward<Rg>(rg); };



/////////////////////////////////////////////////////////////////////////////////////////////////////  ITERATOR


/////  It / x   ---  find() --> it	   usage: scc 'copy(+v9/2, +v9/5, oi),  does not work with pointers (rg++ constrain)
	template<typename It>
	eIF <is_iterator<It>::value,  It>
 operator/       (It&& i, const typename std::iterator_traits<It>::value_type x)    {  while(*i != x) ++i;    return std::forward<It>(i); };


/////////////////////////////////////////////////////////////////////////////////////////////////////  GENERICS
								 // this must be after all front(X) definitions
								
 template<class Rg>	auto operator++(Rg&& rg)      -> decltype(front(rg)) 		{ return front(rg); }
 template<class Rg>	auto operator++(Rg&& rg, int) -> decltype(back (rg)) 		{ return back (rg); }

					};
					#endif	// RO_STL_H
