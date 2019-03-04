#include <iostream>
#include <vector>
#include <tuple>
#include <math.h>

using namespace std;

int clipperx1 = 10;
int clippery1 = 10;
int clipperx2 = 20;
int clippery2 = 20;
vector<tuple<int, int>> polygon;
vector<tuple<int, int>> viewport;
vector<tuple<int, int>> clipper = {make_tuple(clipperx1, clippery1), make_tuple(clipperx1, clippery2), make_tuple(clipperx2, clippery2), make_tuple(clipperx2, clippery1)};
tuple<int, int> rect1 = make_tuple(30, 10);
tuple<int, int> rect2 = make_tuple(50, 30);
//Window-to-Viewport
void windowToViewport() {
	viewport.clear();

	//send values to origin
	for (tuple<int, int> t : polygon) {
		viewport.push_back(make_tuple(get<0>(t) - clipperx1, get<1>(t) - clippery1));
	}
	//dilate values
	int xfact = round(abs( ((float)(get<0>(rect2) - get<0>(rect1))) / ((float)clipperx2 - (float)clipperx1)    ));
	int yfact = round(abs(  ((float)(get<1>(rect2) - get<1>(rect1))) / ((float)clippery2 - (float)clippery1)    ));
	for (tuple<int, int> t : viewport) {
		t = make_tuple(get<0>(t)*xfact, get<1>(t)*yfact);
	}
	//send values to new rect
	for (tuple<int, int> t : viewport) {
		t = make_tuple(get<0>(t) + get<0>(rect1), get<1>(t) + get<1>(rect1));
	}
}

int main(){

  polygon.push_back(make_tuple(12, 12));
  polygon.push_back(make_tuple(12, 14));
  polygon.push_back(make_tuple(14, 14));
  polygon.push_back(make_tuple(14, 12));
  for(tuple<int, int> t : polygon){
    cout << get<0>(t) << ", " << get<1>(t) << endl;
  }
  windowToViewport();
  for(tuple<int, int> t : viewport){
    cout << get<0>(t) << ", " << get<1>(t) << endl;
  }
  return 0;
}
