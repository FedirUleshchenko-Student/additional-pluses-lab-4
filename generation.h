#pragma once
#include <vector>
#include <filesystem>
#include <initializer_list>

#include "global-consts.h"

class cellController {
public:
	dataT operator++() {
		if (_step == _maxStep)
			_step = _minStep;
		else
			_step++;
		return _step;
	}
	//dataT operator++(int) {
	//	dataT val = _step;
	//	++(*this);
	//	return val;
	//}
	inline dataT step() const noexcept { return _step; }
	inline size_t cell() const noexcept { return _cell; }

	cellController(long double probRead, long double probWrite) :
		pr(probRead), pw(probWrite) {};

private:
	const dataT _minStep = 0;
	const dataT _maxStep = UINT_MAX;
	const long double pr = 0.0;
	const long double pw = 0.0;
	dataT _step = _minStep;
	size_t _cell = 0;
};

struct probData {
	const std::vector<cellController> cells;
	const long double probToString;
	probData(cellController theOnlyOneCellData, long double probabilityToString) :
		cells(1, theOnlyOneCellData), probToString(probabilityToString) { };
	probData(std::initializer_list<cellController> cellData, long double probabilityToString) :
		cells(cellData), probToString(probabilityToString) { };
};

// var 14
// do i need &&, or it is just stupid?
probData getClassicProbabilityData() {
	return
	probData(
		{
			cellController(0.25, 0.05),
			cellController(0.05, 0.25)
		},
		0.4
	);
}

std::vector<dataT> getRandomSequence(size_t size, dataT mn, dataT mx);

//void fillTestFile(size_t as, size_t bs, dataT mn, dataT mx, std::ostream& os);

void createSubtaskForWorker(size_t threadCnt, size_t siz, probData& prob,
	std::filesystem::path dir, std::string namePrefix = "");

void createTestBlock(size_t testFilesCnt, std::string namePrefix = "",
	size_t mnsiz = WINT_MIN, size_t mxsiz = USHRT_MAX,
	std::filesystem::path indir = "", probData probabilities = getClassicProbabilityData());

void tryCreateTestBlock(size_t testFilesCnt, bool possibleRewrite = false, std::string namePrefix = "",
	size_t mnsiz = WINT_MIN, size_t mxsiz = USHRT_MAX,
	std::filesystem::path indir = "", probData probabilities = getClassicProbabilityData());