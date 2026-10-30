// Working in Visual Studio 2022, so compiler:
// MSVC (Microsoft Visual C++)
//
// Standard (for version of C++):
// Preview - ISO C++23 Standard (/std:c++23preview)

#include <iostream>
#include <chrono>
#include <format>
#include <filesystem>

#include "generation.h"
#include "Tester.h"

using namespace std;
using namespace std::filesystem;

// needed ??
//int main(int argc, char* argv[])
int main() {
	try {
		if (exists(path("input")) && is_directory(path("input")))
			cout << "COC" << endl;

		bool rewrite = true;

		//Generator::getInstance().tryCreateTestBlock(5, rewrite, "medium ", 1e4, 1e4, "opt", getClassicProbabilityData());
		//Generator::getInstance().tryCreateTestBlock(5, rewrite, "big ", 1e6, 1e6, "opt", getClassicProbabilityData());
		Generator::getInstance().tryCreateTestBlock(5, rewrite, "large ", 1e9, 1e9, "opt", getClassicProbabilityData());
		//Generator::getInstance().tryCreateTestBlock(5, rewrite, "medium ", 1e4, 1e4, "eql", getEqualProbabilityData());
		//Generator::getInstance().tryCreateTestBlock(5, rewrite, "big ", 1e6, 1e6, "eql", getEqualProbabilityData());
		Generator::getInstance().tryCreateTestBlock(5, rewrite, "large ", 1e9, 1e9, "eql", getEqualProbabilityData());
		//Generator::getInstance().tryCreateTestBlock(5, rewrite, "medium ", 1e4, 1e4, "bad", getWorstProbabilityData());
		//Generator::getInstance().tryCreateTestBlock(5, rewrite, "big ", 1e6, 1e6, "bad", getWorstProbabilityData());
		Generator::getInstance().tryCreateTestBlock(5, rewrite, "large ", 1e9, 1e9, "bad", getWorstProbabilityData());

		auto x = Tester::measureAllProbDataTests(
			"input\\opt",
			"input\\eql",
			"input\\bad"
		);

		cout << string(x) << endl;

	}
	catch (exception e) {
		cout << e.what() << endl;
	}
	return 0;
}