
#include <vector>
#include <time.h>

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <unistd.h>

#include "mem_usage.h"
#include "time_routines.hpp"
#include "rdf3x_upload_data.hpp"
#include "query_routines.hpp"
#include <sys/stat.h>
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

long GetFileSize(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}


struct TRdf3xWrapper : TWrapper {
	TRdf3xWrapper(DictionarySegment& dictionary) : Dictionary(dictionary) {
		Name = "rdf3x_dict";
	}
	~TRdf3xWrapper() {
	}
	DictionarySegment& Dictionary;
	bool Set(const string& key) {
		//prohibited
		return false;
	}
	const char Get(const string& key) {
		unsigned id;
		bool keyFound = Dictionary.lookup(key.c_str(), Type::ID::URI, 0, id);
		return keyFound ? 1 : 0;
	}
	bool FindKeyById(const int id) {
		char buffer[1000];
		const char* start = buffer;
		const char* stop = buffer + sizeof(buffer) - 1;
		::Type::ID type;
		unsigned subType;
		bool idFound = Dictionary.lookupById(id, start, stop, type, subType);
		//cout << string(start, stop - start) << "\n";
		return idFound;
	}
};


void TestRdf3x(const vector<string>& storage,
			   const int toUpload,
		       double* uploadTimeSecPtr,
		       double* totalDiskMemoryMbPtr,
		       double* queryTime100KKeysPtr,
		       double* queryTime100KIDsPtr) {

	const string dbFileName = "db.txt";
	{

		{
			Database database;
			database.create(dbFileName.c_str());
			// uploading routines
			{
				TTime startTime = GetTime();
				NUploadData2Database::UploadData2Database(database, storage, toUpload);
				*uploadTimeSecPtr = GetElapsedInSeconds(startTime, GetTime());
			}
			database.close();
		}
		{
			Database database;
			database.open(dbFileName.c_str(), true);
			//query routines
			{
				TRdf3xWrapper wrapper(database.getDictionary());
				*queryTime100KKeysPtr = NQueringTest::GetTime100KKeyQueries(storage, toUpload, &wrapper);
				*queryTime100KIDsPtr = NQueringTest::GetTime100KIDQueries(storage, toUpload, &wrapper);
			}
			database.close();
		}
	}
	*totalDiskMemoryMbPtr = (double)GetFileSize(dbFileName) / (1024.0 * 1024.0);
	remove(dbFileName.c_str());

}

const int CHECK_INTERVAL = 1000000;

void Usage(char** argV) {
    cout << argV[0] << " <path to file with data>\n";
}



#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>


int main(int argC, char** argV) {
    if (argC != 2) {
	Usage(argV);
	return 1;
    }

    vector<string> heater;
    UploadData("../URIs_datasets/DS6", &heater);
    vector<string> storage;
    UploadData(argV[1], &storage);

    std::srand ( unsigned ( 1 ) );


    cout << "mapper\t" << "rdf3x" << "\n";
    int toUpload = CHECK_INTERVAL;
    while (true) {
    	if (toUpload > storage.size()) {
    		toUpload = storage.size();
    	}
    	{
			double memoryConsumedBefore, residentSetBefore;
			process_mem_usage(memoryConsumedBefore, residentSetBefore);
    	}
		double uploadTimeSec;
		double totalDiskMemoryMb;
		double queryTime100KKeys, queryTime100KIDs;
		TestRdf3x(storage,
				  toUpload,
				  &uploadTimeSec,
				  &totalDiskMemoryMb,
				  &queryTime100KKeys,
				  &queryTime100KIDs);


		cout << toUpload << "\t" << uploadTimeSec << "\t" << totalDiskMemoryMb << "\t" << queryTime100KKeys << "\t" << queryTime100KIDs << "\n";
		cout.flush();

		{
			double memoryConsumedAfter, residentSetAfter;
			// yes, many times, to be sure that this stuff refreshed
			process_mem_usage(memoryConsumedAfter, residentSetAfter);
			process_mem_usage(memoryConsumedAfter, residentSetAfter);
			process_mem_usage(memoryConsumedAfter, residentSetAfter);
		}

		if (toUpload == storage.size()) {
			break;
		}
		toUpload += CHECK_INTERVAL;
    }
    return 0;
}
