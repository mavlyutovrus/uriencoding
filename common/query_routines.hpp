#include "wrapper_proto.h"

#include <vector>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <unistd.h>
#include "assert.h"

namespace NQueringTest {

using std::vector;
using std::string;
using std::cout;
using std::ifstream;
using std::pair;

const int QUERIES_COUNT = 100000;

void GenerateQueryKeys(const vector<string>& storage,
							const int leftBound,
							const int rightBound,
							const int takeAmount,
						vector<string>* out) {
	assert(rightBound - leftBound >= takeAmount);
	int step = (rightBound - leftBound) / takeAmount;
	int keyIndex = leftBound;
	for (int queriesDone = 0; queriesDone < takeAmount; ++queriesDone) {
		//don't allow to reuse storage memory
		const string key = storage[keyIndex] + "k";
		out->push_back(key);
		keyIndex += step;
	}
}

void GenerateQueryIds(const int leftBound,
					  const int rightBound,
					  const int takeAmount,
						vector<int>* out) {
	assert(rightBound - leftBound >= takeAmount);
	int step = (rightBound - leftBound) / takeAmount;
	int keyIndex = leftBound;
	for (int queriesDone = 0; queriesDone < takeAmount; ++queriesDone) {
		out->push_back(keyIndex);
		keyIndex += step;
	}
}



double GetTime100KKeyQueries(const vector<string>& storage, const int inserted, TWrapper* mapperPtr) {
	vector<string> keysBuffer;
	GenerateQueryKeys(storage, 0, inserted, QUERIES_COUNT, &keysBuffer);
	TTime start_time = GetTime();
	for (vector<string>::const_iterator bufferIt = keysBuffer.begin(); bufferIt != keysBuffer.end(); ++bufferIt) {
		const string& key = *bufferIt;
		char result = mapperPtr->Get(key);
		if (result != 1) {
			std::cerr << "QUERY FUCKUP " << key << "\n";
		}
	}
	return GetElapsedInSeconds(start_time, GetTime());
}

double GetTime100KIDQueries(const vector<string>& storage, const int inserted, TWrapper* mapperPtr) {
	vector<int> keysBuffer;
	GenerateQueryIds(0, inserted, QUERIES_COUNT, &keysBuffer);
	TTime start_time = GetTime();
	for (vector<int>::const_iterator bufferIt = keysBuffer.begin(); bufferIt != keysBuffer.end(); ++bufferIt) {
		const int id = *bufferIt;
		bool keyFound = mapperPtr->FindKeyById(id);
		if (!keyFound) {
			std::cerr << "ID QUERY FUCKUP " << id << "\n";
		}
	}
	return GetElapsedInSeconds(start_time, GetTime());
}


}
