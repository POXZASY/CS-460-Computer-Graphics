#include <iostream>
#include <vector>
#include <tuple>
#include <math.h>

using namespace std;


int screenx = 10;
//function to check if vector contains a point
bool contains(vector<tuple<int, int>> vec, tuple<int,int> pt) {
	for (tuple<int, int> t : vec) {
		if (t == pt) return true;
	}
	return false;
}

//Scanline Fill Algorithm
vector<tuple<int, int>> scanLineFill(vector<tuple<int, int>> polygonPoints) {
	vector<tuple<int, int>> retVal;
	int maxy = INT_MIN;
	int miny = INT_MAX;
	//find all points intersected by polygon
	vector<tuple<int, int>> intersectPoints;
	for (int i = 0; i < polygonPoints.size(); i++) { //iterate through each of the sides of the polygon
		tuple<int, int> p1 = polygonPoints[i]; //first point
		tuple<int, int> p2 = polygonPoints[(i + 1) % polygonPoints.size()]; //second point
		int p1x = get<0>(p1);
		int p1y = get<1>(p1);
		int p2x = get<0>(p2);
		int p2y = get<1>(p2);
		//store max/min y for later
		if (p1y > maxy) maxy = p1y;
		if (p2y > maxy) maxy = p2y;
		if (p1y < miny) miny = p1y;
		if (p2y < miny) miny = p2y;
		//slope of the line
    //if slope is infinite
    if(p2x - p1x == 0){
      for (int j = min(p1y, p2y); j <= max(p1y, p2y); j++) {
        intersectPoints.push_back(make_tuple(p1x, j));
      }
    }
    //normal slope
    else{
      float m = (float)(p2y - p1y) / (p2x - p1x);
  		//iterate through all of the x values between the two points
  		for (int j = min(p1x, p2x); j <= max(p1x, p2x); j++) {
  			intersectPoints.push_back(make_tuple(j, round(m*(j-p1x)+p1y)));
  		}
    }
	}
  cout << "---------------------------------------" << endl;
  cout << "Boundary Points: " << endl;
  for(tuple<int, int> t : intersectPoints){
    cout << get<0>(t) << ", " << get<1>(t) << endl;
  }
  cout << "---------------------------------------" << endl;
	//apply algorithm
	for (int y = miny; y <= maxy; y++) { //iterate through the y values
		int parity = 0;
		bool lastpointnotboundary = true;
		for (int x = 0; x <= screenx; x++) {
			if (contains(intersectPoints, make_tuple(x, y)) && lastpointnotboundary) { //if we found a boundary point, and the previous point wasn't a boundary point
				parity = (parity+1)%2;
				lastpointnotboundary = false;
			}
			if (parity == 1 && !contains(intersectPoints, make_tuple(x, y))) { //if we are filling and the point is not a boundary
				retVal.push_back(make_tuple(x, y));
				lastpointnotboundary = true;
			}
			else if (parity == 0 && !contains(intersectPoints, make_tuple(x, y))) {
				lastpointnotboundary = true;
			}
		}
	}
	cout << "Total Points: " << retVal.size() << endl;
	return retVal;
}

int main(){
  vector<tuple<int, int>> polygon;
  polygon.push_back(make_tuple(0, 0));
  polygon.push_back(make_tuple(5, 0));
  polygon.push_back(make_tuple(5, 5));
  polygon.push_back(make_tuple(0, 5));
  vector<tuple<int, int>> filler = scanLineFill(polygon);
  for(tuple<int, int> t : filler){
    cout << get<0>(t) << ", " << get<1>(t) << endl;
  }
  return 0;
}
