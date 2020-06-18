using namespace std;
#include <iostream>
#include "CircularDynamicArray.cpp"
#define DUMP(X) cout << "size is : " << X.length() << endl << "capacity is : " << X.capacity() << endl; for (int i=0; i< X.length();i++) cout << X[i] << " ";  cout << endl << endl;

int main() {
	CircularDynamicArray<int> C(10);
	for (int i = 0; i < C.length(); i++) C[i] = i;
	C[11] = 5;
	DUMP(C)
		C.delFront();
	DUMP(C)
		C.delEnd();
	DUMP(C)
		C.addEnd(100);
	DUMP(C)
		C.addEnd(101);
	DUMP(C)
		C.delEnd(); C.delEnd();
	C.addFront(-100); C.addFront(-200);
	DUMP(C)
		C.addFront(37);
	DUMP(C)

		for (int i = 0; i < 20; i++) C.addEnd(i*i);
	for (int i = 0; i < 20; i++) C.addFront(-(i*i));
	DUMP(C)

		for (int i = 0; i < 25; i++) C.delEnd();
	for (int i = 0; i < 10; i++) C.delFront();
	DUMP(C)

		CircularDynamicArray<int> D;
	D = C;

	C[0] = -201;
	DUMP(C)
		// C is : -201 -64 -49 -36 -25 -16 -9 -4 -1 0 37 -200 -100 1 2 3	
		DUMP(D)
		// D is : -81 -64 -49 -36 -25 -16 -9 -4 -1 0 37 -200 -100 1 2 3

		cout << "The index of 37 in C is : " << C.linearSearch(37) << endl;  // 10
	cout << "The index of 37 in D is : " << D.linearSearch(37) << endl;  // 10	

	cout << "The 4th smallest item in C is : " << C.WCSelect(4) << endl;  //-64
	cout << "The 5th smallest item in C is : " << C.QuickSelect(5) << endl; //-49
	DUMP(C)
		cout << "The smallest item in D is : " << D.QuickSelect(1) << endl << endl;  // -200

	D.stableSort();
	DUMP(D)
		// D is : -200 -100 -81 -64 -49 -36 -25 -16 -9 -4 -1 0 1 2 3 37

		cout << "The index of -4 in D is : " << D.binSearch(-4) << endl << endl;  // 9

	C[0] = 31743;
	C[1] = 31744;
	C[2] = 30720;
	C[3] = 30719;
	C.radixSort(10);
	DUMP(C)
		// C is : 31744 30720 0 1 2 3 37 -200 -100 -25 -16 -9 -4 31743  30719 -1
		DUMP(D)
}