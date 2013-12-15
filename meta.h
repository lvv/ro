					#ifndef  RO_META_H
					#define  RO_META_H

					#include <type_traits>
					#include <functional>
					#include <iterator>
					#include <array>
					#include <list>
					#include <forward_list>
					#include <map>
					#include <set>
					#include <unordered_set>
					#include <unordered_map>
					#include <stack>
					#include <queue>
					#include <tuple>
					#include <algorithm>
					#include <string>

					namespace    {
					namespace ro {

// TODO
// 	SFNIE with NOEXCEPT  --- http://nonchalantlytyped.net/blog/2012/06/27/yet-another-sfinae/

////////////////////////////////////////////////////////////////////////////////////////  META META

struct no_type{};
template<class T> struct identity_t{ typedef T type; };
template<class T> using  identity  = typename identity_t<T>::type;

template<class From, class To> struct copy_rcv			           { typedef To type; };
template<class From, class To> struct copy_rcv<From const            , To> { typedef typename copy_rcv<From, To> ::type const           type; };
template<class From, class To> struct copy_rcv<From volatile         , To> { typedef typename copy_rcv<From, To> ::type volatile        type; };
template<class From, class To> struct copy_rcv<From const volatile   , To> { typedef typename copy_rcv<From, To> ::type const volatile  type; };
template<class From, class To> struct copy_rcv<From &                , To> { typedef typename copy_rcv<From, To> ::type&                type; };
template<class From, class To> struct copy_rcv<From const&           , To> { typedef typename copy_rcv<From, To> ::type const&          type; };
template<class From, class To> struct copy_rcv<From volatile&        , To> { typedef typename copy_rcv<From, To> ::type volatile&       type; };
template<class From, class To> struct copy_rcv<From const volatile&  , To> { typedef typename copy_rcv<From, To> ::type const volatile& type; };
template<class From, class To> struct copy_rcv<From &&               , To> { typedef typename copy_rcv<From, To> ::type&&               type; };
template<class From, class To> struct copy_rcv<From const&&          , To> { typedef typename copy_rcv<From, To> ::type const&&         type; };
template<class From, class To> struct copy_rcv<From volatile&&       , To> { typedef typename copy_rcv<From, To> ::type volatile&&      type; };
template<class From, class To> struct copy_rcv<From const volatile&& , To> { typedef typename copy_rcv<From, To> ::type const volatile&& type; };


template<class T>     typename std::add_lvalue_reference<const T>::type const   decllval();

//  SHORTCUTS
template<class Rg>     using	rm_qualifier     = typename std::remove_cv<typename std::remove_reference<Rg>::type>::type;
template<class Rg>     using	rm_ref           = typename std::remove_reference<Rg>::type;

#define        	mk_type(x)	typename std::integral_constant<int,x>
#define  	FWD(T,t)  	std::forward<T>(t)


////////////////////////////////////////////////////////////////////////////////////////  RG_TRAITS


template <typename T>		struct rg_traits      {
		template <typename U, typename VT = typename rm_ref<U>::value_type>	static VT	vt(rm_ref<U>* u);
		template <typename U>							static no_type	vt(...);
	typedef   decltype(vt<T>(0))   elem_type ;

		template <typename U, typename IT = typename rm_ref<U>::iterator>	static IT       it(rm_ref<U>* u);
		template <typename U>							static no_type  it(...);

		template <typename U, typename IT = typename rm_ref<U>::const_iterator>	static IT       cit(rm_ref<U>* u);
		template <typename U>							static no_type  cit(...);
	typedef   decltype(cit<T>(0))   const_iterator;

	//typedef   decltype(it<T>(0))   iterator;
	typedef		typename std::conditional <
				std::is_const<rm_ref<T>>::value,
				const_iterator,
				decltype(it<T>(0))
			>::type					iterator;


		template <typename U, typename RF = typename rm_ref<U>::reference>	static RF       rf(rm_ref<U>* u);
		template <typename U>                                          		static no_type  rf(...);
	typedef   decltype(rf<T>(0))     reference ;

		template <typename U, typename RF = typename rm_ref<U>::const_reference>static RF       crf(rm_ref<U>* u);
		template <typename U>                                          		static no_type  crf(...);
	typedef   decltype(crf<T>(0))     const_reference ;
};


template <>                     struct rg_traits<char*&>         { typedef  char        elem_type;   typedef  char      *  iterator;  typedef  const char*  const_iterator;   typedef  char      &  reference;  typedef  const char&  const_reference; };
template <>                     struct rg_traits<const char*&>   { typedef  char const  elem_type;   typedef  char const*  iterator;  typedef  const char*  const_iterator;   typedef  char const&  reference;  typedef  const char&  const_reference; };
template <>                     struct rg_traits<char*>          { typedef  char        elem_type;   typedef  char      *  iterator;  typedef  const char*  const_iterator;   typedef  char      &  reference;  typedef  const char&  const_reference; };
template <>                     struct rg_traits<const char*>    { typedef  char const  elem_type;   typedef  char const*  iterator;  typedef  const char*  const_iterator;   typedef  char const&  reference;  typedef  const char&  const_reference; };

template <typename T, size_t N> struct rg_traits<T[N]>           { typedef  T     elem_type;   typedef  T*     iterator;  typedef  const T*     const_iterator;   typedef  T&     reference;  typedef  const T&     const_reference; };
template <typename T, size_t N> struct rg_traits<T(&)[N]>        { typedef  T     elem_type;   typedef  T*     iterator;  typedef  const T*     const_iterator;   typedef  T&     reference;  typedef  const T&     const_reference; };


template<typename Rg>   using rg_elem_type      	= typename rg_traits<Rg>::elem_type;
template<typename Rg>   using rg_iterator       	= typename rg_traits<Rg>::iterator;
template<typename Rg>   using rg_const_iterator 	= typename rg_traits<Rg>::const_iterator;
template<typename Rg>   using rg_reference      	= typename rg_traits<Rg>::reference;
template<typename Rg>   using rg_const_reference	= typename rg_traits<Rg>::const_reference;


/////////////////////////////////////////////////////////////////////////////////////////////////  ERASABPLE_CATEGORY

struct non_erasable{};
struct cstr_erasable{};
struct string_erasable{};
struct vector_erasable{};
struct list_erasable{};
struct map_erasable{};

struct basic_range_erasable{};
struct iterator_range_erasable{};

template<class Rg>  				struct  basic_range;
template<class Rg>  				struct  iterator_range;

 __attribute__((unused))					non_erasable		erasable_category(...)                                 { return non_erasable(); }; 

template<size_t N>						cstr_erasable		erasable_category(char*)                          { return cstr_erasable(); };
template<size_t N>						cstr_erasable		erasable_category(const char*)                    { return cstr_erasable(); };
template<size_t N>						cstr_erasable		erasable_category(char(&)[N])                          { return cstr_erasable(); };
template<size_t N>						cstr_erasable		erasable_category(const char(&)[N])                    { return cstr_erasable(); };

//template<class CharT, class Traits, class A>			string_erasable		erasable_category(std::basic_string<CharT,Traits,A>)   { return string_erasable(); }; 
template<class Rg>						basic_range_erasable	erasable_category(basic_range<Rg>)        	       { return basic_range_erasable(); }; 
template<class Rg>						iterator_range_erasable	erasable_category(iterator_range<Rg>)        	       { return iterator_range_erasable(); }; 
template<class CharT, class Traits, class A>			vector_erasable		erasable_category(std::basic_string<CharT,Traits,A>)   { return vector_erasable(); }; 

template<class T, class A>					vector_erasable		erasable_category(std::vector<T,A>)                    { return vector_erasable(); }; 
template<class T, class A>					vector_erasable		erasable_category(std::deque<T,A>)                     { return vector_erasable(); }; 

template<class T, class A>					list_erasable		erasable_category(std::list<T,A>)                      { return list_erasable(); }; 
template<class T, class A>					list_erasable		erasable_category(std::forward_list<T,A>)              { return list_erasable(); }; 

template<class T, class C, class A>				map_erasable		erasable_category(std::set<T,C,A>)                     { return map_erasable(); }; 
template<class T, class C, class A>				map_erasable		erasable_category(std::multiset<T,C,A>)                { return map_erasable(); }; 
template<class T, class C, class A>				map_erasable		erasable_category(std::unordered_set<T,C,A>)           { return map_erasable(); }; 
template<class T, class C, class A>				map_erasable		erasable_category(std::unordered_multiset<T,C,A>)      { return map_erasable(); }; 

template<class K, class V, class C, class A>			map_erasable		erasable_category(std::map<K,V,C,A>)                   { return map_erasable(); }; 
template<class K, class V, class C, class A>			map_erasable		erasable_category(std::multimap<K,V,C,A>)              { return map_erasable(); }; 
template<class K, class V, class H, class KE, class A>		map_erasable		erasable_category(std::unordered_map<K,V,H,KE,A>)      { return map_erasable(); }; 
template<class K, class V, class H, class KE, class A>		map_erasable		erasable_category(std::unordered_multimap<K,V,H,KE,A>) { return map_erasable(); }; 


/////////////////////////////////////////////////////////////////////////////////////////////////  STD SHORTCUTS

template<bool Cnd, class T=void>     	using  eIF		= typename std::enable_if <Cnd,T>::type;
template<bool Cnd, class T1, class T2>	using  SEL		= typename std::conditional <Cnd,T1,T2>::type;
template<typename Rg>	                using  rg_elem_fwd	= typename  copy_rcv<Rg&&, rg_elem_type<Rg>>::type;
template<typename Rg>	                using  rg_iterator_fwd	= typename  copy_rcv<Rg&&, rg_iterator<Rg>>::type;

template<class T, class TT=rm_qualifier<T>>
struct is_cstr_ptr { enum { value = std::is_pointer<TT>::value  &&  std::is_same<char, rm_qualifier<typename std::remove_pointer<TT>::type>>::value }; };

template<class T, class TT=rm_qualifier<T>>
struct is_cstr {
	enum {
		value =
			is_cstr_ptr<TT>::value
			|| 
			( std::is_array<TT>::value &&  std::is_same<char, rm_qualifier<typename std::remove_extent<TT>::type>>::value )
	};
};




/////////////////////////////////////////////////////////////////////////////////////////////////  DEF_HAS_ ...


/*
#define DEF_HAS_TYPE(NAME,MEMBER)										\
	namespace detail {											\
		template <class T>                                std::false_type	NAME##_ol(...);		\
		template <class T, class M = typename T::MEMBER>  std::true_type	NAME##_ol(T* t);	\
	}; 													\
	template<class T> constexpr bool NAME(const T& t)  { return  decltype(detail::NAME##_ol<rm_qualifier<T>>(0))::value; }; \
	template<class T> constexpr bool NAME()            { return  decltype(detail::NAME##_ol<rm_qualifier<T>>(0))::value; };



#define DEF_HAS_METHOD(NAME,METHOD)										\
	namespace detail {											\
		template <class T>					std::false_type	NAME##_ol(...);		\
		template <class T, class F = decltype (((T*)0)->METHOD)>std::true_type	NAME##_ol(T* u);	\
	}; 													\
	template<class T> constexpr bool NAME(T t)   { return  decltype(detail::NAME##_ol<rm_qualifier<T>>(0))::value; };\
	template<class T> constexpr bool NAME()      { return  decltype(detail::NAME##_ol<rm_qualifier<T>>(0))::value; };
*/

#define DEF_HAS_TYPE(NAME,MEMBER)										\
	namespace detail {											\
		template <class T>                                std::false_type	NAME##_ol(...);		\
		template <class T, class M = typename T::MEMBER>  std::true_type	NAME##_ol(T* t);	\
	}; 													\
	template<class T> struct  NAME { enum { value = decltype(detail::NAME##_ol<rm_qualifier<T>>(0))::value }; };



#define DEF_HAS_METHOD(NAME,METHOD)										\
	namespace detail {											\
		template <class T>					std::false_type	NAME##_ol(...);		\
		template <class T, class F = decltype (((T*)0)->METHOD)>std::true_type	NAME##_ol(T* u);	\
	}; 													\
	template<class T> struct  NAME { enum { value = decltype(detail::NAME##_ol<rm_qualifier<T>>(0))::value }; };

DEF_HAS_TYPE(has_iterator,iterator)
DEF_HAS_TYPE(has_iterator_category,iterator_category)
DEF_HAS_TYPE(has_result_type,result_type)
DEF_HAS_TYPE(is_map,mapped_type)
DEF_HAS_TYPE(is_lambda_functor,is_lambda_functor)
DEF_HAS_METHOD(is_fold_functor,fold_init_value(T()))
DEF_HAS_METHOD(has_push_front,push_front(typename T::value_type()))
DEF_HAS_METHOD(has_push_back,push_back(typename T::value_type()))
DEF_HAS_METHOD(has_push,push(typename rm_ref<T>::value_type()))
DEF_HAS_METHOD(has_1arg_insert,insert(typename T::value_type()))
DEF_HAS_METHOD(has_insert,insert(typename T::iterator(), typename T::value_type()))
DEF_HAS_METHOD(has_pop_back,pop_back())
DEF_HAS_METHOD(has_pop_front,pop_front())
DEF_HAS_METHOD(has_pop,pop())
DEF_HAS_METHOD(has_size,size())
DEF_HAS_METHOD(has_empty,empty())
DEF_HAS_METHOD(has_resize,resize(size_t()))
DEF_HAS_METHOD(has_back,back())
DEF_HAS_METHOD(has_front,front())
DEF_HAS_METHOD(has_top,top())
DEF_HAS_METHOD(has_clear,clear())
DEF_HAS_METHOD(has_erase2,erase(rg_iterator<T>(), rg_iterator<T>()))
DEF_HAS_METHOD(has_erase1,erase(rg_iterator<T>()))


//////////////////////////////////////////////////////////////////////////////////////// IS_ITERABLE

	template <typename T>
struct is_range_t {

		template <
			class U,
			class I = typename U::const_iterator,
			class B = decltype(std::declval<U>().begin())
		>
		static int8_t
	test(U* u);

		template <typename U>
		static int16_t
	test(...);

	enum { value  =  sizeof test <T> (0) == 1 };
};



template<typename T, size_t N>	struct  is_range_t <T[N]>		: std::true_type {};
template<typename T, size_t N>	struct  is_range_t <std::array<T,N>>	: std::true_type {};

// cstr
template<            size_t N>	struct  is_range_t <char      [N]>    	: std::true_type {};
template<            size_t N>	struct  is_range_t <char const[N]>     	: std::true_type {};
template<>	                struct  is_range_t <char      *>       	: std::true_type {};
template<>	                struct  is_range_t <char const*>       	: std::true_type {};

//template<class T>		struct  is_range { enum { value = is_range_t<rm_qualifier<T>>::value  || is_cstr<T>::value};};
template<class T>		struct  is_range { enum { value = is_range_t<rm_qualifier<T>>::value};};
//template<typename T>     constexpr bool   is_range()        { return  is_range_t<rm_qualifier<T>>::value; };


//////////////////////////////////////////////////////////////////////////////////////  IS STRING
template<class T>	struct  is_string_t				: std::false_type {};
template<class CharT>	struct  is_string_t <std::basic_string<CharT>>	: std::true_type  {};
template<size_t N>	struct  is_string_t <char[N]>			: std::true_type  {};
template<size_t N>	struct  is_string_t <wchar_t[N]>       		: std::true_type  {};
template<class T>	struct  is_string { enum { value = is_string_t<rm_qualifier<T>>::value };};

//template<class T>     constexpr bool   is_string()        { return  is_string_t<rm_qualifier<T>>::value; };

//////////////////////////////////////////////////////////////////////////////////////  IS PAIR/TUPLE
template<class T>		struct  is_pair_t			: std::false_type {};
template<class T1, class T2>	struct  is_pair_t <std::pair<T1,T2>>	: std::true_type  {};
template<class T>		struct  is_pair { enum { value = is_pair_t<rm_qualifier<T>>::value };};


template<class T>		struct  is_tuple_t				: std::false_type {};
template<class ... Ts>		struct  is_tuple_t <std::tuple<Ts...>>		: std::true_type  {};
template<class T>		struct  is_tuple { enum { value = is_tuple_t<rm_qualifier<T>>::value };};

template<class T>		struct  is_tuple_or_pair { enum { value = is_tuple_t<rm_qualifier<T>>::value || is_pair_t<rm_qualifier<T>>::value};};



//////////////////////////////////////////////////////////////////////////////////////  IS_IOABLE
			// alt impl:  http://stackoverflow.com/questions/257288/is-it-possible-to-write-a-c-template-to-check-for-a-functions-existence/9154394#9154394
template<class T>			struct  is_ioable_t    					: std::conditional<std::is_arithmetic<T>::value, std::true_type, std::false_type>::type  {};
template<class T,class CT,class AL>	struct  is_ioable_t <std::basic_string<T,CT,AL>>	: std::true_type  {};

template<size_t N>			struct  is_ioable_t <char[N]>				: std::true_type  {};
template<>				struct  is_ioable_t <char*>				: std::true_type  {};
template<>				struct  is_ioable_t <const char*>			: std::true_type  {};

template<size_t N>			struct  is_ioable_t <wchar_t[N]>				: std::true_type  {};
template<>				struct  is_ioable_t <wchar_t*>				: std::true_type  {};
template<>				struct  is_ioable_t <const wchar_t*>			: std::true_type  {};

template<class T>	struct  is_ioable { enum { value = is_ioable_t<rm_qualifier<T>>::value };};
//template<class T>     constexpr bool   is_ioable()        { return  is_ioable_t<rm_qualifier<T>>::value; };


//////////////////////////////////////////////////////////////////////////////////////  IS_STACK
	template <class T>
struct is_stack {

		template <
			typename U,
			typename POP	= decltype (((U*)0)->pop()),
			typename TOP	= decltype (((U*)0)->top())
		>
		static int8_t
	test(U* u);

		template <class U>
		static int16_t
	test(...);

	enum { value = sizeof test<rm_qualifier<T>>(0) == 1 };
};
//template<class T>     constexpr bool   is_stack()        { return  is_stack_t<rm_qualifier<T>>::value; };


//////////////////////////////////////////////////////////////////////////////////////  IS_QUEUE
	template <class T>
struct is_queue {

		template <
			typename U,
			typename POP	= decltype (((U*)0)->pop()),
			typename FRONT	= decltype (((U*)0)->front()),
			typename BACK	= decltype (((U*)0)->back())
		>
		static int8_t
	test(U* u);

		template <typename U>
		static int16_t
	test(...);

	enum { value = sizeof test<rm_qualifier<T>>(0) == 1 };
};
//template<typename T>     constexpr bool   is_queue()        { return  is_queue_t<rm_qualifier<T>>::value; };



//////////////////////////////////////////////////////////////////////////////////////  IS_ITERATOR

// TO CHECK: better(?) is_iterator --
// 	http://stackoverflow.com/questions/12161109/stdenable-if-or-sfinae-for-iterator-or-pointer
//	http://gcc.gnu.org/bugzilla/show_bug.cgi?id=40497#c23

/*
	template<typename T>    
	constexpr bool  
is_iterator()        {
	return  has_iterator_category<T>() 
		|| (std::is_pointer<T>::value  &&  ! std::is_function<typename std::remove_pointer<T>::type>::value);
};*/

	template<typename T>    
struct is_iterator   {
	enum { value =
		has_iterator_category<T>::value 
		|| (std::is_pointer<T>::value  &&  ! std::is_function<typename std::remove_pointer<T>::type>::value)
	};
};

	template<typename T>
struct is_input_iterator_t {
					static int16_t	test (...);					// no match
	template<typename  U>		static int8_t	test (decltype(std::declval<U>()++)**);		// Pointer
	template<typename  U, size_t N>	static int8_t	test (decltype(std::declval<U>()++)(*)[N]);	// C-array
	template<typename  U>		static 
		eIF<std::is_base_of<std::input_iterator_tag, typename U::iterator_category>::value, int8_t>
							test (U*);					// Iterator 

	static const bool value = sizeof(test((typename std::remove_reference<T>::type*)nullptr)) == 1;
};

	template<class T, class TAG> 	
struct is_iterator_of {
				static int16_t	test (...);					// no match
	template<typename U>	static eIF<std::is_pointer<U>::value && ! std::is_function<typename std::remove_pointer<T>::type>::value,
										std::random_access_iterator_tag>	test (U*);	// Pointer
	template<typename U>	static eIF<std::is_array<U>::value,		std::random_access_iterator_tag>	test (U*);	// C-array
	template<typename U>	static eIF<has_iterator_category<U>::value,	typename U::iterator_category>		test (U*);	// Iterator 

	enum { value = std::is_base_of<TAG, decltype(test((rm_ref<T>*)nullptr))>::value };
	typedef is_iterator_of	type;
};

template<class T> using  is_input_iterator		= typename is_iterator_of<T,std::input_iterator_tag>::type;
template<class T> using  is_output_iterator		= typename is_iterator_of<T,std::output_iterator_tag>::type;
template<class T> using  is_forward_iterator		= typename is_iterator_of<T,std::forward_iterator_tag>::type;
template<class T> using  is_bidirectional_iterator	= typename is_iterator_of<T,std::bidirectional_iterator_tag>::type;
template<class T> using  is_random_access_iterator	= typename is_iterator_of<T,std::random_access_iterator_tag>::type;


	template<class IT, class T=decltype(*std::declval<IT>())>    
	constexpr bool  
is_const_iterator() {	// does not answer if this is CI, but if IT is const or not
	return  ! std::is_assignable <
			decltype( *std::declval<IT>() ),
			T
		  >::value;
}


////////////////////////////////////////////////////////////////////////////////////////  IS_CALLABLE

// see also http://stackoverflow.com/questions/5100015/c-metafunction-to-determine-whether-a-type-is-callable

template<class F, class Sig> struct is_callable; 

		template<class F, class R, class... Args>
struct is_callable<F, R(Args...)> {					// plain callable

		template<class>   static int8_t
	test(...){return int8_t();};

		template<class U> static
		eIF<
			//old old 	std::is_same<decltype(std::declval<rm_qualifier<U>>()(std::declval<Args>()...)), R>::value,
			//old 		std::is_same<decltype(std::declval<U>()(std::declval<Args>()...)), R>::value,
			std::is_convertible<decltype(std::declval<U>()(std::declval<Args>()...)),R>::value,
			int16_t
		>
	test(int){return int16_t();};

	enum { value = (sizeof(test<F>(0)) == sizeof(int16_t)) } ;
};

		template<class F, class R, class... Args>
struct is_callable<F, R(*)(Args...)> {					// pointer to callable

		template<class>   static int8_t
	test(...);

		template<class U> static
		eIF<
			std::is_convertible<decltype(std::declval<U>()(std::declval<Args>()...)),R>::value,
			int16_t
		>
	test(int);

	enum { value = (sizeof(test<F>(0)) == sizeof(int16_t)) } ;
};

		template<class F, class R, class... Args>
struct is_callable<F, R(&)(Args...)> {					// ref to callable

		template<class>   static int8_t
	test(...);

		template<class U> static
		eIF<
			std::is_convertible<decltype(std::declval<U>()(std::declval<Args>()...)), R> ::value,
			int16_t
		>
	test(int);

	enum { value = (sizeof(test<F>(0)) == sizeof(int16_t)) } ;
};

		template<class F, class R, class... Args>
struct is_callable<F, R(&&)(Args...)> {					// rval-ref to callable

		template<class>   static int8_t
	test(...);

		template<class U> static
		eIF<
			std::is_convertible<decltype(std::declval<U>()(std::declval<Args>()...)), R> ::value,
			int16_t
		>
	test(int);

	enum { value = (sizeof(test<F>(0)) == sizeof(int16_t)) } ;
};


/////////////////////////////////////////////////////////////////// RANGE GENERICS: ENDZ, SIZE, CLEAR
									// not really a meta functions

/////  ENDZ - like std::end() but type const char[] is assumed to be C-string and its corresponding correct end (at '\0') is returned
template<class Rg>	auto  endz(Rg&& rg)                 -> decltype(std::end(rg))    { return  std::end(rg); };
__attribute__((unused)) auto  endz(char      * p)           -> char      *               { auto e=p; while(*e) ++e;   return e; };
__attribute__((unused)) auto  endz(char const* p)           -> char const*               { auto e=p; while(*e) ++e;   return e; };
template<size_t N>	auto  endz(char const (&array)[N] ) -> decltype(std::end(array)) { return  std::find(std::begin(array), std::end(array),'\0'); };
template<size_t N>	auto  endz(char       (&array)[N] ) -> decltype(std::end(array)) { return  std::find(std::begin(array), std::end(array),'\0'); };

// BEGINZ
template<class Rg>	auto  beginz(Rg&& rg)                 -> decltype(std::begin(rg))  { return  std::begin(rg); };
__attribute__((unused)) auto  beginz(char      * p)           -> char      *               { return  p; };
__attribute__((unused)) auto  beginz(char const* p)           -> char const*               { return  p; };
template<size_t N>	auto  beginz(char const (&array)[N] ) -> decltype(std::begin(array)) { return  std::begin(array); };
template<size_t N>	auto  beginz(char       (&array)[N] ) -> decltype(std::begin(array)) { return  std::begin(array); };
/*
*/

/////  SIZE

struct size_fo {
		typedef 	size_t 	result_type;

	template<class Rg>    eIF<has_size<Rg>::value, size_t>	size_impl (const Rg& rg)  const		   { return  rg.size(); };
	//template<class Rg>		auto	size_impl (const Rg& rg) ->decltype(rg.size())  const      { return rg.size(); };
	template<class T, size_t N>    eIF<!is_cstr<T[N]>::value, size_t>  constexpr  size_impl (const T    (&C)[N]) const                       { return N; };
	template<class T, size_t N>	size_t constexpr  size_impl (const std::array<T,N>& A) const                 { return N; };
	template<class... Types>	size_t 		  size_impl (const typename std::tuple<Types...>& Tpl) const { return  std::tuple_size<std::tuple<Types...> >::value; };
	template<class U, class V>	size_t constexpr  size_impl (const std::pair<U,V>& P) const                  { return 2; };

	template<class Rg>    eIF<is_cstr<Rg>::value, size_t>	size_impl (const Rg& rg)  const		   { return  endz(rg)-beginz(rg); };
	//template<class T, size_t N>	size_t 		  size_impl (const char (&C)[N]) const                       { return ro::endz(C) - beginz(C); };

	template<class X>		size_t	operator() (const X& x)  const                             { return size_impl(x); };

};

 __attribute__((unused))
size_fo  size;

//template<class X>   		size_t			size 	(const X& x)  { return size_impl(x); };


/////  EMPTY
template<typename Rg>	eIF< has_empty<Rg>::value,bool>	empty(const Rg& rg)	{ return  rg.empty(); }
template<typename Rg>	eIF<!has_empty<Rg>::value,bool>	empty(const Rg& rg)	{ return  ro::size(rg)==0; }

/////  CLEAR
template<typename Rg>	eIF< has_clear<Rg>::value>	clear(Rg&& rg) 		{ rg.clear(); }
template<typename Rg>	eIF<!has_clear<Rg>::value>	clear(Rg&& rg) 		{}
 __attribute__((unused))                     void	clear(char*rg) 		{ *rg = '\0'; }
/////  RESIZE
template<typename Rg>	eIF< has_resize<Rg>::value>	resize(Rg&& rg, size_t n) 		{ rg.resize(n); }
template<typename Rg>	eIF<!has_resize<Rg>::value>	resize(Rg&& rg, size_t n) 		{}
__attribute__((unused))                      void	resize(char*rg, size_t n) 		{ *(rg+n) = '\0'; }


/////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>  constexpr bool   is_collection()     {
	return      is_range<T>::value 
		||  is_stack<T>::value 
		||  is_queue<T>::value 
		||  is_range<T>::value
	;
 };

//template<typename T, typename Rg>   constexpr bool   is_elem_of()        { return  is_collection<Rg>()  &&  std::is_same<rm_ref<T>, rm_ref<rg_elem_type<Rg>>>::value; }
template<typename T, typename Rg>                 struct is_elem_of { enum { value = is_collection<Rg>()  &&  std::is_same<rm_qualifier<T>, rm_qualifier<rg_elem_type<Rg>>>::value }; };
template<typename T, typename Rg>                 struct is_convertible_to_elem_of { enum { value = std::is_convertible<T,rg_elem_type<Rg>>::value}; };

//template<class Rg1, class Rg2>      constexpr bool  have_same_elems()      { return  is_range<Rg1>::value  &&  is_range<Rg2>::value  &&  std::is_convertible< rm_qualifier<rg_elem_type<Rg1>>,  rm_qualifier<rg_elem_type<Rg2>> >::value; }
  template<class Rg1, class Rg2>              struct  have_same_elems { enum { value = is_range<Rg1>::value  &&  is_range<Rg2>::value  &&  std::is_convertible< rm_qualifier<rg_elem_type<Rg1>>,  rm_qualifier<rg_elem_type<Rg2>> >::value }; };
  template<class Rg1, class Rg2>              struct  have_convertible_elems { enum { value = is_range<Rg1>::value  &&  is_range<Rg2>::value  &&  std::is_convertible< rm_qualifier<rg_elem_type<Rg1>>,  rm_qualifier<rg_elem_type<Rg2>> >::value }; };
  								// TO THINK ABOUT:  this test convertion to one side. How about other direction?



template<class Rg>		struct is_ro_range               		: std::false_type {};
template<class It>		struct is_ro_range_iterator      		: std::false_type {};

/////////////////////////////////////////////////////////////////////////////////////////  REF CONTAINER

template<class T>	struct  ref_container;

template<class T>	struct  ref_container<T& >  {
	typedef void is_lvalue; 
	typedef T& value_type; 
	T& value; 
	explicit ref_container(T&  x) : value(x)         {}
 };

template<class T>	struct  ref_container<T const& >  {
	typedef void is_lvalue; 
	typedef T const& value_type; 
	T const& value; 
	explicit ref_container(T const& x) : value(x)         {}
 };

template<class T>	struct  ref_container<T&&>  {
	typedef void is_rvalue;
	typedef T value_type; 
        T  value;	
	explicit ref_container(T&& x) : value(std::move(x)) {}
 };

template<class T>	struct  ref_container<T const&&>  {
	typedef void is_rvalue;
	typedef T value_type; 
        T const value;	
	explicit ref_container(T const && x) : value(x) {}
 };

// 2								// ugly - TODO refactor
template<class T>	struct  ref_container2;

template<class T>	struct  ref_container2<T& >  {
	typedef void is_lvalue; 
	typedef T& value_type2; 
	T& value2; 
	explicit ref_container2(T&  x) : value2(x)         {}
 };

template<class T>	struct  ref_container2<T&&>  {
	typedef void is_rvalue;
	typedef T&& value_type2; 
	rm_ref<T>  value2;	
	explicit ref_container2(T&& x) : value2(std::move(x)) {}
 };

					};
					};
					#endif
