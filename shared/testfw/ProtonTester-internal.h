#ifndef PROTONTESTER_INTERNAL_H
#define PROTONTESTER_INTERNAL_H

#include "util/MiscUtils.h"

struct TestLocation {
	std::string testName;
	std::string fileName;
	int lineNumber;

	TestLocation(const std::string& testName, const std::string& fileName, int lineNumber) :
	    testName(testName),
	    fileName(fileName),
	    lineNumber(lineNumber)
	{
	}
};

class TestResult
{
public:
	enum Result {
		PASS,
		FAIL
	};

	template<typename T>
	TestResult(const TestLocation& testLocation, Result result, const std::string& expectedStr, const std::string& actualStr, const T& actual) :
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

	Result GetResult() const
	{
		return m_result;
	}

	std::string GetResultString() const
	{
		return m_resultStr;
	}

private:
	Result m_result;
	std::string m_resultStr;
};


class TestResults
{
public:
	TestResults();

	void clear();

	static void add(const TestResult &result);

	unsigned int GetTotalRun() const;

	unsigned int GetNumPassed() const;

	unsigned int GetNumFailed() const;

	std::string GetResultString() const;

private:
	unsigned int m_numPassed;
	unsigned int m_numFailed;

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
void CmpEq(const T& expected, const std::string& expectedStr, const T& actual, const std::string& actualStr, const TestLocation& testLocation)
{
	TestResult::Result result = TestResult::FAIL;

	if (expected == actual)
	{
		result = TestResult::PASS;
	}

	TestResults::add(TestResult(testLocation, result, expectedStr, actualStr, actual));
}

#endif // PROTONTESTER_INTERNAL_H
