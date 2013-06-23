				#include <scc/simple.h>
				#include <lvv/check.h>
				#include <ro/debug.h>
				#include <ro/ro.h>
				using namespace ro;

		///// func types (for testing is_callable)
		bool   f(int )                         { return true; };
		struct fo_t	{ bool operator()(int) { return true; }; };
		struct mfo_t	{ bool mf(int)         { return true; }; };
		struct empty_t  {};
		auto lam = [](int x)->bool { return x==0; };
		typedef decltype(lam) lam_t;

				int main () {


//  CL_ELEM_TYPE
CHECK_TYPES_ARE_SAME(   rg_elem_type<vint>        , int)
CHECK_TYPES_ARE_SAME(   rg_elem_type<vint&>       , int)
CHECK_TYPES_ARE_SAME(   rg_elem_type<vint&&>      , int)

CHECK_TYPES_ARE_SAME(   rg_elem_type<const vint>  , int)
CHECK_TYPES_ARE_SAME(   rg_elem_type<const vint&> , int)
CHECK_TYPES_ARE_SAME(   rg_elem_type<const vint&&>, int)

CHECK_TYPES_ARE_SAME(   rg_elem_type<int(&)[2]>   , int)
CHECK_TYPES_ARE_SAME(   rg_elem_type<int*(*)(int)>, no_type)
CHECK_TYPES_ARE_SAME(   rg_elem_type<int>         , no_type)


//  IS_RANGE
CHECK(   is_range<vint>::value         ) 
CHECK(   is_range<vint&>::value        ) 
CHECK(   is_range<vint&&>::value       ) 

CHECK(   is_range<const vint>::value   ) 
CHECK(   is_range<const vint&>::value  ) 
CHECK(   is_range<const vint&&>::value ) 

CHECK(   is_range<decltype(range(1,2))>::value   ) 
CHECK(   is_range<decltype(range(v9))>::value  ) 
CHECK(   is_range<decltype(range(range(v9)))>::value  ) 
CHECK(   is_range<decltype(range(+v9,-v9))>::value ) 
CHECK(   is_range<ro::basic_range<vector<int>>&&>::value )
CHECK(   is_range<vector<int>&>::value)

CHECK( ! is_range<int*(*)(int)>::value ) 
CHECK( ! is_range<int>::value          ) 
CHECK(   is_range<int(&)[2]>::value    ) 

CHECK(   is_range<decltype(range(v9))>::value    ) 
CHECK( ! is_range<decltype(+range(v9))>::value    ) 
CHECK( ! (is_range<std::move_iterator<std::__cxx1998::_Deque_iterator<long int, long int&, long int*> >::iterator_type>::value))
CHECK( ! (is_range<std::__cxx1998::_Deque_iterator<long int, long int&, long int*>>::value))


//  IS_STRING
CHECK(   is_string<decltype("abc")>::value) 
CHECK(   is_string<char[3]>::value) 
CHECK( ! is_string<unsigned char[3]>::value) 
CHECK( ! is_string<signed char[3]>::value) 
CHECK( ! is_string<int[3]>::value) 
CHECK(   is_string<char(&)[3]>::value) 
//CHECK( ! is_string<const char[3]>::value) 
//CHECK( ! is_string<char*>::value) 
//CHECK(   is_string<const char*>()) 
CHECK(   is_string<string>::value) 
CHECK(   is_string<const string&>::value) 


//  IS_CSTR
CHECK(   is_cstr_ptr<char*>          ::value)
CHECK(   is_cstr_ptr<const char*>    ::value)
CHECK( ! is_cstr_ptr<int*>    ::value)

CHECK(   is_cstr<decltype( "abc")>   ::value)
CHECK(   is_cstr<char[3]>            ::value)
CHECK( ! is_cstr<unsigned char[3]> ::value)
CHECK( ! is_cstr<signed char[3]>   ::value)
CHECK( ! is_cstr<int[3]>           ::value)
CHECK(   is_cstr<char(&)[3]>         ::value)
CHECK(   is_cstr<const char[3]>      ::value)
CHECK(   is_cstr<char*>            ::value)
CHECK(   is_cstr<const char*>        ::value)
CHECK( ! is_cstr<string>           ::value)
CHECK( ! is_cstr<const string&>    ::value)

//  HAS_PUSH_BACK
CHECK(   has_push_back<vint>::value			)
CHECK(   has_push_back<vint&>::value			)
CHECK(   has_push_back<vint&&>::value		)
CHECK(   has_push_back<const vint&>::value		)
CHECK(   has_push_back<const vint&&>::value		)
CHECK(   has_push_back<volatile vint&>::value	)
CHECK( ! has_push_back<int(&)[2]>::value		)
CHECK( ! has_push_back<forward_list<int>>::value	)
CHECK( ! has_push_back<stack<int>>::value		)
CHECK(   has_push_back<string>::value		)
CHECK( ! has_push_back<char(&)[2]>::value		)
CHECK( !(has_push_back<map<int,int>>::value)		)


//  REFNESS
CHECK_TYPES_ARE_SAME(    rg_traits<vint>::elem_type,		int	)
CHECK_TYPES_ARE_SAME(    rg_traits<vint&>::elem_type,		int	)
CHECK_TYPES_ARE_SAME(    rg_traits<vint&&>::elem_type,		int	)

CHECK_TYPES_ARE_SAME(    rg_traits<const vint>::elem_type,	int	)
CHECK_TYPES_ARE_SAME(    rg_traits<const vint&>::elem_type,	int	)
CHECK_TYPES_ARE_SAME(    rg_traits<const vint&&>::elem_type,	int	)


//  HAVE_SAME_ELEM
CHECK( ! (have_same_elem<vint, int>::value)			)

CHECK(   (have_same_elem<vint, int[3]>::value)		)
CHECK(   (have_same_elem<vint, int(&)[3]>::value)		)
CHECK(   (have_same_elem<vlong, int(&)[3]>::value)		)

CHECK(   (have_same_elem<vint, vlong>::value)		)
CHECK(   (have_same_elem<vint, llong>::value)		)
CHECK(   (have_same_elem<vint, vint&&>::value)		)
CHECK(   (have_same_elem<vint, vint&&>::value)		)
CHECK(   (have_same_elem<vint, vint&&>::value)		)
CHECK(   (have_same_elem<vint, basic_range<vint>>::value)	)
CHECK(   (have_same_elem<vint, basic_range<vint&>&>::value)	)
CHECK(   (have_same_elem<vint, basic_range<vint&>&>::value)	)

CHECK(   (have_same_elem<vchar, dchar>::value)	)
CHECK(   (have_same_elem<vchar, char[3]>::value)	)
CHECK(   (have_same_elem<vchar, char*>::value)	)

// ITERATORS
CHECK(   is_input_iterator<vint::const_iterator>::value	)

CHECK( ! is_input_iterator         <void>  		::value)
CHECK( ! is_random_access_iterator <int>		::value)
CHECK(   is_input_iterator         <char*> 		::value)
CHECK(   is_input_iterator         <char[]>		::value)
CHECK( ! is_input_iterator         <std::true_type>	::value)
CHECK( ! is_input_iterator         <int(*)(int)>	::value)

CHECK( ! is_input_iterator         <int>         	::value)
CHECK(   is_input_iterator         <vint::iterator>	::value)
CHECK(   is_bidirectional_iterator <vint::iterator>	::value)
CHECK(   is_input_iterator         <lint::iterator>	::value)
CHECK(   is_forward_iterator       <lint::iterator>	::value)
CHECK(   is_bidirectional_iterator <lint::iterator>	::value)
CHECK( ! is_random_access_iterator <lint::iterator>	::value)
CHECK(   is_random_access_iterator <vint::iterator>	::value)
CHECK(   is_random_access_iterator <int*>  		::value)
CHECK(   is_random_access_iterator <int[3]>		::value)

// constess of iterator
CHECK(   is_const_iterator<vint::const_iterator>()	)
CHECK( ! is_const_iterator<vint::iterator>()		)
CHECK(   is_const_iterator<const int*>()		)
CHECK( ! is_const_iterator<int*>()			)
CHECK(   (is_const_iterator<std::array<int,3>::const_iterator>()))
CHECK( ! (is_const_iterator<std::array<int,3>::iterator>()))

// basic_range
CHECK(   (is_ro_range_iterator	<decltype(+range(vint{1}))>::value))
CHECK( ! (is_ro_range_iterator	<decltype( range(vint{1}))>::value))
CHECK(   (is_random_access_iterator	<decltype(+range(vint{1}))>::value))
CHECK( ! (is_random_access_iterator	<decltype(+range(lint{1}))>::value))
CHECK(   (is_bidirectional_iterator	<decltype(+range(vint{1}))>::value))
CHECK( ! (is_ro_range		<decltype(+range(vint{1}))>::value))
CHECK(   (is_ro_range		<decltype( range(vint{1}))>::value))
CHECK(   (is_range			<decltype( range(vint{1}))>::value))
CHECK( ! (is_range			<decltype(+range(vint{1}))>::value))

// cstr
CHECK(   (is_range			<char*>::value))
CHECK(   (is_range			<char[3]>::value))

// numeric_range
CHECK(   (is_numeric_range_iterator	<decltype(+range(2))>::value))
CHECK( ! (is_numeric_range_iterator	<decltype( range(2))>::value))
CHECK( ! (is_numeric_range		<decltype(+range(2))>::value))
CHECK(   (is_numeric_range		<decltype( range(2))>::value))
CHECK(   (is_random_access_iterator	<decltype(+range(2))>::value))
CHECK(   (is_range			<decltype( range(2))>::value))
CHECK( ! (is_range			<decltype(+range(2))>::value))

// erasable_category
CHECK(   (is_same<non_erasable,decltype(erasable_category(1))>::value))
CHECK(   (is_same<cstr_erasable,decltype(erasable_category("abc"))>::value))
CHECK(   (is_same<cstr_erasable,decltype(erasable_category(c9))>::value))
CHECK(   (is_same<vector_erasable,decltype(erasable_category(v9))>::value))

// TODO  FIGURE OUT WHY THIS IS FAILTING
// CHECK(   (is_same<basic_range_erasable,decltype(erasable_category(range(c9)))>::value))

// pair / tuple
CHECK( ! (is_pair<void>::value))
CHECK( ! (is_pair<int>::value))
CHECK( ! (is_pair<std::tuple<int,int>>::value))
CHECK(   (is_pair<std::pair<int,int>>::value))
CHECK(   (is_tuple<std::tuple<int,int>>::value))

// is_fold_functor
CHECK(   (is_fold_functor<add_t>::value))
CHECK( ! (is_fold_functor<std::plus<int>>::value))
CHECK( ! (is_fold_functor<int>::value))

// IS_CALLABLE
		
CHECK( ! (is_callable<bool,  bool(int)>::value))		// is not
CHECK( ! (is_callable<int,   bool(int)>::value))
CHECK( ! (is_callable<void*, bool(int)>::value))
CHECK( ! (is_callable<void,  bool(int)>::value))
CHECK( ! (is_callable<bool(),bool(int)>::value))
CHECK(   (is_callable<bool(int), int (int)>::value))
CHECK( ! (is_callable<bool(int), int ()>::value))

CHECK(   (is_callable<bool(int), bool(int)>::value))		// is
CHECK(   (is_callable<bool(int), bool(*)(int)>::value))
CHECK(   (is_callable<bool(int), bool(&)(int)>::value))
CHECK(   (is_callable<bool(int), bool(char)>::value))
CHECK(   (is_callable<bool(int), bool(const int)>::value))
CHECK(   (is_callable<bool(int), bool(const int&)>::value))
CHECK(   (is_callable<bool(int), bool(int&)>::value))
CHECK(   (is_callable<bool(int), bool(int&&)>::value))


CHECK( ! (is_callable<bool(int&), bool(int)>::value))		// ref
CHECK( ! (is_callable<bool(int&), bool(const int)>::value))
CHECK( ! (is_callable<bool(int&), bool(const int&)>::value))
CHECK(   (is_callable<bool(int&), bool(int&)>::value))
CHECK( ! (is_callable<bool(int&), bool(int&&)>::value))

CHECK(   (is_callable<decltype(::sin), 	        double(double)>::value))
CHECK(   (is_callable<decltype(f),              bool(int)>::value))
CHECK(   (is_callable<fo_t,                     bool(int)>::value))
CHECK(   (is_callable<std::function<bool(int)>, bool(int)>::value))
CHECK( ! (is_callable<std::function<bool()>,	bool(int)>::value))
//CHECK(   (is_callable<std::function<bool(*)(int)>, bool(int)>::value))
//CHECK(   (is_callable<std::function<lam_t>, bool(int)>::value)) 	// ??????????????????????????
CHECK(   (is_callable<std::negate<int>,		int(int)>::value))
CHECK(   (is_callable<std::less<int>,		bool(int,int)>::value))

// REF_CONTAINER
{ to::instances = 0;       to  lto;  ref_container<      to&>  lrc{ lto};   CHECK_ARE_EQUAL( lto.instances, 1) }
{ to::instances = 0; const to clto;  ref_container<const to&> clrc{clto};   CHECK_ARE_EQUAL(clto.instances, 1) }

to::constructed = 0; 	CHECK_ARE_EQUAL((ref_container<to&&>{to()}).value.constructed, 1)
to::constructed = 0; 	CHECK_ARE_EQUAL((ref_container<to&&>{to()}).value.constructed, 1)

CHECK(   (is_same<decltype(((ref_container<to&&>{to()}).value)),to>::value))

__ "- - - - - - - - - - - - - - - - - - - - - - U-META passed   - -- - - - - - - - - - - - - - - - - - - - -";
						CHECK_EXIT;

					}
