#pragma once
#include <string>
#include <cstring>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>

#define GCC_VERSION (__GNUC__ * 10000 \
	    + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#if GCC_VERSION >= 40300
#include <tr1/unordered_map>
#define common_unordered_map std::tr1::unordered_map
#else
#include <ext/hash_map>
#define common_unordered_map __gnu_cxx::hash_map
#endif

namespace NLexicoTree {
using namespace std;
typedef unsigned long int KEY_ID;

class Leaf {
public:
	KEY_ID id; //key_id (combined type & seq)
//	class Node *node; //pointer to the parent Node ???
	//KEY_ID id** // multiplr id for one element if there is more than one type of certain an dthen type is in each ID
//	TYPE_ID	**tid; //terminated by NULL
//	size_t len;
	Leaf(KEY_ID _id);
	~Leaf();
};

class Node {
public:
//	static KEY_ID leafIDautoincrement;
//	map< KEY_ID, class Node * >elements;
	char* uri; //terminated by NULL
	size_t len; //TODO cumulated size of string
	class Node **next; //many children, terminated by NULL
	class Node *prev; //only one parent
	class Leaf *leaf; //NULL in no leaf, otherwise leaf

	class Node * Add(char *part1, const char *part2, common_unordered_map< KEY_ID, class Node * >* inv);
//	TYPE_ID GetType(KEY_ID id);
	KEY_ID Get(const char* _uri);
	string GetUri();
	void FindAdd(const char *part2, bool *done, class Node ** newNode, common_unordered_map< KEY_ID, class Node * >* inv);
	//void MakeInverseIndexTypes(vector< class Types >* types);
	//void MakeInverseIndex(vector<class Types>* types, multimap<Leaf*, TYPE_ID>* data);
	void MakeInverseIndex(common_unordered_map< KEY_ID, class Node * >* elements);
	void Display();
	Node();
	~Node();
};



class KeyManager {
	common_unordered_map< KEY_ID, class Node * > elements;
	class Node **uri_id; //head
//	vector< class Types > types; /*
//									0 - unclassified URI
//									1 - type
//								*/

//	class IdUri **id_uri;
public:
	Leaf* Add(const char * _uri);
	void Display();
	//void MakeInverseIndexTypes();
	void MakeInverseIndex();
	//void MakeInverseIndex(multimap<Leaf*,ID>* data);
	string Get(KEY_ID id);
	KEY_ID Get(const char* _uri);
	//TYPE_ID GetType(KEY_ID id);
	//TYPE_ID GetType(const char* _uri);
	KEY_ID GetSeq(KEY_ID id);
	static KEY_ID GetId(KEY_ID seq);
	KEY_ID GetSeq(const char* _uri);
	//map< KEY_ID, class Node * > * GetElementsOfType( TYPE_ID _type, vector<KEY_ID>* elems );
	KeyManager();
	~KeyManager();
};

};

