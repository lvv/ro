						#ifndef  RO_RANGE_H
						#define  RO_RANGE_H

						#include <ro/numeric_range.h>
						#include <ro/iterator_range.h>
						#include <ro/mapped_range.h>
						#include <ro/predicated_range.h>

						namespace ro {

/////////////////////////////////////////////////////////////////  OPERATORS - FREE FUNCTIONS

				// FIXME: this really should be 2 separate funcs for chain
				// and  numeric to make corresponding includes not to depend on range.h 

////  it - n
	template <class RgI>
	eIF<
		is_ro_range_iterator<RgI>::value  &&  is_random_access_iterator<RgI>::value
		, rm_ref<decltype(std::declval<RgI>().current-1, std::declval<RgI>())> 
	>  
operator-  (RgI it, typename RgI::difference_type n) {
	it.current-=n;  return std::move(it);
 }


////  it + n
	template <class RgI>
	eIF<
		is_ro_range_iterator<RgI>::value  &&  is_random_access_iterator<RgI>::value
		, rm_ref<decltype(std::declval<RgI>().current+1, std::declval<RgI>())> 
	>  
operator+  (RgI it, typename RgI::difference_type n) {
	it.current+=n;  return std::move(it);
 }


////  n + it
	template <class RgI>
	eIF<
		( is_numeric_range_iterator<RgI>::value  ||  is_ro_range_iterator<RgI>::value )
		&&  is_random_access_iterator<RgI>::value
		, rm_ref<decltype(1 + std::declval<RgI>().current,  std::declval<RgI>())> 
	>  
operator+  (typename RgI::difference_type n,  RgI it) {
	it.current+=n;  return std::move(it);
 }


////  it - it
	template <class RgI>
	eIF <
		( is_numeric_range_iterator<RgI>::value  ||  is_ro_range_iterator<RgI>::value )
		&&  is_random_access_iterator<RgI>::value
		, rm_ref<decltype(std::declval<RgI>().current - std::declval<RgI>().current,  typename RgI::difference_type())> 
	>  
operator-  (const RgI& it1, const RgI& it2) {
	return it1.current-it2.current;
 }


////////////////////////////////////////////////////////////////////////////////////////////

						}       // ro namespace
						#endif	// RO_RANGE_H
