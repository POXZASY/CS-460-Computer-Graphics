#include <iostream>
#include <vector>
#include <tuple>
#include <math.h>

using namespace std;

int clipperx1 = 125;
int clippery1 = 125;
int clipperx2 = 375;
int clippery2 = 375;
vector<tuple<int, int>> polygon;
vector<tuple<int, int>> viewport;
vector<tuple<int, int>> clipper = {make_tuple(clipperx1, clippery1), make_tuple(clipperx1, clippery2), make_tuple(clipperx2, clippery2), make_tuple(clipperx2, clippery1)};
tuple<int, int> rect1 = make_tuple(400, 400);
tuple<int, int> rect2 = make_tuple(450, 300);
//Window-to-Viewport
void windowToViewport() {
  viewport.clear();

	//send values to origin
	for (tuple<int, int> t : polygon) {
		viewport.push_back(make_tuple(get<0>(t) - clipperx1, get<1>(t) - clippery1));
	}

  cout << "After first translation" << endl;
  for(tuple<int, int> t : viewport){
    cout << get<0>(t) << ", " << get<1>(t) << endl;
  }
	//dilate values
	float xfact = abs( ((float)(get<0>(rect2) - get<0>(rect1))) / ((float)clipperx2 - (float)clipperx1)    );
	float yfact = abs(  ((float)(get<1>(rect2) - get<1>(rect1))) / ((float)clippery2 - (float)clippery1)    );
  //cout << ((float)(get<0>(rect2) - get<0>(rect1))) / ((float)clipperx2 - (float)clipperx1)<<endl;
  cout << "xfact: " << xfact << endl;
  cout << "yfact: " << yfact << endl;
	for (int i = 0; i < viewport.size(); i++) {
		viewport[i] = make_tuple(round(get<0>(viewport[i])*xfact), round(get<1>(viewport[i])*yfact));
	}
  cout << "After dilation" << endl;
  for(tuple<int, int> t : viewport){
    cout << get<0>(t) << ", " << get<1>(t) << endl;
  }
	//send values to new rect
	for (int i = 0; i < viewport.size(); i++) {
		viewport[i] = make_tuple(get<0>(viewport[i]) + get<0>(rect1), get<1>(viewport[i]) + get<1>(rect1));
	}
}

int main(){
  polygon.push_back(make_tuple(129, 127));
  polygon.push_back(make_tuple(371, 130));
  polygon.push_back(make_tuple(371, 371));
  polygon.push_back(make_tuple(131, 366));
  for(tuple<int, int> t : polygon){
    cout << get<0>(t) << ", " << get<1>(t) << endl;
  }
  windowToViewport();
  cout << "afterwards" << endl;
  for(tuple<int, int> t : viewport){
    cout << get<0>(t) << ", " << get<1>(t) << endl;
  }
  return 0;
}
