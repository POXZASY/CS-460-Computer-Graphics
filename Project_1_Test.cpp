#include <iostream>
#include <vector>
#include <stdio.h>
#include <tuple>

using namespace std;

vector<tuple<int,int>> lines;
void midpoint(int x_1, int y_1, int x_2, int y_2) {
	cout <<"1: "<< x_1 << ", " << y_1 << ", " << x_2 << ", " << y_2 << endl;
	//---------------------------------------------
	//adjust variables to first octant
	bool reflecty = false;
	bool flipyx = false;
	//if the second point is behind the first, flip them
	if (x_2 < x_1) {
		int temp = x_1;
		x_1 = x_2;
		x_2 = temp;
		temp = y_1;
		y_1 = y_2;
		y_2 = temp;
	}
  cout << "2: " << x_1 << ", " << y_1 << ", " << x_2 << ", " << y_2 << endl;
	//bring the points to the origin to prepare for surgery
	int tempx = x_1;
	int tempy = y_1;
	x_1 = x_1 - tempx;
	y_1 = y_1 - tempy;
	x_2 = x_2 - tempx;
	y_2 = y_2 - tempy;
	cout << "3: " << x_1 << ", " << y_1 << ", " << x_2 << ", " << y_2 << endl;
	//if the second point is below the first
	if (y_2 - y_1 < 0) {
		y_2 = -1 * y_2;
		reflecty = true;
	}
	cout << "4: " << x_1 << ", " << y_1 << ", " << x_2 << ", " << y_2 << endl;
	int dx = x_2 - x_1;
  int dy= y_2 - y_1;
	float m = (float)dy / (float)dx;
	//if the line is above y=x
	if (m > 1) {
		int temp = x_2;
		x_2 = y_2;
		y_2 = temp;
		flipyx = true;
	}
	cout << "5: " << x_1 << ", " << y_1 << ", " << x_2 << ", " << y_2 << endl;

	//initialize variables
	dx = x_2 - x_1;
	dy = y_2 - y_1;
	int D = 2 * dy - dx;
	int deltaE = 2 * dy;
	int deltaNE = 2 * (dy - dx);
	int x = x_1;
	int y = y_1;

	//---------------------------------------------
	//ALGORITHM
	vector<tuple<int, int>> points;
	points.push_back(make_tuple(x_1, y_1));
	while (x < x_2) {
		if (D <= 0) {
			D = D + deltaE;
			x++;
		}
		else {
			D = D + deltaNE;
			x++;
			y++;
		}
		points.push_back(make_tuple(x, y));
	}
	//---------------------------------------------
	//Convert Back
	for (tuple<int, int> t : points) {
		tuple<int, int> temp = t;
		if (flipyx) {
			temp = make_tuple(get<1>(temp), get<0>(temp));
		}
		if (reflecty) {
			temp = make_tuple(get<0>(temp), -1*get<1>(temp));
		}
		temp = make_tuple(get<0>(temp)+tempx, get<1>(temp)+tempy);
		lines.push_back(temp);
	}
}
int main(){
	midpoint(20,5,10,10);
	for(tuple<int,int> t : lines){
		cout <<"(" << get<0>(t) << ", " << get<1>(t) <<") ";
	}
	return 0;
}
