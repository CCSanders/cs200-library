using namespace std;
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include "CircularDynamicArray.cpp"
#define DUMP(X) cout << "size is : " << X.length() << endl << "capacity is : " << X.capacity() << endl; for (int i=0; i< X.length();i++) cout << X[i] << " ";  cout << endl << endl;
#define SIZECAP(X) cout << "size is : " << X.length() << endl << "capacity is : " << X.capacity() << endl;
#define DUMPSMALL(X) cout << "size is : " << X.length() << endl << "capacity is : " << X.capacity() << endl; if(X.length()>12) for (int i=2; i<12;i++) cout << X[i] << " ";  cout << endl << endl;

struct Input{
	int *data;
	int *options;
	int size;
};

void testInput(Input *in, CircularDynamicArray<int> &C);
int readInData(Input *in, string filename);
void runTest20();

int main(int argc, char *argv[]){
	if(argc > 1 && strcmp(argv[1],"test20")==0){
		runTest20();
	}
	else{
		Input *in = new Input;
		int size = 0;
		if(argc > 1){
			size = readInData(in,argv[1]);
		}
		else{
			size = readInData(in,"data.txt");
		} 
		CircularDynamicArray<int> C;
		testInput(in,C);
	}
}

void runTest20(){
	CircularDynamicArray<int> C(10);
	//Verify correct usage of constructor
	SIZECAP(C)
	for (int i=0; i< C.length();i++) C[i] = i*4;
	//Check for failure from out of bounds
	C[11]=50;
	//Check array
	DUMP(C)
}

int readInData(Input *in, string filename){
	ifstream fp;
	fp.open(filename.c_str());
	string s = "";
	
	//find size
	getline(fp,s);
	istringstream iss (s);
	iss >> in->size;
	
	//Create arrays
	in->data = new int [in->size];
	in->options = new int [in->size];
	
	//Get data array
	getline(fp,s);
	istringstream iss2 (s);
	for(int i = 0; i < in->size; i++) iss2 >> in->data[i];

	//Get options array
	getline(fp,s);
	istringstream iss3 (s);
	for(int i = 0; i < in->size; i++) iss3 >> in->options[i];
	
	//Close and return
	fp.close();
	return in->size;	
}

/*	The data array is an of input data to insert into the array. If it corresponds with a delete
 *		operation then it will not be added.
 *	The operations array selects the operation for every instance of the loop. 
 *
 */
 
void testInput(Input *in, CircularDynamicArray<int> &C){
	bool print = false;
	for(int i = 0; i < in->size; i++){
		if(C.length() > 0){
			switch(in->options[i]){
				case 0:
					C.addFront(in->data[i]);
					if(print) cout << "adding f => " << in->data[i] <<  " " << C[0] << endl;
					if(print) for(int i = 0; i < C.length(); i++) cout << C[i] << " " << endl;
					break;
				case 1:
					C.addEnd(in->data[i]);
					if(print) cout << "adding r => " << in->data[i] << " " << C[C.length()-1] << endl;
					if(print) for(int i = 0; i < C.length(); i++) cout << C[i] << " " << endl;
					break;
				case 2:
					C.delFront();
					break;
				case 3:
					C.delEnd();
					break;
				case 4:
					cout << "QuickSelect result is " << C.QuickSelect(in->data[i]) << endl;
					break;
				case 5:
					cout << "WCSelect result is " << C.WCSelect(in->data[i]) << endl;
					break;
				case 6:
					C.stableSort();
					break;
				case 7:
					C.radixSort(in->data[i]);
					break;
				case 8:
					cout << "linearSearch result is " << C.linearSearch(in->data[i]) << endl;
					break;
				case 9:
					cout << "binSearch result is " << C.binSearch(in->data[i]) << endl;
					break;
				case 10:
					DUMP(C)
					break;
				case 11:
					DUMPSMALL(C)
					break;
			}
		}else{
			if(in->options[i] == 11){
				DUMPSMALL(C)
			}
			else if(in->options[i]%2){
				C.addEnd(in->data[i]);
				if(print) cout << "adding r1 => " << in->data[i] << " " << C[C.length()-1] << endl;
			}else{
				C.addFront(in->data[i]);
				if(print) cout << "adding f1 => " << in->data[i] << " " << C[0] << endl;
			}
		}
	}
	return;
}
