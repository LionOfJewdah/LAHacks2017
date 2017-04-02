/*
 *
 *----------------------------------------------------------------------------
 * Rubik Cube solver.
 * Copyright (C) Marek Karcz 2015. All rights reserved.
 *
 * License agreement between the User and the Author:
 *
 * This software and its source code is free to use and distribute
 * for non-commercial purposes only and as long as following conditions
 * are met:
 *
 * - this copyright notice in its entirety along with
 *   the license agreement and disclaimer notice are included
 *   in the distributed package,
 *
 * - the source code is included in distributed package.
 *
 * User is free to modify the code and distribute the modified code
 * as long as the modifications are properly documented and also
 * included in the distributed package.
 * Following forms of documentation of the changes to the code are
 * accepted:
 *
 * - source code comments,
 *
 * - description of the changes in the form of a readme document
 *   in any commonly used format (text, ms office, open office).
 *
 * Disclaimer:
 * 
 * No warranty is offered in any form. Author can not be held
 * responsible for any damage caused by the software.
 * Use at your own risk.
 *----------------------------------------------------------------------------
 *
 */
 
#ifndef _EXCEPTION_H_INCLUDED
#define _EXCEPTION_H_INCLUDED

#include <string>
#include <exception>

using namespace std;

namespace Rubik {
	
class Exception : public exception {
	
	public:
		
		Exception();
		Exception(string cause);
		~Exception() throw() {};
		string GetCause();
		string what();
		void Print();
		
	private:
		
		string	msCause;
};
	
} // namespace Rubik

#endif // _EXCEPTION_H_INCLUDED
