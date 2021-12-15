#pragma once
#ifndef ITEST_H
#define ITEST_H

#ifdef TEST_EXPORTS
#define TEST_API __declspec(dllexport)
#else
#define TEST_API __declspec(dllimport)
#endif

class  ITest
{
public:
	virtual ~ITest() {};
	virtual bool test() = 0;
};
#endif