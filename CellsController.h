#pragma once
#include <random>
#include <chrono>
#include <istream>

#include "global-consts.h"

class CellsController {
private:
	class Cell {
	public:
		const long double pr = 0.0;
		const long double pw = 0.0;
		const long double ptot = 0.0;
	public:
		Cell(long double probRead, long double probWrite);

		dataT operator++();
	private:
		static const dataT _minStep = 0;
		static const dataT _maxStep = UINT_MAX;

		dataT _step = _minStep;
	};

public:
	struct DSInteraction {
		enum class ACT { READ, WRITE, STRING };
		const ACT type;
		const dataT id;
		const dataT v;
		DSInteraction(ACT type,
			dataT arg_id = 0,
			dataT arg_value = 0);
	};

	CellsController(
		long double prob1_read, long double prob1_write,
		long double prob2_read, long double prob2_write,
		long double prob_string);

	static DSInteraction getCellInteraction(std::istream& is);
	std::string getCellInteraction();
private:
	std::mt19937_64 _gen;
	Cell _c1, _c2;

	const long double _probToString = 0;
	const long double _totalProb = 0;
};
