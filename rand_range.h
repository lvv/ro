						#ifndef  RO_RAND_RANGE_H
						#define  RO_RAND_RANGE_H

						#include <ro/meta.h>
					
						#include <cassert>

						#include <random>

						namespace    {
						namespace ro {

/////////////////////////////////////////////////////////////////////////////////////////  TODO: RANDOM RANGE

#ifndef __OPENCV_CORE_HPP__

// for now opencv compatible randn/randu

	template<class Rg, class T>
	eIF< std::is_integral<T>::value , void >  
randu(Rg& rg, T from, T to) {
	// http://www.cplusplus.com/reference/random/uniform_int_distribution/
	std::default_random_engine generator;
	std::uniform_int_distribution<T> distribution(from,to);
	for (auto& x: rg) x = distribution(generator);
}
	template<class Rg, class T>
	eIF< std::is_floating_point<T>::value , void >  
randu(Rg& rg, T from, T to) {
	// http://www.cplusplus.com/reference/random/uniform_real_distribution/
	std::default_random_engine generator;
	std::uniform_real_distribution<T> distribution(from,to);
	for (auto& x: rg) x = distribution(generator);
}

	template<class Rg, class T>
void randn(Rg& rg, T mean, T stddev) {
	// http://www.cplusplus.com/reference/random/normal_distribution/
	std::default_random_engine generator;
	std::normal_distribution<T> distribution(mean,stddev);
	for (auto& x: rg) x = distribution(generator);
}

#endif

/////////////////////////////////////////////////////////////////////////////////////////  RAND_RANGE_ITERATOR
	template<typename T, typename D>
struct	rand_range;

	template<typename T, typename D>
struct	rand_range_iterator {

	// CTOR
	explicit rand_range_iterator (rand_range<T, D>* range, size_t elem_i)
		:	range		(range), 
			elem_i		(elem_i)
	{};

	// NON-STL 
	typedef		rand_range_iterator		self_type;


	// STL IFACE
	typedef		rm_ref<T>		value_type;
	typedef		rand_range_iterator	const_iterator;
	typedef		rand_range_iterator	iterator;
	typedef		size_t			difference_type ;
	typedef		size_t			size_type;
	typedef		const value_type*	const_pointer;
	typedef		const_pointer		pointer;
	typedef		value_type		const_reference;
	typedef		value_type		reference;


	// 
	T get_value(int i) 		const	{  assert( 0 <= i && i <= (ssize_t) range->range_end);  return value; }


	// input  iter
	const_reference	operator*()	const	{ return   value; }
	const_pointer	operator->()	const	{ return  &(operator*()); }
	const_iterator&	operator++()		{ ++elem_i;   value = next_value();  return *this; }
	const_iterator&	operator++(int)		{ auto tmp=*this;  this->operator++();  return tmp; }

			// we make assumpation that comparission is done only with  end()
	bool		operator==(const const_iterator &rhs)	const	{ return   rhs.elem_i == elem_i; }
	bool		operator!=(const const_iterator &rhs)	const	{ return   ! (*this == rhs); }

	typedef		std::input_iterator_tag		iterator_category;	//  §24.2.3 -- input.iterators

	// MEMBERS
	const rand_range<T,D>*	range;
	size_t			elem_i;	
	T			value = next_value();
	T next_value() { return const_cast<rand_range<T, D>*>(range)->distribution(const_cast<rand_range<T, D>*>(range)->generator);}

};


/////////////////////////////////////////////////////////////////////////////////////////  NUMERIC_RANGE

	//template<typename T, typename D = std::uniform_real_distribution<T>>
	template<
		typename T,
		typename D = SEL<std::is_integral<T>::value, std::uniform_int_distribution<T>, std::uniform_real_distribution<T>>
	>
struct  rand_range {

	// STL IFACE
	typedef		rm_ref<T>				value_type;
	typedef		size_t					difference_type;
	typedef		size_t					size_type;
	typedef		void					const_pointer;
	typedef		void					pointer;
	typedef		value_type				const_reference;
	typedef		value_type				reference;
	typedef		rand_range_iterator<value_type,D>	const_iterator;
	typedef		rand_range_iterator<value_type,D>	iterator;


	// NON-STL 
	typedef		rand_range				self_type;

	// MEMBERS
	//T 	from, to, step;
	T 	param1, param2;
	size_t 	range_end;

	
	std::default_random_engine	generator;           // http://www.cplusplus.com/reference/random/uniform_int_distribution/
	D				distribution;


	// CTOR
	rand_range()  									: param1(T()),  param2(T())  {};
	rand_range(T param1, T param2,  size_t sz = std::numeric_limits<T>::max())	: param1(param1), param2(param2),   range_end(sz), distribution(param1,param2)  { assert(range_end >= 0);};

	// ITERATOR
	      iterator	begin()		{ return       iterator(this, 0); };
	      iterator	end()   	{ return       iterator(this, range_end); };
	const_iterator	begin() const	{ return const_iterator(this, 0); };
	const_iterator	end()   const	{ return const_iterator(this, range_end); };
	//const_iterator	cbegin() const	{ return const_iterator(this, 0); };
	//const_iterator	cend()   const	{ return const_iterator(this, range_end); };
	//const_iterator	begin()	 const	{ return cbegin(); };
	//const_iterator	end()    const	{ return cend(); };


	// RG PROPERTIES
	size_t		size  () const	{ return range_end; };
	bool		empty () const	{ return size() == 0; }
	explicit operator bool() const	{ return !empty(); }



	// conversion to stl-containers
	#include <ro/conversion.h>
 };


////////////////////////////////////////////////////////////////   TRAITS

template<class T, class D>	struct  is_range_t  <rand_range<T,D>>				: std::true_type  {};
template<class T, class D>	struct 	is_range_t  <rand_range_iterator<T,D>>			: std::false_type  {};

template<class T, class D>      struct  is_ro_range	     <rand_range<T,D>>			: std::true_type {};
template<class T, class D>	struct  is_ro_range_iterator <rand_range_iterator<T,D>>		: std::true_type {};



////////////////////////////////////////////////////////////////  RANGE() -- range maker

	template<class T1, class T2, class T=decltype(T1()+T2())>
	eIF<std::is_arithmetic<T>::value,  rand_range<T>>
urange(T1 p1,  T2 p2, size_t size = std::numeric_limits<T>::max())  { return rand_range<T>(p1, p2, size); };

	template<class T1, class T2, class T=decltype(T1()+T2())>
	eIF<std::is_arithmetic<T>::value,  rand_range<T, std::normal_distribution<T>>>
grange(T1 p1,  T2 p2, size_t size = std::numeric_limits<T>::max())  { return rand_range<T, std::normal_distribution<T>>(p1, p2, size); };

/*
	template<class T1, class T2, class T3, class T=decltype(T1()+T2()+T3())>
	eIF<std::is_arithmetic<T>::value,  numeric_range<T>>
range(T1 from,  T2 to,  T3 step) { return numeric_range<T>(from, to, step); };

	template<class T>
	eIF<std::is_arithmetic<T>::value,  numeric_range<T>>
range(T to) { return numeric_range<T>(0, to-1, 1); };
*/



						};
						};
						#endif //  RO_RAND_RANGE_H
