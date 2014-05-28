
//rdf3x
#include "rts/database/Database.hpp"
#include "infra/util/Type.hpp"
#include "rts/segment/DictionarySegment.hpp"
#include "rts/database/DatabasePartition.hpp"
#include "rts/database/DatabaseBuilder.hpp"
#include "rts/buffer/BufferManager.hpp"
#include "rts/partition/FilePartition.hpp"
#include "tools/rdf3xload/TempFile.hpp"
#include "assert.h"
#include <stdio.h>
#include <string.h>



namespace NUploadData2Database {

using std::vector;
using std::string;
using std::cout;
using std::ifstream;
using std::pair;

const int BUFFER_SIZE = 4000;

class TVectorReader : public DictionarySegment::StringSource {
private:
	const vector<string>& Storage;
	char KeyBuffer[BUFFER_SIZE];
	const int ToUpload;
	int Position;
public:
	vector<unsigned> Remembered;
	virtual ~TVectorReader() {
	}

	TVectorReader (const vector<string>& storage, const int toUpload) :
			Storage(storage),
			ToUpload(toUpload),
			Position (0) {
	}
	bool next(unsigned& len,const char*& data,Type::ID& type,unsigned& subType) {
		if (Position == Storage.size() || Position == ToUpload) {
			return false;
		}
		{
			//don't waste time on memory allocation, use buffer
			if (Storage[Position].length() >= BUFFER_SIZE) {
				len = 0;
			} else {
				strcpy(KeyBuffer, Storage[Position].c_str());
				KeyBuffer[Storage[Position].length()] = 'k';
				KeyBuffer[Storage[Position].length() + 1] = '\0';
				len = Storage[Position].length() + 1;
			}
			data = KeyBuffer;
			type = Type::ID::URI;
			subType = 0;
		}

		++Position;
		return true;
	}
	void rememberInfo(unsigned page,unsigned ofs,unsigned hash) {
		Remembered.push_back(page);
		Remembered.push_back(ofs);
		Remembered.push_back(hash);
	}

};


class THashSource : public DictionarySegment :: HashSource {
public:
	/// Destructor
	virtual ~THashSource() {

	}
	int Position;
	vector< pair<unsigned, unsigned> > HashValues;


	THashSource(const vector<unsigned>& sequenceOfRemeberedData) :
							Position(0) {
		int sourcePosition = 0;
		while (sourcePosition < sequenceOfRemeberedData.size()) {
			unsigned page_value = sequenceOfRemeberedData[sourcePosition];
			unsigned ofs_value = sequenceOfRemeberedData[sourcePosition + 1];
			unsigned hash_value = sequenceOfRemeberedData[sourcePosition + 2];
			HashValues.push_back(std::pair<unsigned, unsigned>(hash_value, page_value));
			sourcePosition += 3;
		}
		std::sort(HashValues.begin(), HashValues.end());
	}
	///should give it sorted by hash value
	virtual bool next(unsigned& hash,unsigned& page) {
		if (Position == HashValues.size()) {
			return false;
		}
		hash = HashValues[Position].first;
		page = HashValues[Position].second;;
		++Position;
		return true;
	}
};


class TIdSource : public DictionarySegment :: IdSource {
public:
	/// Destructor
	virtual ~TIdSource() {
	}
	const vector<unsigned>& Remembered;
	int Position;

	TIdSource(const vector<unsigned>& sequenceOfRemeberedData) :
							Remembered(sequenceOfRemeberedData),
							Position(0) {
	}
	/// Get the next entry
	virtual bool next(unsigned& page,unsigned& ofsLen) {
		if (Position == Remembered.size()) {
			return false;
		}
		unsigned page_value = Remembered[Position];
		unsigned ofs_value = Remembered[Position + 1];
		unsigned hash_value = Remembered[Position + 2];
		ofsLen = ofs_value;
		page = page_value;
		Position += 3;
		return true;
	}
};

void UploadData2Database(Database& database, const vector<string>& storage, const int toUpload) {
	TVectorReader storageReader(storage, toUpload);
	//strings
	{
		DictionarySegment* segment = new DictionarySegment(database.getFirstPartition());
		database.getFirstPartition().addSegment(segment, DatabasePartition::Tag_Dictionary);
		segment->loadStrings(storageReader);
	}
	TIdSource idSource(storageReader.Remembered);
	{
		//strings mappings
		DictionarySegment* segmentPtr = database.getFirstPartition().lookupSegment<DictionarySegment>(DatabasePartition::Tag_Dictionary);
		segmentPtr->loadStringMappings(idSource);
	}
	THashSource hashSource(storageReader.Remembered);
	// hashes
	{
		DictionarySegment* segmentPtr = database.getFirstPartition().lookupSegment<DictionarySegment>(DatabasePartition::Tag_Dictionary);
		segmentPtr->loadStringHashes(hashSource);
	}

}


};
