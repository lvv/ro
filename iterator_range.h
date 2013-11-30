
						#ifndef  RO_ITERATOR_RANGE_H
						#define  RO_ITERATOR_RANGE_H

						#include <ro/meta.h>
						namespace    {
						namespace ro {

/////////////////////////////////////////////////////////////////////////////////////////  ITERATOR_RANGE

	template<typename I>
struct  iterator_range {
		typedef		      I							iterator;
		typedef		const I							const_iterator;
		typedef		typename std::iterator_traits<rm_ref<I>>::value_type		value_type;
		typedef		typename std::iterator_traits<I>::difference_type	difference_type ;
		typedef		typename std::iterator_traits<I>::pointer		pointer ;
		typedef		typename std::iterator_traits<I>::reference		reference ;
	iterator b_, e_;

	// CTOR
	iterator_range()  : b_(0), e_(0) {};
	iterator_range(iterator b, iterator e)  : b_(b), e_(e) {};

	// ASSIGNMENT ------------------------------------------------------------------------------------
	
	// RG = VALUE
	iterator_range& operator= (value_type x) { std::fill(b_, e_, x);  return *this; };


	// RG<T> = RG2
		// primary
		// iterators do not know about about undelying stucture of container and we can not resize it.
		// we copy min(size(RG), size(RG2)) elements
		template<typename rhsRn>
		eIF <
			/*!is_cstr<I>::value  &&*/  is_elem_of<value_type, rhsRn>::value,
			iterator_range&
		>
	operator= (const rhsRn& rhs) {
		auto it= b_;
		for (const auto &x: rhs)  {
			if (it==e_) break; 
			*it++ = x;
		}
		return *this;
	}

	/*
	// RG<cstr> = RG2
		// for C-STR we can clear destination
		// for now, we copy size(RG2) elements 
		template<typename rhsRn>
		eIF <
			is_cstr<I>::value  &&  std::is_convertible<rg_elem_type<rhsRn>,char>::value,
			iterator_range&
		>
	operator= (const rhsRn& rhs) {
		e_=b_;
		for (const char &c: rhs) *e_++ = c;
		*e_ = '\0';
		return *this;
	}
	*/
	




	//template<typename Rg>
	//iterator_range( typename std::enable_if<std::is_same<I, typename Rg::iterator>::value, Rg>::type&  C)  : b_(begin(C)), e_(end(C)) {};

	iterator	begin()		{ return b_; };
	iterator	end()		{ return e_; };
	const_iterator	begin() const	{ return b_; };
	const_iterator	end()   const	{ return e_; };
	size_t		size()   const	{ return std::distance(b_,e_); };



	/* to bool conversion --
	*/
	 
			struct PointerConversion { int valid; };
			typedef int PointerConversion::* datamemptr;

	operator datamemptr  () const {
		return  (b_ != e_) ? &PointerConversion::valid : 0;
	};

 };

////////////////////////////////////////////////////////////////  TRAITS

//template<typename T>	struct  is_range_t	<iterator_range<T>> 	: std::true_type  {};
template<typename I>	struct  is_range_t	<iterator_range<I>>	: std::true_type { };


////////////////////////////////////////////////////////////////  RANGE() -- range maker

	template<typename I>
	eIF<is_iterator<I>::value, iterator_range<I>>
range(I b, I e) { return iterator_range<I>(b,e); };

	template<typename I>
	eIF<is_random_access_iterator<I>::value, iterator_range<I>>
range(I b, size_t sz) { return iterator_range<I>(b,b+sz); };


////////////////////////////////////////////////////////////////  SIMPLE PIPE

	template<class Rg> 					// for std::unique
	eIF <is_range<Rg>::value,  iterator_range<rg_iterator<Rg>>>
operator| (Rg&& rg, rg_iterator<Rg> (*f)(rg_iterator<Rg> b, rg_iterator<Rg> e) )    {
	auto b     = std::begin(rg);
	auto new_e = f(b,std::end(rg));
	// TODO: add erase	
	return iterator_range<rg_iterator<Rg>>(b,new_e);
 }

						};
						};
						#endif //  RO_ITERATOR_RANGE_H
