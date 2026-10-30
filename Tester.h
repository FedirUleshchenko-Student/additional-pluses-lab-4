#pragma once
#include <filesystem>
#include <thread>
#include <iostream>
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

	static void threadFunc(std::ifstream& is, DS& ds);
public:
	//static Tester& getInstance();

	struct BlockMeasurement {
		size_t testsCnt = 1;
		long double t1 = 0;
		long double t2 = 0;
		long double t3 = 0;

		BlockMeasurement& operator+=(const BlockMeasurement& o);
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

	static BlockMeasurement measureSingleBlock(std::filesystem::path testBlock);
	static BlockMeasurement measureAllTests(std::filesystem::path dir="input");

	static Measurements measureAllProbDataTests(
		std::filesystem::path opt, std::filesystem::path eql, std::filesystem::path bad);
};