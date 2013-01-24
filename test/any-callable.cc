#include <iostream>
#include <vector>
using namespace std;

#include <ro/ro.h>
using namespace ro;




//////////////////////////////////////////////////////////////////////////   UPDATE

		////  UPDATER
		int	 updater(int x) 			{ cout << " updater(int        )   ";	return x+=-1; }
		int&&	 updater(int&& x) 		{ cout << " updater(int&&      )   ";	return move(x+=1); }
		int&	 updater(int& x)			{ cout << " updater(int&       )   ";	return x+=2; }


/* UPDATER       - T&&(*)(T&&)
*/
	template<class T>
	T&& 
apply_update(identity<T>&&(*updater)(identity<T>&&), T&& x) { return updater(FWD(T,x)); } // identity needed for clang


//////////////////////////////////////////////////////////////////////////   REAPLCE
	
		////  REPLACER 
		int   	 replacer(int x) 		{ cout << " replacer(int        )   ";	return -1; }
		int   	 replacer(int& x)		{ cout << " replacer(int&       )   ";	return 2; }
		int   	 replacer(const int& x)		{ cout << " replacer(const int& )   ";	return 20; }
		int   	 replacer(int&& x) 		{ cout << " replacer(int&&      )   ";	return 1; }
		int   	 replacer(const int&& x)       	{ cout << " replacer(const int&&)   ";	return 10; }
		long  	 replacer(long x)        	{ cout << " replacer(long       )   ";	return 42; }


/* OVERLOADS
*/
	template<class T>
	rm_qualifier<T>
apply_replace(rm_qualifier<T>(*replacer)(T&&), T&& x) { return replacer(FWD(T,x)); }

/* LAMBDAS, can't match overloaded
 */
	template<class T, class F>
	eIF<is_callable<F,rm_qualifier<T>(T&&)>::value,rm_qualifier<T>>
apply_replace (F replacer, T&& x) { return replacer(FWD(T,x)); }
                           
//////////////////////////////////////////////////////////////////////////   TRANSFORM

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

	cout << "arg:  int                " << apply_replace([](int  i) ->int{cout << "[](int  )\t\t";  return 11;}, 4)		<< endl;
	cout << "arg:  int&&              " << apply_replace([](int&& i)->int{cout << "[](int&&)\t\t";  return 22;}, int())	<< endl;
	cout << "arg:  int&               " << apply_replace([](int&  i)->int{cout << "[](int& )\t\t";  return 33;}, i)		<< endl;

	cout << "\n***  UPDATE VALUE AT REF \n";
	cout << "arg:  int                " << apply_update(updater, 0)		<< endl;
	cout << "arg:  int&&              " << apply_update(updater, int())	<< endl;
	cout << "arg:  int&               " << apply_update(updater, i)		<< endl;

	cout << "\n***  TRANSFORM TYPE \n";
	cout << "arg:  int                " << apply_transform(transformer, 0)	<< endl;
	cout << "arg:  int&&              " << apply_transform(transformer, int())	<< endl;
	cout << "arg:  int&               " << apply_transform(transformer, i)	<< endl;
	cout << "arg:  const int&         " << apply_transform(transformer, ci)	<< endl << endl;
}

