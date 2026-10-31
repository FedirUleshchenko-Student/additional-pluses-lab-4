#include "DS.h"
#include <format>
#include <stdexcept>

using namespace std;

DS::DS() {
	clear();
}

dataT DS::get(size_t position) const {
	_checkPos(position);
	if (position == 0) return _get0();
	else return _get1();
}

void DS::set(size_t position, dataT data) {
	_checkPos(position);
	if (position == 0) _set0(data);
	else _set1(data);
}

dataT DS::_get0() const {
	shared_lock lk(_mutex[0]);
	return a[0];
}

dataT DS::_get1() const {
	shared_lock lk(_mutex[1]);
	return a[1];
}

void DS::_set0(dataT data) {
	lock_guard lk(_mutex[0]);
	a[0] = data;
}

void DS::_set1(dataT data) {
	lock_guard lk(_mutex[1]);
	a[1] = data;
}

void DS::_checkPos(size_t& position) const {
	if (position >= MDAT)
		throw invalid_argument(format("position is not in range [{},{}]", 0, MDAT - 1));
}

DS::operator std::string() const {
	shared_lock lk0(_mutex[0], defer_lock);
	shared_lock lk1(_mutex[1], defer_lock);
	lock(lk0, lk1);
	string ans="[";
	for (size_t i = 0; i < MDAT; i++)
		ans += a[i] + ((i + 1 < MDAT) ? "," : "");
	ans += "]";
	return ans;
}