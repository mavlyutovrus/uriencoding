#pragma once

#define GCC_VERSION (__GNUC__ * 10000 \
	    + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#if GCC_VERSION >= 40300
#include <tr1/unordered_map>
#define hash_map std::tr1::unordered_map
#else
#include <ext/hash_map>
#define hash_map __gnu_cxx::hash_map
#endif


#include <string>
#include <vector>
#include <map>
#include <boost/unordered_map.hpp>
#include <sparsehash/dense_hash_map>
#include <sparsehash/sparse_hash_map>
#include "lexico_tree/KeyManager.h"
#include "bplus_tree.hpp"
#include "wrapper_proto.h"




using std::vector;
using std::string;
using std::cout;
using std::ifstream;
using std::pair;


struct TEmtpyWrapper : TWrapper {
	TEmtpyWrapper() {
		Name = "empty_wrapper";
	}
	~TEmtpyWrapper() {
	    cout << "before deleting\n";
	}
	bool Set(const string& key) {
		return true;
	}
	const char Get(const string& key) {
		return 1;
	}
};


struct TBoostUnorderedMapWrapper : TWrapper {
	TBoostUnorderedMapWrapper() {
		Name = "boost::unordered_map";
	}
	~TBoostUnorderedMapWrapper() {
	    cout << "before deleting\n";
	}
	boost::unordered_map<string, char> Dictionary;
	bool Set(const string& key) {
		bool inserted = Dictionary.insert(pair<string, char>(key, 1)).second;
		return inserted;
	}
	const char Get(const string& key) {
		boost::unordered_map<string, char>::const_iterator it = Dictionary.find(key);
		if (it == Dictionary.end()) {
			return 0;
		}
		return it->second;
	}
};



struct TLexicoTreeWrapper : TWrapper {
	TLexicoTreeWrapper() {
		Name = "Lexicographic tree";
	}
	~TLexicoTreeWrapper() {
	    cout << "before deleting\n";
	}
	NLexicoTree::KeyManager Dictionary;
	bool Set(const string& key) {
		NLexicoTree::Leaf* leaf = Dictionary.Add(key.c_str());
		return leaf != NULL;
	}
	const char Get(const string& key) {
		NLexicoTree::KEY_ID keyId = Dictionary.Get(key.c_str());
		return 1;
	}
};


struct TBPlusTreeWrapper : TWrapper {
	TBPlusTreeWrapper() {
		Name = "B+ tree";
	}
	~TBPlusTreeWrapper() {
	    cout << "before deleting\n";
	}
	NBPlusTree::BPlusTree<string, char, 10, 10> Dictionary;
	bool Set(const string& key) {
		Dictionary.insert(key, 1);
		return 1;
	}
	const char Get(const string& key) {
		char value;
		bool found = Dictionary.find(key, &value);
		return found ? value : 0;
	}
};




struct THashMapWrapper : TWrapper {
	THashMapWrapper() {
		Name = "std::unordered_map";
	}
	~THashMapWrapper() {
	    cout << "before deleting\n";
	}
	hash_map<string, char> Dictionary;
	bool Set(const string& key) {
		bool inserted = Dictionary.insert(pair<string, char>(key, 1)).second;
		return inserted;
	}
	const char Get(const string& key) {
		hash_map<string, char>::const_iterator it = Dictionary.find(key);
		if (it == Dictionary.end()) {
			return 0;
		}
		return it->second;
	}
};

struct TStdMapWrapper : TWrapper {
	TStdMapWrapper() {
		Name = "std::map";
	}
	~TStdMapWrapper() {
	    cout << "before deleting\n";
	}
	std::map<string, char> Dictionary;
	bool Set(const string& key) {
		bool inserted = Dictionary.insert(pair<string, char>(key, 1)).second;
		return inserted;
	}
	const char Get(const string& key) {
		std::map<string, char>::const_iterator it = Dictionary.find(key);
		if (it == Dictionary.end()) {
			return 0;
		}
		return it->second;
	}
};


struct TGoogleDenseMapWrapper : TWrapper {
	TGoogleDenseMapWrapper() {
		Name = "google dense_hash_map";
		string empty_key = "a hyle";
		Dictionary.set_empty_key(empty_key);
	}
	~TGoogleDenseMapWrapper() {
	    cout << "before deleting\n";
	}
	google::dense_hash_map<string, char> Dictionary;
	bool Set(const string& key) {
		bool inserted = Dictionary.insert(pair<string, char>(key, 1)).second;
		return inserted;
	}
	const char Get(const string& key) {
		google::dense_hash_map<string, char>::iterator it = Dictionary.find(key);
		if (it == Dictionary.end()) {
			return 0;
		}
		return it->second;
	}
};

struct TGoogleSparseMapWrapper : TWrapper {
	TGoogleSparseMapWrapper() {
		Name = "google sparse_hash_map";
		string empty_key = "a hyle";
		//Dictionary.set_empty_key(empty_key);
	}
	~TGoogleSparseMapWrapper() {
	    cout << "before deleting\n";
	}
	google::sparse_hash_map<string, char> Dictionary;
	bool Set(const string& key) {
		bool inserted = Dictionary.insert(pair<string, char>(key, 1)).second;
		return inserted;
	}
	const char Get(const string& key) {
		google::sparse_hash_map<string, char>::iterator it = Dictionary.find(key);
		if (it == Dictionary.end()) {
			return 0;
		}
		return it->second;
	}
};
