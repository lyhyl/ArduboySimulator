#include "ProtonTester.h"

#include <list>

#include "Entity/EntityUtils.h"


TestResults gTestResults;

TestResults::TestResults()
{
	clear();
}

void TestResults::clear()
{
	m_passedTests.clear();
	m_failedTests.clear();

	m_resultStr.clear();
}

void TestResults::add(const CheckResult &result)
{
	if (result.GetResult() == CheckResult::FAIL)
	{
		gTestResults.m_passedTests.erase(result.GetTestName());
		gTestResults.m_failedTests.insert(result.GetTestName());

		if (!gTestResults.m_resultStr.empty()) {
			gTestResults.m_resultStr.append("\n\n");
		}

		gTestResults.m_resultStr.append(result.GetResultString());
	} else
	{
		if (gTestResults.m_failedTests.count(result.GetTestName()) == 0) {
			gTestResults.m_passedTests.insert(result.GetTestName());
		}
	}
}

unsigned int TestResults::GetTotalRun() const
{
	return m_passedTests.size() + m_failedTests.size();
}

unsigned int TestResults::GetNumPassed() const
{
	return m_passedTests.size();
}

unsigned int TestResults::GetNumFailed() const
{
	return m_failedTests.size();
}

std::string TestResults::GetResultString() const
{
	return m_resultStr + "\n\n" + toString(GetTotalRun()) + " tests run. " + toString(GetNumFailed()) + " failed, " + toString(GetNumPassed()) + " passed.";
}


class TestRunner
{
public:
	static TestRunner& get()
	{
		static TestRunner* instance = NULL;
		if (instance == NULL)
		{
			instance = new TestRunner;
		}
		return *instance;
	}

	void addTestCase(TestCase *testCase)
	{
		m_testCases.push_back(testCase);
	}

	void runAllTests()
	{
		gTestResults.clear();

		for (TestCases::iterator it(m_testCases.begin()); it != m_testCases.end(); it++)
		{
			(*it)->runTest();
		}
	}

private:
	TestRunner() {}

	typedef std::list<TestCase*> TestCases;
	TestCases m_testCases;
};



TestCase::TestCase(const std::string& testName) :
    m_testName(testName)
{
	TestRunner::get().addTestCase(this);
}


namespace ProtonTester
{

void runAllTests()
{
	TestRunner::get().runAllTests();
	LogMsg("%s", gTestResults.GetResultString().c_str());
}

unsigned int GetTotalRun()
{
	return gTestResults.GetTotalRun();
}

unsigned int GetNumPassed()
{
	return gTestResults.GetNumPassed();
}

unsigned int GetNumFailed()
{
	return gTestResults.GetNumFailed();
}

}
