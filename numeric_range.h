						#ifndef  RO_NUMERIC_RANGE_H
						#define  RO_NUMERIC_RANGE_H

						#include <ro/meta.h>
					
						#include <cassert>

						namespace ro {

/////////////////////////////////////////////////////////////////////////////////////////  NUMERIC_RANGE_ITERATOR
	template<typename T>
struct	numeric_range;

	template<typename T>
struct	numeric_range_iterator {

	// CTOR
	explicit numeric_range_iterator (const numeric_range<T>* range, size_t current)
		:	range		(range), 
			current		(current)
	{};

	// NON-STL 
	typedef		numeric_range_iterator		self_type;


	// STL IFACE
	typedef		rm_ref<T>		value_type;
	typedef		numeric_range_iterator	const_iterator;
	typedef		numeric_range_iterator	iterator;
	typedef		size_t			difference_type ;
	typedef		size_t			size_type;
	typedef		const value_type*	const_pointer;
	typedef		const_pointer		pointer;
	typedef		value_type		const_reference;
	typedef		value_type		reference;


	// 
	T get_value(int i) 		const	{  assert( 0 <= i && i <= range->range_end);  return range->from  +  i * range->step; }


	// forward  iter
	const_reference	operator*()	const	{ return   get_value(current); }
	const_pointer	operator->()	const	{ return  &(operator*()); }
	const_iterator&	operator++()		{ ++current;   return *this; }
	const_iterator&	operator++(int)		{ auto tmp=*this;  ++current;  return tmp; }

			// we make assumpation that comparission is done only with  end()
	bool		operator==(const const_iterator &rhs)	const	{ return   rhs.current == current; }
	bool		operator!=(const const_iterator &rhs)	const	{ return   ! (*this == rhs); }

	typedef		std::random_access_iterator_tag		iterator_category;
		
	// bidi iter 
	const_iterator&	operator--()					{ --current;   assert(current>=0); return *this; }
	const_iterator&	operator--(int)					{ auto tmp=*this;  --current;   assert(current>=0); return tmp; }

	// random access iter
	const_iterator	operator+= (difference_type n)			{ current+=n;  assert(current<=range->range_end);  return *this; }
	const_iterator	operator-= (difference_type n)			{ current-=n;  assert(current>=0);           return *this; }
	value_type	operator[] (difference_type n)		const	{ return get_value(n); }

	bool		operator<  (const self_type& other)	const	{ return current <  other.current; } 
	bool		operator<= (const self_type& other)	const	{ return current <= other.current; } 
	bool		operator>  (const self_type& other)	const	{ return current >  other.current; } 
	bool		operator>= (const self_type& other)	const	{ return current >= other.current; } 

	// MEMBERS
	const numeric_range<T>*		range;
	size_t				current;	

};


/////////////////////////////////////////////////////////////////////////////////////////  NUMERIC_RANGE

	template<typename T>
struct  numeric_range {

	// STL IFACE
	typedef		rm_ref<T>				value_type;
	typedef		size_t					difference_type;
	typedef		size_t					size_type;
	typedef		void					const_pointer;
	typedef		void					pointer;
	typedef		value_type				const_reference;
	typedef		value_type				reference;
	typedef		numeric_range_iterator<value_type>	const_iterator;
	typedef		numeric_range_iterator<value_type>	iterator;


	// NON-STL 
	typedef		numeric_range				self_type;

	// MEMBERS
	T 	from, to, step;
	size_t 	range_end;

	// CTOR
	numeric_range()  			: from(T()),  to(T()), step(T())  {};
	numeric_range(T from, T to, T step)	: from(from), to(to),  step(step),  range_end((to-from)/step + 1)  { assert(range_end >= 0);};

	// ITERATOR
	const_iterator	cbegin() const	{ return const_iterator(this, 0); };
	const_iterator	cend()   const	{ return const_iterator(this, range_end); };
	const_iterator	begin()	 const	{ return cbegin(); };
	const_iterator	end()    const	{ return cend(); };


	// RG PROPERTIES
	size_t		size  () const	{ return range_end; };
	bool		empty () const	{ return size() == 0; }
	explicit operator bool() const	{ return !empty(); }


	// ELEM ACCESS
	value_type  	front()  const	{ return  from; }
	value_type  	back()   const	{ return  size() * step; }  
	value_type	operator[] (difference_type n)	const { return from+step*n; }


	// CONVERTABILITY TO STD-CONTAINERS
	
		template<class A, template<class,class> class Ct> 		//  std sequence containers
	operator Ct<value_type,A>(){
		Ct<value_type,A>  C(size());
		C.clear();
		C << *this;							// TOFIX: remove dependance on RO

		return std::move(C);
	}

		template<class Cmp, class A, template<class, class, class> class Ct> 	//  std::set
	operator Ct<value_type,Cmp,A>(){
		Ct<value_type,Cmp,A>  C;
		C << *this;
		return std::move(C);
	}

		
		template<class H, class KE, class A, template<class, class, class, class> class Ct> 
	operator Ct<value_type,H,KE,A>() {					//  std::unordered_set
		Ct<value_type,H,KE,A>  C;
		C << *this;
		return std::move(C);
	}
								// is it possilble to convert to c-array?
 };


////////////////////////////////////////////////////////////////   TRAITS

template<class T>	struct  is_range_t  <numeric_range<T>>				: std::true_type  {};
template<class T>	struct 	is_range_t  <numeric_range_iterator<T>>			: std::false_type  {};

template<class Rg>	struct	is_numeric_range               				: std::false_type {};
template<class T>	struct	is_numeric_range<numeric_range<T>>			: std::true_type {};

template<class It>	struct	is_numeric_range_iterator      				: std::false_type {};
template<class T>	struct	is_numeric_range_iterator <numeric_range_iterator<T>>	: std::true_type {};


////////////////////////////////////////////////////////////////  RANGE() -- range maker

	template<class T1, class T2, class T=decltype(T1()+T2())>
	eIF<std::is_arithmetic<T>::value,  numeric_range<T>>
range(T1 from,  T2 to) { return numeric_range<T>(from, to, from<to ? 1 : -1); };

	template<class T1, class T2, class T3, class T=decltype(T1()+T2()+T3())>
	eIF<std::is_arithmetic<T>::value,  numeric_range<T>>
range(T1 from,  T2 to,  T3 step) { return numeric_range<T>(from, to, step); };

	template<class T>
	eIF<std::is_arithmetic<T>::value,  numeric_range<T>>
range(T to) { return numeric_range<T>(0, to-1, 1); };



						};
						#endif //  RO_NUMERIC_RANGE_H
