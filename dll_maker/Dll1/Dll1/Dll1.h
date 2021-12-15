//Dll1.h  Declaration of Dll1
//Fang Wang, Syracuse University


#ifdef DLL1_EXPORTS
#define DLL1_API __declspec(dllexport)
#else
#define DLL1_API __declspec(dllimport)
#endif

#include "ITest.h"

class TestClass1 : public ITest {

public:
	bool test() override;
};


extern "C" DLL1_API TestClass1 * createTest() {
	return new TestClass1;
};

