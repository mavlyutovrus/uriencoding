#include <vector>
#include <time.h> 
#include <chrono>
#include <iostream>
#include <fstream> 
#include <string>
#include <algorithm>
#include <unistd.h>
#include <stdio.h>


const int CHECK_INTERVAL = 100000;
//!!!! have to be not less and able to divide evenly on CHECK_INTERVAL
const int QUERIES_TIME_CHECK_INTERVAL = 100000;
const int CHECK_QUERIES = 100000;

#include "mem_usage.h"
#include "wrappers.hpp"
#include "time_routines.hpp"

using std::vector;
using std::string;
using std::cout;
using std::ifstream;
using std::pair;

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

void FillDictionary(const vector<string>& storage, TWrapper* mapperPtr) {
    double dataInserted = 0.0;
    double memoryConsumedBefore, residentSetBefore;
    process_mem_usage(memoryConsumedBefore, residentSetBefore);
    process_mem_usage(memoryConsumedBefore, residentSetBefore);
    process_mem_usage(memoryConsumedBefore, residentSetBefore);
    cout << "before mem: " << memoryConsumedBefore / 1024.0 << "Mb\n";
    cout << "mapper\t" << mapperPtr->Name << "\n";
    int keyIndex = 0;
    while (true) {
    	if (keyIndex + CHECK_INTERVAL > storage.size()) {
    		break;
    	}
    	double time100KInserts;
    	double memoryConsumedAfter, residentSetAfter;
    	{
        	vector<string> keysBuffer;
        	GenerateQueryKeys(storage, keyIndex, keyIndex + CHECK_INTERVAL, CHECK_INTERVAL, &keysBuffer);
        	TTime start_time = GetTime();
    		for (vector<string>::const_iterator bufferIt = keysBuffer.begin(); bufferIt != keysBuffer.end(); ++bufferIt) {
    			const string& key = *bufferIt;
    			bool inserted = mapperPtr->Set(key);
    			dataInserted += key.length() + 1;
    			if (!inserted) {
    				std::cerr << "INSERT FUCKUP\n";
    				exit(1);
    			}
    		}
    		time100KInserts = GetElapsedInSeconds(start_time, GetTime());
			process_mem_usage(memoryConsumedAfter, residentSetAfter);
			process_mem_usage(memoryConsumedAfter, residentSetAfter);
    	}
    	keyIndex += CHECK_INTERVAL;
    	double time100KQueries = -1;
    	if (keyIndex % QUERIES_TIME_CHECK_INTERVAL == 0) {
        	vector<string> keysBuffer;
        	GenerateQueryKeys(storage, 0, keyIndex, CHECK_QUERIES, &keysBuffer);
        	TTime start_time = GetTime();
    		for (vector<string>::const_iterator bufferIt = keysBuffer.begin(); bufferIt != keysBuffer.end(); ++bufferIt) {
    			const string& key = *bufferIt;
    			char result = mapperPtr->Get(key);
    			if (result != 1) {
    				std::cerr << "QUERY FUCKUP\n";
    				exit(1);
    			}
    		}
    		time100KQueries = GetElapsedInSeconds(start_time, GetTime());
    	}
    	{

			cout << "\t" << keyIndex << "\t" << dataInserted / (1024.0 * 1024.0) << "\t"
						 << (memoryConsumedAfter - memoryConsumedBefore) / 1024.0 << "\t"
						 << (residentSetAfter - residentSetBefore) / 1024.0 << "\t"
						 << time100KInserts << "\t" << time100KQueries << "\n";
			cout.flush();
    	}
    }
}

string GetHostName() {
	char HOST_NAME[100];
	gethostname(HOST_NAME, sizeof(HOST_NAME));
	return (string)HOST_NAME;
}

void UploadData(const char* filename, vector<string>* storagePtr) {
    const int MAX_UPLOAD = 100000000;
    ifstream file(filename);
    if (!file.is_open()) {
    	return;
    }
    string line;
    while (getline(file, line)) {
		storagePtr->push_back(line);
		if (storagePtr->size() >= MAX_UPLOAD) {
			break;
		}
    }
}

void Usage(char** argV) {
    cout << argV[0] << " <path to file with data> <mapper index>\n";
}

int main(int argC, char** argV) {
    if (argC != 3) {
	Usage(argV);
	return 1;
    }
    const int algo_index = atoi(argV[2]);

    { // heater: for smooth results
	vector<string> occupier;
	UploadData(argV[1], &occupier);
    }

    vector<string> storage;
    UploadData(argV[1], &storage);

    vector<TWrapper*> mappers;
    mappers.push_back(new TEmtpyWrapper());
    mappers.push_back(new THashMapWrapper());
    mappers.push_back(new TStdMapWrapper());
    mappers.push_back(new TGoogleDenseMapWrapper());
    mappers.push_back(new TGoogleSparseMapWrapper());
    mappers.push_back(new TBoostUnorderedMapWrapper());
    mappers.push_back(new TLexicoTreeWrapper());
    mappers.push_back(new TBPlusTreeWrapper());
    std::srand (1);
    {
	FillDictionary(storage, mappers[algo_index]);
	delete mappers[algo_index];
    }
    return 0;
}
