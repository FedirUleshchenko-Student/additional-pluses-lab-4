#pragma once
#include <filesystem>
#include <thread>
#include <iostream>

#include <map>
//#include <memory>

#include "global-consts.h"
#include "DS.h"

#include <fstream>

static class Tester
{
private:
	//Tester() = default;
	//Tester(Tester& other) = delete;
	//void operator=(const Tester&) = delete;

	static void threadFunc(std::ifstream& is, DS& ds, bool& isError);
public:
	//static Tester& getInstance();

	struct BlockMeasurement {
		size_t testsCnt = 1;
		
		// averages

		long double t1 = 0;
		long double t2 = 0;
		long double t3 = 0;

		BlockMeasurement& operator+=(const BlockMeasurement& o);
		friend BlockMeasurement operator+(BlockMeasurement a, BlockMeasurement b) {
			a.t1 *= a.testsCnt;
			a.t2 *= a.testsCnt;
			a.t3 *= a.testsCnt;

			a.t1 += b.t1 * b.testsCnt;
			a.t2 += b.t2 * b.testsCnt;
			a.t3 += b.t3 * b.testsCnt;

			a.testsCnt += b.testsCnt;

			a.t1 /= a.testsCnt;
			a.t2 /= a.testsCnt;
			a.t3 /= a.testsCnt;
			return a;
		}
	};

	struct Measurements {
		BlockMeasurement opt;
		BlockMeasurement eql;
		BlockMeasurement bad;
		Measurements(BlockMeasurement optimal, BlockMeasurement equal, BlockMeasurement awful) :
			opt(optimal), eql(equal), bad(awful) {}

		operator std::string() const;
	};

	//void threadFunc(std::unique_ptr<std::istream> is, DS *ds);

	static BlockMeasurement measureSingleTest(std::filesystem::path testBlock);
	static BlockMeasurement measureAllTestsInBlock(std::filesystem::path dir);
	static std::map<std::string, Tester::BlockMeasurement>
		measureAllBlocks(std::filesystem::path dir);

	static std::map<std::string, Tester::Measurements> measureAllProbDataTests(
		std::filesystem::path optdir = "input\\opt",
		std::filesystem::path eqldir = "input\\eql",
		std::filesystem::path baddir = "input\\bad");

	static void outputTotalMeasurements(
		std::ostream& os, const std::map<std::string, Measurements>& m);

	static void outputBlockMeasurements(std::ostream& os, std::string name, Measurements m);
};