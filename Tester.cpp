#include "Tester.h"
#include <format>
#include <iostream>

#include "CellsController.h"

using namespace std;
using namespace std::filesystem;

//Tester& Tester::getInstance() {
//	static Tester tester;
//	return tester;
//}

Tester::BlockMeasurement& Tester::BlockMeasurement::operator+=(const BlockMeasurement& o) {
	t1 *= testsCnt;
	t2 *= testsCnt;
	t3 *= testsCnt;

	t1 += o.t1;
	t2 += o.t2;
	t3 += o.t3;

	testsCnt++;

	t1 /= testsCnt;
	t2 /= testsCnt;
	t3 /= testsCnt;

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

void Tester::threadFunc(std::ifstream& is, DS& ds) {
	while (is.good()) {
		try {
			auto x = CellsController::getCellInteraction(is);
			if (x.type == CellsController::DSInteraction::ACT::READ) ds.get(x.id - 1);
			if (x.type == CellsController::DSInteraction::ACT::WRITE) ds.set(x.id - 1, x.v);
			if (x.type == CellsController::DSInteraction::ACT::WRITE) string(ds);
		}
		catch (...) {
			cout << "STUPID NICK ON " << this_thread::get_id() << endl;
		}
	}
}

Tester::BlockMeasurement Tester::measureSingleBlock(path testBlock) {
	DS ds = DS();
	BlockMeasurement ans;

	{
		ifstream o(format("{}\\1 threads\\1.txt", testBlock.string()));
		auto t1 = chrono::steady_clock::now();
		{
			jthread th(threadFunc, ref(o), ref(ds));
		}
		auto t2 = chrono::steady_clock::now();
		ans.t1 = chrono::duration<timeT, RATIO>(t2 - t1).count();
	}

	ds.clear();
	
	{
		ifstream o1(format("{}\\2 threads\\1.txt", testBlock.string()));
		ifstream o2(format("{}\\2 threads\\2.txt", testBlock.string()));
		auto t1 = chrono::steady_clock::now();
		{
			jthread th1(threadFunc, ref(o1), ref(ds));
			jthread th2(threadFunc, ref(o2), ref(ds));
		}
		auto t2 = chrono::steady_clock::now();
		ans.t2 = chrono::duration<timeT, RATIO>(t2 - t1).count();
	}

	ds.clear();

	{
		ifstream o1(format("{}\\3 threads\\1.txt", testBlock.string()));
		ifstream o2(format("{}\\3 threads\\2.txt", testBlock.string()));
		ifstream o3(format("{}\\3 threads\\3.txt", testBlock.string()));
		auto t1 = chrono::steady_clock::now();
		{
			jthread th1(threadFunc, ref(o1), ref(ds));
			jthread th2(threadFunc, ref(o2), ref(ds));
			jthread th3(threadFunc, ref(o3), ref(ds));
		}
		auto t2 = chrono::steady_clock::now();
		ans.t3 = chrono::duration<timeT, RATIO>(t2 - t1).count();
	}

	return ans;
}

Tester::BlockMeasurement Tester::measureAllTests(path dir) {
	if (!is_directory(dir))
		throw invalid_argument(format("there is no such directory as {}", dir.string()));

	BlockMeasurement ans;
	for (const auto& x : directory_iterator(dir)) {
		ans += measureSingleBlock(x.path());
	}
	return ans;
}

Tester::Measurements Tester::measureAllProbDataTests(
	std::filesystem::path opt, std::filesystem::path eql, std::filesystem::path bad) {
	return Measurements(
		measureSingleBlock(opt),
		measureSingleBlock(eql),
		measureSingleBlock(bad)
	);
}