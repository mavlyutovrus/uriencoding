//============================================================================
// Name        : keymanager.cpp
// Author      : rashmi
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;
#include "KeyManager.h"
#include"commonclass.h"
#include "helper_papi_tracer.h"
#include <vector>
#include <stdlib.h>

int main() {
	commonclass sp;
	PapiTracer p;
	KeyManager KM;
//	KM.Add("http://qwert1");
//	KM.Add("http://qwert12");
//	KM.Add("http://qwert123");
//	KM.Add("http://qwert1235");
//	KM.Add("http://qwert12346");
//	KM.Add("http://qwert1587");
//	KM.Add("abcd");
//	KM.Display();



//	int key = KM.Get("http://qwert1587");
//	string value = KM.Get(key);

//	cout<< "------------> " << key<< "\t" << value << endl;

//*********************************************************************

	ifstream myfile("/home/rashmi/datasets/DS2_DBPEDIA200");
	vector<string> lines;
	vector<string>::iterator i;
	set<int>::iterator i2;
	set<int>RandomSet;

	string line;
	int cnt = 0;
	long int g=0;
	int c=0;

	if (myfile.is_open()) {
		while (myfile.good()) {
			getline(myfile, line);

			lines.push_back(line);

		}
	}
	int y = sp.vmdata(); // compute vmdata after adding to vector
		double tm = sp.getcputime();

		int events = p.start();
	    timeval tim;
		gettimeofday(&tim, NULL); //get current time.
		double t1 = (double) tim.tv_sec + (double) tim.tv_usec / 1000000.0;

		for (i = lines.begin(); i != lines.end(); i++) {
g++;
			string str = *i;
			KM.Add(str.c_str());

//		if (g <= 60000000 and g % 5000000 == 0) {
//					cout << g << endl;
//					gettimeofday(&tim, NULL);
//
//					double t3 = sp.getcputime();
//					double tm2 = t3 - tm; // cpu time before and after reading data
//					cout << "cpu time :" << tm2 << endl;
					//cout << "number of collisions are:" << cnt << endl;

//					int z = sp.vmdata(); //compute vmdata after retrieving data from a vector
//					int a = z - y;
//					cout << "Total memory consumed (Vmdata): " << a << " Kb" << endl;
//					c++;
//				}
		}
		//RETRIEVAL OF URIS
			// creating random 6 million uris
			for (int r = 0; r <= 6000000; r++) {
				int ran = rand() % lines.size();
				RandomSet.insert(ran);
			}
			// starting timer
			double tr = sp.getcputime();

		for (i2 = RandomSet.begin(); i2 != RandomSet.end(); i2++)
         {
			string str2=lines.at(*i2);
        	 int key = KM.Get(str2.c_str());
							string value = KM.Get(key);
							//cout<<"key  " << key<<"  string "<<value<<endl;


		}

			double tr4= sp.getcputime();
			double tr5 = tr4 - tr; //
			cout<<"cpu time for retrieval of 6 million uris "<<tr5<<endl;

		// rest below is commented for incremental purpose. otherwise its imp
//		gettimeofday(&tim, NULL);
//			double t2 = tim.tv_sec + (tim.tv_usec / 1000000.0);
//
//			double t3 = sp.getcputime();
//			double tm2 = t3 - tm; // cpu time before and after reading data
//
//			PapiTracer::result_t result = p.stop(events);
//			double tt = t2 - t1;
///
//			sp.report(tt, tm2);
//			int z = sp.vmdata(); //compute vmdata after retrieving data from a vector
//			int a = z - y;
//			cout << "Total memory consumed (Vmdata): " << a << " Kb" << endl;
//			cout<<"vector size: "<<lines.size()<<endl;
//			std::cout << "CPU Cycles " << result.first << std::endl;
//			std::cout << "Total Instructions " << result.second << std::endl;
		cout << "nu of lines " << g << endl;
		cout << "no of steps" << c << endl;
	return 0;
}
