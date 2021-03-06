#pragma once
/////////////////////////////////////////////////////////////////////
// TestRequest.cpp - Creates and parses Test Requests              //
// ver 1.0                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////

#include "TestRequest.h"
#include "Process.h"
#include <iostream>
//#define TEST_TESTREQUEST

#ifdef TEST_TESTREQUEST

using namespace Testing;

int main()
{
	Utilities::title("Create TestRequest and Serialize:");

	TestRequest tr1;
	DateTime dt;
	tr1.name("tr1");
	tr1.author("Yuexin");
	tr1.date(dt.now());
	tr1.addDll("../TestDrivers/TestDriver1.dll");
	tr1.addDll("../TestDrivers/TestDriver2.dll");
	tr1.addDll("../TestDrivers/TestDriver3.dll");
	tr1.addDll("../TestDrivers/TestDriver4.dll");

	std::string tr1Str = tr1.toString();
	std::cout << "\n  " << tr1Str;
	std::cout << "\n";


	Utilities::title("Create new TestRequest from Serialized String");

	TestRequest tr2;
	tr2 = tr2.fromString(tr1Str);
	std::cout << "\n  tr2.name() = " << tr2.name();
	std::cout << "\n  tr2.author() = " << tr2.author();
	std::cout << "\n  tr2.date() = " << tr2.date();

	for (auto item : tr2.request())
	{
		std::cout << "\n  dll = " << item;
	}
	std::cout << "\n";


	return 0;
}

#endif