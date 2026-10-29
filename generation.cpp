#include "generation.h"
#include <fstream>
#include <random>

using namespace std;
using namespace std::filesystem;

mt19937_64 gen(chrono::system_clock::to_time_t(chrono::system_clock::now()));

vector<dataT> getRandomSequence(size_t size, dataT mn, dataT mx) {
	vector<dataT> ans;
	uniform_int_distribution<dataT> rand(mn, mx);

	while (size > 0) {
		size--;
		ans.push_back(rand(gen));
	}
	sort(ans.begin(), ans.end());
	return ans;
}

void fillTestFile(size_t as, size_t bs, dataT mn, dataT mx, ostream& os) {
	os << as << '\n';
	vector<dataT> sq(getRandomSequence(as, mn, mx));
	for (auto&& x : sq) os << x << " ";
	os << '\n';

	os << bs << '\n';
	sq = getRandomSequence(bs, mn, mx);
	for (auto&& x : sq) os << x << " ";
	os << '\n';
	os.flush();
}

void createSubtaskForWorker(size_t threadCnt, size_t siz, probData& prob,
	path dir, string namePrefix = "") {

}

void createTestBlock(size_t testFilesCnt, string namePrefix,
	size_t mnsiz, size_t mxsiz,
	path indir, probData probabilities) {
	path dir = path("input") / indir;

	if (!is_directory(dir))
		create_directories(dir);
	
	uniform_int_distribution<dataT> rand(mnsiz, mxsiz);

	for (int i = 1; i <= testFilesCnt; i++) {
		for (int th = 1; th <= MDAT; th++) {
			//ofstream fo(format("{}\\{}{}.txt", dir.string(), namePrefix, i));

			auto curr_dir = dir / path(format("{} threads", th));

			if (!is_directory(curr_dir))
				create_directories(curr_dir);

			createSubtaskForWorker(th, rand(gen), probabilities, curr_dir, namePrefix);
			//fillTestFile(rand(gen), rand(gen), fo);
		}
	}
}

void tryCreateTestBlock(size_t testFilesCnt, bool possibleRewrite, string namePrefix,
	size_t mnsiz, size_t mxsiz,
	path indir, probData probabilities) {
	path dir = path("input") / indir;

	if (!exists(dir))
		createTestBlock(testFilesCnt, namePrefix, mnsiz, mxsiz, indir, probabilities);
	else if (!is_directory(dir))
		throw logic_error("there is already a file with name of test's directory (not a dir)");
	else if (possibleRewrite)
		createTestBlock(testFilesCnt, namePrefix, mnsiz, mxsiz, indir, probabilities);
}