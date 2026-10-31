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

int main() {
	try {
		string nameType[3] = { "opt", "eql", "bad" };

		// Pseudo code - START
		{
			bool rewrite = true;
			CellsController(* const data[3])() = {
				 getClassicProbabilityData,
				 getEqualProbabilityData,
				 getWorstProbabilityData,
			};

			for (int i = 0; i < 3; i++) {
				Generator::getInstance().tryCreateTestBlock(15, rewrite, "small", 1e2, 1e2, nameType[i], data[i]());
				Generator::getInstance().tryCreateTestBlock(15, rewrite, "medium", 3e4, 3e4, nameType[i], data[i]());
				Generator::getInstance().tryCreateTestBlock(5, rewrite, "big", 1e6, 1e6, nameType[i], data[i]());
			}
		}
		// Pseudo code - END

		auto testData = Tester::measureAllProbDataTests(
			format("input\\{}", nameType[0]),
			format("input\\{}", nameType[1]),
			format("input\\{}", nameType[2])
		);

		for (auto& [key, val] : testData) {
			Tester::outputBlockMeasurements(cout, key, val);
			cout << "\n\n" << endl;
		}
		Tester::outputTotalMeasurements(cout, testData);

	}
	catch (exception e) {
		cout << e.what() << endl;
	}
	return 0;
}