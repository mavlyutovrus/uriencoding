#pragma once

#include <string>

struct TWrapper {
	virtual bool Set(const std::string& key) = 0;
	virtual const char Get(const std::string& key) = 0;
	virtual bool FindKeyById(const int id) {
		return false;
	}
	virtual ~TWrapper() {
	}
	std::string Name;
};

