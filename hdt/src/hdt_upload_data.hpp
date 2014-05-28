#include <libdcs/CSD.h>
#include <libdcs/CSD_PFC.h>
#include <PlainDictionary.hpp>
#include <Iterator.hpp>
#include <HDTListener.hpp>

namespace NHDTMimic {

using std::vector;
using std::string;
using std::cout;
using std::ifstream;
using std::pair;

using namespace hdt;

class TSilentListener : public hdt::ProgressListener {
public:
	~TSilentListener() { }
	virtual void notifyProgress(float level, const char *section) {

	}
};
bool CmpLexicographic(hdt::DictionaryEntry *c1, hdt::DictionaryEntry *c2) {
    return strcmp(c1->str,c2->str)<0;
}


struct THDTImitator {
	csd::CSD_PFC* FinalDictPtr;

	THDTImitator() : FinalDictPtr(NULL) {
	}

	~THDTImitator() {
		if (FinalDictPtr) {
			delete FinalDictPtr;
		}
	}

	void UploadData(const vector<string>& storage, const int toUpload) {
		if (FinalDictPtr) {
			throw "already filled";
			return;
		}
		int rightBound = std::min((int)storage.size(), toUpload);
		vector<hdt::DictionaryEntry> entriesPull(rightBound);
		vector<hdt::DictionaryEntry*> EntriesVector;
	    hdt::DictEntryHash modifiableDictionary;
	    for (int recordIndex = 0; recordIndex < rightBound; ++recordIndex) {
	    	const string& key = storage[recordIndex];
	    	hdt::DictionaryEntry* entryPtr = &entriesPull[recordIndex];
	    	entryPtr->str = new char[key.length() + 2];
	    	strcpy(entryPtr->str, key.c_str());
	    	entryPtr->str[key.length() + 1] = '\0';
	    	entryPtr->str[key.length()] = 'k';
	    	modifiableDictionary.insert(pair<const char*, hdt::DictionaryEntry*>(entryPtr->str, entryPtr));
	    }
	    for (hdt::DictEntryHash::const_iterator mapIterator = modifiableDictionary.begin();
	    		mapIterator != modifiableDictionary.end(); ++mapIterator) {
	    	EntriesVector.push_back(mapIterator->second);
	    }
	    std::sort(EntriesVector.begin(), EntriesVector.end(), CmpLexicographic);
	    {
			const int BLOCK_SIZE = 16;
			hdt::DictIterator iterator(EntriesVector);
			TSilentListener listener;
			FinalDictPtr = new csd::CSD_PFC(&iterator, BLOCK_SIZE, &listener);
	    }

	    //release memory
	    for (int entryIndex = 0; entryIndex < EntriesVector.size(); ++entryIndex) {
	    	delete[] EntriesVector[entryIndex]->str;
	    }
	}
	unsigned int GetId(const string& key) {
		unsigned int position = FinalDictPtr->locate((const unsigned char *)key.c_str(), key.length());
		return position;
	}
	const unsigned char* GetKeyById(const int id) {
		const char* str = (const char *)FinalDictPtr->extract(id);
		return FinalDictPtr->extract(id);
	}

};



}
