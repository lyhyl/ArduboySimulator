#ifndef PROTONTESTER_INTERNAL_H
#define PROTONTESTER_INTERNAL_H

#include "util/MiscUtils.h"
#include <set>

struct CheckLocation {
	std::string testName;
	std::string fileName;
	int lineNumber;

	CheckLocation(const std::string& testName, const std::string& fileName, int lineNumber) :
	    testName(testName),
	    fileName(fileName),
	    lineNumber(lineNumber)
	{
	}
};

class CheckResult
{
public:
	enum Result {
		PASS,
		FAIL
	};

	template<typename T>
	CheckResult(const CheckLocation& testLocation, Result result, const std::string& expectedStr, const std::string& actualStr, const T& actual) :
	    m_testName(testLocation.testName),
	    m_result(result)
	{
		m_resultStr = testLocation.testName + " (" + testLocation.fileName + ":" + toString(testLocation.lineNumber) + "):\n";

		switch (result)
		{
		case PASS:
			m_resultStr += "  " + actualStr + "\n  is\n  " + expectedStr;
			break;

		case FAIL:
			m_resultStr += "  Expected:    " + actualStr + "\n";
			m_resultStr += "  to be:       " + expectedStr + "\n";
			m_resultStr += "  but it was:  " + toString(actual);
			break;
		}
	}

	std::string GetTestName() const
	{
		return m_testName;
	}

	Result GetResult() const
	{
		return m_result;
	}

	std::string GetResultString() const
	{
		return m_resultStr;
	}

private:
	std::string m_testName;
	Result m_result;
	std::string m_resultStr;
};


class TestResults
{
public:
	TestResults();

	void clear();

	static void add(const CheckResult &result);

	unsigned int GetTotalRun() const;

	unsigned int GetNumPassed() const;

	unsigned int GetNumFailed() const;

	std::string GetResultString() const;

private:
	std::set<std::string> m_passedTests;
	std::set<std::string> m_failedTests;

	std::string m_resultStr;

};

class TestCase
{
public:
	TestCase(const std::string& testName);

	virtual ~TestCase() {}
	virtual void runTest() = 0;

protected:
	std::string m_testName;
};


template<typename T>
void CmpEq(const T& expected, const std::string& expectedStr, const T& actual, const std::string& actualStr, const CheckLocation& testLocation)
{
	CheckResult::Result result = CheckResult::FAIL;

	if (expected == actual)
	{
		result = CheckResult::PASS;
	}

	TestResults::add(CheckResult(testLocation, result, expectedStr, actualStr, actual));
}

#endif // PROTONTESTER_INTERNAL_H
