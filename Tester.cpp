#include "Tester.h"
#include <format>
#include <iostream>

#include "CellsController.h"

using namespace std;
using namespace std::filesystem;

Tester::BlockMeasurement& Tester::BlockMeasurement::operator+=(const BlockMeasurement& o) {
	*this = (*this + o);
	return *this;
}

Tester::Measurements::operator string() const {
	return format("{0}{1}{2}{3}{4}{0}",
		string(89, '-') + '\n',
		format("{0:>15} | {1:>15} ({4}) | {2:>15} ({4}) | {3:>15} ({4}) |\n",
			"thread cnt", "1 thread(s) avg.", "2 thread(s) avg.", "3 thread(s) avg.", RATIO_NAME),
		format("{0:>15} | {1:>21.7f} | {2:>21.7f} | {3:>21.7f} |\n",
			"optimal", opt.t1, opt.t2, opt.t3),
		format("{0:>15} | {1:>21.7f} | {2:>21.7f} | {3:>21.7f} |\n",
			"equal", eql.t1, eql.t2, eql.t3),
		format("{0:>15} | {1:>21.7f} | {2:>21.7f} | {3:>21.7f} |\n",
			"bad", bad.t1, bad.t2, bad.t3)
		);
}

void Tester::threadFunc(std::ifstream& is, DS& ds, bool& isError) {
	while (is.good()) {
		try {
			auto x = CellsController::getCellInteraction(is);
			if (x.type == CellsController::DSInteraction::ACT::READ) ds.get(x.id);
			if (x.type == CellsController::DSInteraction::ACT::WRITE) ds.set(x.id, x.v);
			if (x.type == CellsController::DSInteraction::ACT::WRITE) string(ds);
		}
		catch (...) {
			isError = true;
			return;
		}
	}
}

Tester::BlockMeasurement Tester::measureSingleTest(path testBlock) {
	DS ds = DS();
	BlockMeasurement ans;
	ans.testsCnt = 1;

	{
		bool isError = false;
		ifstream o(format("{}\\1 threads\\1.txt", testBlock.string()));
		auto t1 = chrono::steady_clock::now();
		{
			jthread th(threadFunc, ref(o), ref(ds), ref(isError));
		}
		auto t2 = chrono::steady_clock::now();

		if (isError) throw exception(
				format("Something went wrong in {}\\1 threads", testBlock.string()).c_str());
		ans.t1 = chrono::duration<timeT, RATIO>(t2 - t1).count();
	}

	ds.clear();
	
	{
		bool isError = false;
		ifstream o1(format("{}\\2 threads\\1.txt", testBlock.string()));
		ifstream o2(format("{}\\2 threads\\2.txt", testBlock.string()));
		auto t1 = chrono::steady_clock::now();
		{
			jthread th1(threadFunc, ref(o1), ref(ds), ref(isError));
			jthread th2(threadFunc, ref(o2), ref(ds), ref(isError));
		}
		auto t2 = chrono::steady_clock::now();

		if (isError) throw exception(
				format("Something went wrong in {}\\2 threads", testBlock.string()).c_str());
		ans.t2 = chrono::duration<timeT, RATIO>(t2 - t1).count();
	}

	ds.clear();

	{
		bool isError = false;
		ifstream o1(format("{}\\3 threads\\1.txt", testBlock.string()));
		ifstream o2(format("{}\\3 threads\\2.txt", testBlock.string()));
		ifstream o3(format("{}\\3 threads\\3.txt", testBlock.string()));
		auto t1 = chrono::steady_clock::now();
		{
			jthread th1(threadFunc, ref(o1), ref(ds), ref(isError));
			jthread th2(threadFunc, ref(o2), ref(ds), ref(isError));
			jthread th3(threadFunc, ref(o3), ref(ds), ref(isError));
		}
		auto t2 = chrono::steady_clock::now();

		if (isError) throw exception(
			format("Something went wrong in {}\\3 threads", testBlock.string()).c_str());
		ans.t3 = chrono::duration<timeT, RATIO>(t2 - t1).count();
	}

	return ans;
}

Tester::BlockMeasurement Tester::measureAllTestsInBlock(path dir) {
	if (!is_directory(dir))
		throw invalid_argument(format("there is no such directory as {}", dir.string()));

	BlockMeasurement ans;
	for (const auto& x : directory_iterator(dir)) {
		ans += measureSingleTest(x.path());
	}
	return ans;
}

map<string, Tester::BlockMeasurement> Tester::measureAllBlocks(path dir) {
	if (!is_directory(dir))
		throw invalid_argument(format("there is no such directory as {}", dir.string()));

	map<string, BlockMeasurement> ans;
	for (const auto& x : directory_iterator(dir)) {
		ans.emplace(x.path().filename().string(), measureAllTestsInBlock(x.path()));
	}
	return ans;
}

map<string, Tester::Measurements> Tester::measureAllProbDataTests(
	std::filesystem::path optdir, std::filesystem::path eqldir, std::filesystem::path baddir) {

	auto optm = measureAllBlocks(optdir);
	auto eqlm = measureAllBlocks(eqldir);
	auto badm = measureAllBlocks(baddir);

	map<string, Measurements> ans;

	for (auto [key, optval] : optm) {
		ans.emplace(key, Measurements
		(
			optval,
			eqlm[key],
			badm[key]
		));
	}
	
	return ans;
}

void Tester::outputTotalMeasurements(
	std::ostream& os, const map<string, Measurements>& m) {
	BlockMeasurement optm;
	BlockMeasurement eqlm;
	BlockMeasurement badm;

	for (auto [_, val] : m) {
		optm += val.opt;
		eqlm += val.eql;
		badm += val.bad;
	}

	outputBlockMeasurements(os, "TOTAL MEASUREMENTS (NOT WEIGHTED)", Measurements(optm, eqlm, badm));
}

void Tester::outputBlockMeasurements(ostream& os, string name, Measurements m) {
	os << format("{0}{1}{0}{2}{0}",
		string(89, '=') + '\n',
		format("{0:-^89}", " " + name + " ") + '\n',
		string(m)
	);
}