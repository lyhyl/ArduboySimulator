#ifndef PROTONTESTER_H
#define PROTONTESTER_H

#include "ProtonTester-internal.h"

namespace ProtonTester
{
	void runAllTests();

	unsigned int GetTotalRun();

	unsigned int GetNumPassed();

	unsigned int GetNumFailed();
}

#define TestEq(expected, actual) CmpEq(expected, #expected, actual, #actual, TestLocation(m_testName, __FILE__, __LINE__))

#define TEST(testName) class testName : public TestCase { \
public: \
testName() : TestCase(#testName) {} \
virtual ~testName() {} \
virtual void runTest(); \
}; \
testName g_##testName; \
void testName::runTest()


#endif // PROTONTESTER_H
