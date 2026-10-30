#pragma once
#include <mutex>
#include <shared_mutex>
#include <string>

#include "global-consts.h"

class DS
{
public:
	DS();
	dataT get(size_t position) const;
	void set(size_t position, dataT data);

	explicit operator std::string() const;

	inline void clear() { for (int i = 0; i < MDAT; i++) a[i] = 0; }
private:
	dataT a[MDAT];
	mutable std::shared_mutex _cell0;
	mutable std::mutex _cell1;
private:
	inline void _checkPos(size_t &position) const;
	dataT _get0() const;
	dataT _get1() const;
	void _set0(dataT data);
	void _set1(dataT data);
};

