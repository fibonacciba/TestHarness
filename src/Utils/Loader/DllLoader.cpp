/////////////////////////////////////////////////////////////////////////
// Author: Fang Wang, Syracuse University
// CSE687 object orriented Design
//
// DllLoader.cpp : implementation and testing for DllLoader.h
////////////////////////////////////////////////////////////////////////

#include "DllLoader.h"
#include <codecvt>
#include <locale>
#include "ITest.h"
#include <iostream>

typedef ITest* (*ITest_Factory)();

using std::exception;

DllLoader::DllLoader(const string& dll_path)
{
	itest_result_ = false;
	dll_path_ = dll_path;
	std::wstring w_dll_path = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(dll_path_);
	hDll_ = LoadLibrary(w_dll_path.c_str());
}

void DllLoader::TestITest()
{
	if (hDll_ == NULL) {
		appendLogMessage("Failed to load DLL file with path: " + dll_path_);
		itest_result_ = false;
	}
	else {
		appendLogMessage("Successful to load DLL file with path: " + dll_path_);
		ITest_Factory createTest = (ITest_Factory)GetProcAddress(hDll_, "createTest");

		if (createTest == NULL) {
			appendLogMessage("Failed to locate the function createTest() in the dll");
			itest_result_ = false;
		}
		else {
			appendLogMessage("Successful to locate the function createTest() in the dll");
			ITest* pTest = createTest();
			//itest_result_ = pTest->test();
			//itest_result_ = pTest->operator()();
			try {
				//itest_result_ = (*pTest)();
				itest_result_ = pTest->test();
				appendLogMessage("Successful execute the callable object without error and return result");
			}
			catch (const exception& ex) {
				appendLogMessage("Execute the callable object throw an error:");
				appendLogMessage(ex.what());
				itest_result_ = false;
			}
			delete pTest;
		}
	}
}

DllLoader::~DllLoader()
{
	FreeLibrary(hDll_);
}


string DllLoader::getDllPath() const
{
	return dll_path_;
}

TestResult DllLoader::getTestResult() const
{
	TestResult testResult{ getDllPath(), getItestBoolResult(), getLogMessage() };
	return testResult;
}

string DllLoader::getLogMessage() const
{
	return log_message_;
}

bool DllLoader::getItestBoolResult() const
{
	return itest_result_;
}

void DllLoader::appendLogMessage(const string& message)
{
	log_message_ += message;
	log_message_ += "\n";
}
