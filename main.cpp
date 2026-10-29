// Working in Visual Studio 2022, so compiler:
// MSVC (Microsoft Visual C++)
//
// Standard (for version of C++):
// Preview - ISO C++23 Standard (/std:c++23preview)

#include <iostream>
#include <chrono>
#include <format>
#include <filesystem>

using namespace std;
using namespace std::filesystem;

// needed ??
//int main(int argc, char* argv[])
int main() {
	try {
		if (exists(path("input")) && is_directory(path("input")))
			cout << "COC" << endl;
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
	return 0;
}