//Dll1.h  Implementations of Dll1
//Fang Wang, Syracuse University


#include "pch.h"
#include "Dll1.h"
#include <new>
#include <iostream>

bool TestClass1::test()
{
	//std::bad_alloc x;
	std::cout << "TestClass1 is started \n";

	/*while (true)
	{
		new int[100000000ul];
	}

	throw x;*/
	std::cout << "The test is passed! \n";

	return true;
}
