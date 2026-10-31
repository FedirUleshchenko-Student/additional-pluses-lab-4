#include "CellsController.h"

using namespace std;

CellsController::Cell::Cell(long double probRead, long double probWrite) :
	pr(probRead), pw(probWrite), ptot(pr + pw) {
};

dataT CellsController::Cell::operator++() {
	if (_step == _maxStep)
		_step = _minStep;
	else
		_step++;
	return _step;
}

CellsController::CellsController(
	long double prob0_read, long double prob0_write,
	long double prob1_read, long double prob1_write,
	long double prob_string) :
	_c0(prob0_read, prob0_write), _c1(prob1_read, prob1_write),
	_probToString(prob_string), _totalProb(_c0.ptot + _c1.ptot + _probToString),
	_gen(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())) {}

CellsController::DSInteraction::DSInteraction(ACT type, dataT arg_id, dataT arg_value) :
	type(type), id(arg_id), v(arg_value) {}

#include <iostream>

CellsController::DSInteraction CellsController::getCellInteraction(std::istream& is) {
	string type; is >> type;
	if (type == "read") {
		dataT id; is >> id;
		return DSInteraction(DSInteraction::ACT::READ, id);
	}
	else if (type == "write") {
		dataT id; is >> id;
		dataT val; is >> val;
		return DSInteraction(DSInteraction::ACT::WRITE, id, val);
	}
	else if (type == "string") {
		return DSInteraction(DSInteraction::ACT::STRING);
	}
	else if(type != "")
		throw invalid_argument("cannot read type of ds interation");
}

string CellsController::getCellInteraction() {
	long double t = (std::uniform_real_distribution<long double>(0.0, _totalProb))(_gen); // threshold
	if (t < _c0.ptot) {
		if (t < _c0.pr) return "read 0";
		return ("write 0 " + to_string(++_c0));
	}
	
	t -= _c1.ptot;

	if (t < _c1.ptot) {
		if (t < _c1.pr) return "read 1";
		return ("write 1 " + to_string(++_c1));
	}
	return "string";
}