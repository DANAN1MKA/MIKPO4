#include <iostream>
#include <math.h>
using namespace std;
int main()
{
	int x = 20;
	int y = 5;
	int C = 0;
	float temp = 0;
	temp = cos(y);
	temp = pow(temp, 3);
	C = 3 * x;
	C = (float) C + temp;
	temp = x * y;
	temp = 2 * temp;
	temp = sin(temp);
	C = (float) C / temp;
	cout << C << endl;
	return 0;
}

