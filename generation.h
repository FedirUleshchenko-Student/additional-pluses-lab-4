#pragma once
#include <filesystem>
#include <random>

#include "global-consts.h"
#include "CellsController.h"

CellsController getClassicProbabilityData();
CellsController getEqualProbabilityData();
CellsController getWorstProbabilityData();

class Generator {
private:
	std::mt19937_64 _gen;

	Generator();
	Generator(Generator& other) = delete;
	void operator=(const Generator&) = delete;
public:
	static Generator& getInstance();

	std::string create1WorkerSubtask(size_t siz,
		std::filesystem::path dir, CellsController& manager);

	void copyWorkerSubtask(
		size_t threadCnt, std::string copyFile, std::filesystem::path dir);

	void createTestBlock(size_t testFilesCnt, std::string namePrefix = "",
		size_t mnsiz = WINT_MIN, size_t mxsiz = USHRT_MAX,
		std::filesystem::path indir = "", CellsController manager = getClassicProbabilityData());

	void tryCreateTestBlock(size_t testFilesCnt, bool isRewritePossible = true,
		std::string namePrefix = "", size_t mnsiz = WINT_MIN, size_t mxsiz = USHRT_MAX,
		std::filesystem::path indir = "", CellsController manager = getClassicProbabilityData());
};
