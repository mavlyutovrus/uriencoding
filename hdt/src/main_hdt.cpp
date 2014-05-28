
#include <vector>
#include <time.h>

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <unistd.h>
#include <functional>
#include "mem_usage.h"
#include "time_routines.hpp"
#include "query_routines.hpp"
#include <sys/stat.h>
#include "hdt_upload_data.hpp"
using std::vector;
using std::string;
using std::cout;
using std::ifstream;
using std::pair;


string GetHostName() {
	char HOST_NAME[100];
	gethostname(HOST_NAME, sizeof(HOST_NAME));
	return (string)HOST_NAME;
}

int GetMaxUpload() {
	const int MAX_UPLOAD = GetHostName() == "butch" ? 1000000 : 100000000;
	return MAX_UPLOAD;
}

void UploadData(const char* filename, vector<string>* storagePtr) {
    ifstream file(filename);
    string line;
    if (!file.is_open()) {
    	return;
    }
    const int MAX_UPLOAD = GetMaxUpload();
    while (getline(file, line)) {
		storagePtr->push_back(line);
		if (storagePtr->size() >= MAX_UPLOAD) {
			break;
		}
    }
}


struct THDTWrapper : TWrapper {
	THDTWrapper(NHDTMimic::THDTImitator& hdt) : HDT(hdt) {
		Name = "hdt";
	}
	~THDTWrapper() {
	}
	NHDTMimic::THDTImitator& HDT;
	bool Set(const string& key) {
		//prohibited
		return false;
	}
	const char Get(const string& key) {
		unsigned id;
		int keyFound = HDT.GetId(key);
		return keyFound ? 1 : 0;
	}
	bool FindKeyById(const int id) {
		return HDT.GetKeyById(id) != NULL;
	}
};



const int CHECK_INTERVAL = 1000000;

void Usage(char** argV) {
    cout << argV[0] << " <path to file with data>\n";
}



int main(int argC, char** argV) {
    if (argC != 2) {
		Usage(argV);
		return 1;
    }

    vector<string> occupier;
    UploadData("../URIs_datasets/DS6", &occupier);
    vector<string> storage;
    UploadData(argV[1], &storage);

    std::srand ( unsigned ( 1 ) );
	cout << "mapper\t" << "hdt" << "\n";

    int toUpload = CHECK_INTERVAL;
    while (true) {
    	if (toUpload > storage.size()) {
    		toUpload = storage.size();
    	}
		double memoryConsumedBefore, residentSetBefore;
		{
			process_mem_usage(memoryConsumedBefore, residentSetBefore);
			process_mem_usage(memoryConsumedBefore, residentSetBefore);
		}

	    NHDTMimic::THDTImitator hdtBase;

	    double uploadTimeSec;
	    {
	    	TTime startTime = GetTime();
	    	hdtBase.UploadData(storage, toUpload);
	    	uploadTimeSec = GetElapsedInSeconds(startTime, GetTime());
	    }

	    double memoryConsumedAfter, residentSetAfter;
	    {
			process_mem_usage(memoryConsumedAfter, residentSetAfter);
			process_mem_usage(memoryConsumedAfter, residentSetAfter);
	    }

	    THDTWrapper wrapper(hdtBase);
	    double queryTime100KKeys = NQueringTest::GetTime100KKeyQueries(storage, toUpload, &wrapper);
	    double queryTime100KIDs = NQueringTest::GetTime100KIDQueries(storage, toUpload, &wrapper);

		cout << toUpload << "\t" << uploadTimeSec << "\t" << (memoryConsumedAfter - memoryConsumedBefore) / 1024.0 <<
				            "\t" << queryTime100KKeys << "\t" << queryTime100KIDs << "\n";
		cout.flush();
		if (toUpload == storage.size()) {
			break;
		}
		toUpload += CHECK_INTERVAL;
    }
    return 0;
}
