#include "BigInteger.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
	BigInteger n1("1231423424252525242");
	BigInteger n2("4902478209845290482");
	BigInteger n3;
	n3 = n1 + n2;
	cout << n3.getNumber() << endl;
}