#include "syntax.h"
#include <iostream>
using namespace std;
using namespace PL0;

int main(int argc, char const *argv[]) try
{
	if(argc==2){
		if(freopen(argv[1],"r",stdin)==NULL)
		{
			cout << "file is invalid\n";
			return 0;
		}
	}
	else {
		cout << "read script from stdin\n";
	}
	PL_0 plc(cin);
	plc.run();
	return 0;
}
catch (string e)
{
	cout << e << endl;
}
