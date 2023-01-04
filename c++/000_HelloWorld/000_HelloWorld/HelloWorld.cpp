#include <iostream>
#include <sstream>
#include <cmath>

using namespace std;

int main()
{

	int      a = 1;
	int		 b = 3;
	int		 result = a + b;
	int		 carrot;
	double	 x;
	double   y;
	double   z;
	double   money;
	std::string mystring;
	mystring = "This is a string";

	std::cout << result << std::endl;
	std::cout << mystring << std::endl;


	ostringstream oss("into en green tea");
	cout << oss.str() << endl;

	oss << 16.9 << " Ounce ";
	cout << oss.str() << endl;

	oss << "(Pack of " << 12 << ")";
	cout << oss.str() << endl;

	cin >> carrot ;
	cin.get();
	cout <<"carrot:"<< carrot << endl;

	x = sqrt(25);
	y = pow(x,3);
	z = rand()  ;
	
	cout << "x:" << x << endl;
	cout << "y:" << y << endl;
	cout << "z:" << z << endl;
	

	cin.get();

	return 0 ;
}