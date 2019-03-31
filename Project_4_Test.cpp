#include <fstream>
#include <vector>

using namespace std;

const int width = 384;
const int height = 256;

struct Color {
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

vector<vector<Color>> structimage(width, vector<Color>(height));
unsigned char image[width * height * 3];

void readBMP(const char * filename) {
	FILE* img = fopen(filename, "rb");

	//get rid of the .bmp header
	unsigned char header[54];
	fread(header, sizeof(unsigned char), 54, img);
	//get width, height
	//width = (int)(header[18]);
	//height = (int)(header[22]);

	fread(image, sizeof(unsigned char), width*height*3, img);
	fclose(img);

}
void bitsToStructs() {
	int inc = 0;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Color color;
			color.b = image[inc];
			inc++;
			color.g = image[inc];
			inc++;
			color.r = image[inc];
			inc++;
			structimage[i][j]=color;
		}
	}
}

int main(){
  readBMP("flower.bmp");
  bitsToStructs();
  return 0;
}
