#include <iostream>
#include <vector>
using namespace std;

#include <ro/ro.h>
using namespace ro;


typedef   vector<int>   vint;


//////////////////////////////////////////////////////////////////////////   UPDATE  T&&(T&&)

		////  UPDATER
		int	 updater(int x)        		{ cout << " updater(int  )   ";	return x=1; }
		int&&	 updater(int&& x) 		{ cout << " updater(int&&)   ";	return move(x=2); }
		int&	 updater(int& x)       		{ cout << " updater(int& )   ";	return x=3; }


/* UPDATER       - T&&(*)(T&&)
*/
	template<class T>
	T&& 
apply_update(identity<T>&&(*f)(identity<T>&&), T&& x) { return f(FWD(T,x)); } // identity needed for clang


                           
	template<class T, class F>
	eIF<is_callable<F, T&&(T&&)>::value, T&&>
apply_update (F f, T&& x) { return f(FWD(T,x)); }



/*
                           
	template<class T, class F>
	auto
apply_update (F f, T&& x) 
	-> eIF<is_callable<F, T&&(T&&)>::value, T&&>
	{ return f(FWD(T,x)); }
*/
                           
//////////////////////////////////////////////////////////////////////////   REAPLCE   T(T&&)
	
		////  REPLACER 
		int   	 replacer(int x) 		{ cout << " replacer(int        )   ";	return -1; }
		int   	 replacer(int& x)		{ cout << " replacer(int&       )   ";	return 2; }
		int   	 replacer(const int& x)		{ cout << " replacer(const int& )   ";	return 20; }
		int   	 replacer(int&& x) 		{ cout << " replacer(int&&      )   ";	return 1; }
		int   	 replacer(const int&& x)       	{ cout << " replacer(const int&&)   ";	return 10; }
		long  	 replacer(long x)        	{ cout << " replacer(long       )   ";	return 42; }


/* VALUNESS OVERLOADS
*/
	template<class T>
	rm_qualifier<T>
apply_replace(rm_qualifier<T>(*replacer)(T&&), T&& x) { return replacer(FWD(T,x)); }

/* ABS-like functions
*/
int apply_replace(int(*replacer)(int), int x) { return replacer(x); }	// can catch std::abs(int)

/* LAMBDAS, BIND,  can't match overloaded
 */
	template<class T, class F>
	eIF<is_callable<F,rm_qualifier<T>(T&&)>::value,rm_qualifier<T>>
apply_replace (F replacer, T&& x) { return replacer(FWD(T,x)); }
                           
//////////////////////////////////////////////////////////////////////////   TRANSFORM     R(T&&)

		////  TRANSFORMER 

		//float  	transformer(int x) 		{ cout << " transformer(int        )   ";	return -1.1; }
		//float  	transformer(int& x)		{ cout << " transformer(int&       )   ";	return 2.1; }
		float  		transformer(const int& x)      	{ cout << " transformer(const int& )   ";	return 20.1; }
		//float  	transformer(int&& x) 		{ cout << " transformer(int&&      )   ";	return 1.1; }
		//float  	transformer(const int&& x)     	{ cout << " transformer(const int&&)   ";	return 10.1; }
		//float  	transformer(long x)        	{ cout << " transformer(long       )   ";	return 42.1; }

/* does not match overloads
*/
	template<class T, class F, class R = rm_ref<decltype((std::declval<F>()(std::declval<T>())))>>
	R
apply_transform(F transformer, T&& x) { return transformer(FWD(T,x)); }




int main() {
	int i=0;
	const int ci=0;

cout << "\n*** REPLACE \n";
	cout << "arg:  int                " << apply_replace(replacer, 0)	<< endl;
	//	cout << "arg:  long               " << apply_replace(replacer, 42l)	<< endl;
	cout << "arg:  int&&              " << apply_replace(replacer, int())	<< endl;
	cout << "arg:  int&               " << apply_replace(replacer, i)	<< endl;
	cout << "arg:  const int&         " << apply_replace(replacer, ci)	<< endl;

	cout << "arg:  int                " << apply_replace([](int  i) ->int{cout << "λ [](int  )\t\t";  return 11;}, 4)		<< endl;
	cout << "arg:  int&&              " << apply_replace([](int&& i)->int{cout << "λ [](int&&)\t\t";  return 22;}, int())	<< endl;
	cout << "arg:  int&               " << apply_replace([](int&  i)->int{cout << "λ [](int& )\t\t";  return 33;}, i)		<< endl;

	cout << "arg:  int&               RO λ          " << apply_replace(var(100), i)				<< endl;
	cout << "arg:  int&               bind(plus...) " << apply_replace(bind(std::plus<int>(),_1,200), i)   	<< endl;
	cout << "arg:  int                function<>    " << apply_replace(function<int(int)>([](int i)->int{return 1;}) , -600)		<< endl;

	cout << "arg:  int                ro:abs        " << apply_replace(ro::abs, -300)			<< endl;
	cout << "arg:  int                std::abs      " << apply_replace(std::abs, -400)				<< endl;
	cout << "arg:  int                abs           " << apply_replace(abs, -500)				<< endl;


cout << "\n***  UPDATE REFERENCED VALUE \n";
	cout << "arg:  int                " << apply_update(updater, 0)		<< endl;
	cout << "arg:  int&&              " << apply_update(updater, int())	<< endl;
	cout << "arg:  int&               " << apply_update(updater, i)		<< endl;
	cout << "arg:  int&               RO λ          " << apply_update((_1=2), i)		<< endl;
	cout << "arg:  int&               bind()        " << apply_update(bind(_1,_1), i)     	<< endl;
	cout << "arg:  int&               λ             " << apply_update([](int&  i)->int&{return i+=100;}, i)		<< endl;
	cout << "arg:  int&               function<>    " << apply_update(function<int&(int&)>([](int&i)->int&{return i+=200;}) , i)		<< endl;

	// R-VAL
	cout << "arg:  vint&&             λ(&&)         " << apply_update(     [](vint&&  v)->vint&&{return move(v=vint{99});},      vint{})		<< endl;
	cout << "arg:  vint&&             bind(&&)      " << apply_update(bind([](vint&&  v)->vint&&{return move(v=vint{99});}, _1), vint{})     	<< endl;

	vint lv_v; 
	cout << "arg:  vint&              λ(&)          " << apply_update([](vint&  v)->vint&{return v=vint{99};}, lv_v)		<< endl;
	cout << "arg:  vint&              RO λ(&)       " << apply_update((_1=vint{99}),                            lv_v)       	<< endl;

	// ERROR
	//cout << "arg:  vint&&             RO λ(&&)      " << apply_update((_1=vint{99}),                          vint{})       	<< endl;

cout << "\n***  TRANSFORM TYPE \n";
	cout << "arg:  int                " << apply_transform(transformer, 0)	<< endl;
	cout << "arg:  int&&              " << apply_transform(transformer, int())	<< endl;
	cout << "arg:  int&               " << apply_transform(transformer, i)	<< endl;
	cout << "arg:  const int&         " << apply_transform(transformer, ci)	<< endl << endl;

	//cout << "arg:  int&               RO λ          " << apply_replace(var(100), i)				<< endl;
	//cout << "arg:  int&               bind(plus...) " << apply_replace(bind(std::plus<int>(),_1,200), i)   	<< endl;
	//cout << "arg:  int                function<>    " << apply_replace(function<int(int)>([](int i)->int{return 1;}) , -600)		<< endl;

}

