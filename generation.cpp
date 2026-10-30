#include "generation.h"
#include <fstream>
#include <random>
#include <utility>
#include <functional>

using namespace std;
using namespace std::filesystem;

// var 14
CellsController getClassicProbabilityData() {
	return CellsController(0.25, 0.05, 0.05, 0.25, 0.4);
}

CellsController getEqualProbabilityData() {
	return CellsController(0.20, 0.20, 0.20, 0.20, 0.20);
}

// TODO
CellsController getWorstProbabilityData() {
	return CellsController(0.01, 0.03, 0.01, 0.45, 0.50);
}

Generator::Generator() : _gen(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())) {}

Generator& Generator::getInstance() {
	static Generator gen;
	return gen;
}

string Generator::create1WorkerSubtask(size_t siz,
	path dir, CellsController& manager) {

	string main_file = dir.string() + "\\1.txt";
	ofstream fos1(main_file);

	for (int p = 1; p <= siz; p++)
		fos1 << manager.getCellInteraction() << endl;

	return main_file;
}

void Generator::copyWorkerSubtask(
	size_t threadCnt, string copyFile, path dir) {
	if (!is_directory(dir)) create_directories(dir);

	ifstream fis(copyFile);
	vector<ofstream> fos; fos.resize(threadCnt);

	for (int j = 1; j <= threadCnt; j++)
		fos[j - 1].open(format("{}\\{}.txt", dir.string(), j));

	string t;
	while (fis.good()) {
		for (int j = 1; j <= threadCnt; j++)
			if (getline(fis, t)) fos[j - 1] << t << endl;
	}
}

void Generator::createTestBlock(
	size_t testFilesCnt, string namePrefix,
	size_t mnsiz, size_t mxsiz,
	path indir, CellsController manager) {

	path dir = path("input") / indir;
	if (!is_directory(dir))
		create_directories(dir);
	
	uniform_int_distribution<dataT> rand(mnsiz, mxsiz);

	for (int i = 1; i <= testFilesCnt; i++) {

		//auto temp_dir = dir / path(format("{}{} - 1", namePrefix, i));
		auto temp_dir = dir / path(format("{}{}", namePrefix, i)) / path(format("{} threads", 1));
		if (!is_directory(temp_dir)) create_directories(temp_dir);

		string file = create1WorkerSubtask(rand(_gen), temp_dir, manager);

		for (int th = 2; th <= MTEST; th++) {
			auto curr_dir = dir / path(format("{}{}", namePrefix, i)) / path(format("{} threads", th));

			copyWorkerSubtask(th, file, curr_dir);
		}
	}
}

void Generator::tryCreateTestBlock(
	size_t testFilesCnt, bool isRewritePossible, string namePrefix,
	size_t mnsiz, size_t mxsiz,
	path indir, CellsController manager) {

	path dir = path("input") / indir;

	if (!exists(dir))
		createTestBlock(testFilesCnt, namePrefix, mnsiz, mxsiz, indir, manager);
	else if (!is_directory(dir))
		throw logic_error("there is already a file with name of test's directory (not a dir)");
	else if(isRewritePossible)
		createTestBlock(testFilesCnt, namePrefix, mnsiz, mxsiz, indir, manager);
}