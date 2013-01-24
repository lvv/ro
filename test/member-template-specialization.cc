#include <iostream>
using namespace std;

// NON-TEMPLATE MEM-F
template<class T>  struct S		{ void memf(); };
template<>	   struct S<int>      	{ void memf();  int m=0; };

template<class T>  void S<T>  ::memf()  { cout << " S<T>::memf()\n"; };
		   void S<int>::memf()  { cout << " S<int>::memf()\n\n"; };

// TEMPLATE MEM-F
template<class T>  struct SS		{ template<class U>	void memf(); };
template<>	   struct SS<int>      	{ template<class U>	void memf();  int m=0; };

template<class T>  template<class U> void SS<T>::memf()    { cout << " SS<T>::memf()\n"; };
      	           template<class U> void SS<int>::memf()  { cout << " SS<int>::memf()\n\n"; };

// SPECIALIZATION GROUPING
template<class T>  struct SSS	  	      
template<>	   struct SSS<int>      	{ template<class U>	void memf();  int m=0; };
template<>	   struct SSS<float>      	{ template<class U>	void memf();  int m=0; };

template<class T>  template<class U> void SSS<T>::memf()     { cout << " SSS<T>::memf()\n"; };
//      	           template<class U> void SSS<int>::memf()    { cout << " SSS<int>::memf()\n"; };
 //     	           template<class U> void SSS<float>::memf()  { cout << " SSS<float>::memf()\n"; };


int main() {

	S<float>().memf();
	S<int>().memf();

	SS<float>().memf<double>();
	SS<int>().memf<char>();

	SSS<float>().memf<void>();
	SSS<int>().memf<void>();
}
