// include all RO headers

#ifndef  RO_H
#define  RO_H

#include <ro/meta.h>
#include <ro/functor.h>

#include <ro/range.h>
#include <ro/stl.h>
#include <ro/io.h>
#include <ro/regex.h>

#ifndef scc_BOOST_LAMBDA
	// lambda must be after stl.h, io.h, range,h
	#include <ro/lambda.h>
#endif

#ifndef NDEBUG
#include "ro/debug.h"
#endif


//// CHECK IF C++11 MODE

// __cplusplus = 199711L / 201103L is only as of GCC 4.7.0 and higher, unless the patch gets backported. 4.6 and earlier use __cplusplus = 1


// GCC
#ifdef __GNUC__

	#define GNUC_VER (__GNUC__ * 100 + __GNUC_MINOR__)

	/* WHY IS THIS NOT WORKING
	 * #if  GNUC_VER < 470
		#error  GNUC_VER "GCC of this version does not support RO"
	#endif*/

	#if  !defined(__GXX_EXPERIMENTAL_CXX0X__) 
		#error  "GCC: not C++11 mode (needed by RO)"
	#endif
#endif



// CLANG
/*  HOTO MAKE THIS TEST?
#if  defined(__clang__)  &&  !__has_feature(cxx_decltype)
	#error  "CLANG: not a C++11 mode "
#endif
*/

// MSVC (not tested)
#if  defined(_MSC_VER)  &&  _MSC_VER < 1600
	#error  "MSC: compiler does not support C++11"
#endif


#endif // RO_H
