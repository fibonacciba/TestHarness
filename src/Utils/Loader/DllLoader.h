/////////////////////////////////////////////////////////////////////////
// Author: Fang Wang, Syracuse University
// CSE687 object orriented Design
//
// This file is to load dlls, help each test driver create its own instance of logger
/////////////////////////////////////////////////////////////////////////

#ifndef DLLLOADER_H
#define DLLLOADER_H

#include <string>
#include <Windows.h>
#include <ctime>
#include <stdexcept>

using std::string;

struct TestResult {
	string dll_path_;
	bool test_result_;
	string log_message_;
};

//class DLLLoader will load a DLL from provided path and provide option to test the function inside DLL
class DllLoader {
public:
	DllLoader(const string& dll_path);
	void TestITest();
	~DllLoader();
	TestResult getTestResult() const;

private:
	string dll_path_;
	HINSTANCE hDll_;
	bool itest_result_;
	string log_message_;

	void appendLogMessage(const string& message);
	string getDllPath() const;
	string getLogMessage() const;
	bool getItestBoolResult() const;

};

#endif
