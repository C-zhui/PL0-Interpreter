#include "syntax.h"
#include <iostream>
using namespace std;
using namespace PL0;
void testCharStream() {

	inputCharStream cs(cin);
	while (cin) {
		cout << cs.peek();
		cs.move();
	}
}

void testTokenStream() {
	TokenStream ts(cin);
	while (cin) {
		Token t = ts.peek();
		ts.move();
		cout << int(t.type) << "   " << t.value << endl;
	}
}

int main() try{
	//testTokenStream();
	//testCharStream();
	PL_0 plc(cin);
	plc.run();
	return 0;
}
catch (string e) {
	cout << e << endl;
}
