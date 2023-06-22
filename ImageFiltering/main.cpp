/* Image Filtering */

#pragma warning(disable : 4996)

// Includes

#include <vector>
#include <string>

#include "common/cl.hpp"

#include "common/util.hpp"


#include "common/quickcg.h"

// Defines

#ifndef DEVICE
#define DEVICE CL_DEVICE_TYPE_DEFAULT
#endif

// Namespaces

using namespace std;
using namespace QuickCG;
using namespace cl;
using namespace util;

// Structures

struct Color {
	int r;
	int g;
	int b;
};

// Methods Definition

void load_image(string image_path, string image2_path);
void convolute();
void draw();

void convert_ColorRGB_to_Color(vector<ColorRGB> originalImage, vector<Color> image);

void convert_Color_to_ColorRGB(vector<Color> originalImage);

// Filters

vector<float> blur_filter{
   0.0, 0.2,  0.0,
   0.2, 0.2,  0.2,
   0.0, 0.2,  0.0
};

vector<float>sharpen_filter{
  -1, -1, -1,
  -1,  9, -1,
  -1, -1, -1
};

vector<float>emboss_filter{
  -1, -1,  0,
  -1,  0,  1,
   0,  1,  1
};
double factor = 1.0;
double bias = 0.0;

// Global Variables

vector<Color> zzzzzzzzzzzzzz, yyyyyyyy;
unsigned long imageWidth;
unsigned long imageHeight;

vector<ColorRGB> processedImage;

int currentFilter = 0;
string currentFilterName = "";

int main(int argc, char* argv[]) {


	while (!done())
	{
		load_image("C:\\Users\\Jonathan  M.B\\Desktop\\ImageFiltering\\pics\\photo1.png",
			"C:\\Users\\Jonathan  M.B\\Desktop\\ImageFiltering\\pics\\photo2.png");


		screen(imageWidth, imageHeight, 0, "Filters (Convolution)");

		convolute();

		draw();

		sleep();
	}

	return 0;
}

// Methods Implementation


void load_image(string image_path, string image2_path) {
	vector<ColorRGB> loaded_image;
	vector<ColorRGB> loaded_image2;

	loadImage(loaded_image, imageWidth, imageHeight, image_path);
	loadImage(loaded_image2, imageWidth, imageHeight, image2_path);

	vector<Color> auxColor(loaded_image.size());
	/*vector<Color> auxColor2(loaded_image2.size());*/

	zzzzzzzzzzzzzz = auxColor;
	//yyyyyyyy = auxColor2;

	vector<ColorRGB> auxColorRGB(loaded_image.size());
	processedImage = auxColorRGB;

	convert_ColorRGB_to_Color(loaded_image,zzzzzzzzzzzzzz);
	//convert_ColorRGB_to_Color(loaded_image2, yyyyyyyy);
}

void convolute() {
	Buffer m_image;
	Buffer m_filter;
	Buffer m_result;

	vector<Color> result(zzzzzzzzzzzzzz.size());

	Context context(DEVICE);

	Program program(context, loadProgram("convolution.cl"), true);

	CommandQueue queue(context);

	auto kernel = make_kernel<Buffer, Buffer, Buffer, int, int, float, float>(program, "convolute");

	Timer timer;

	switch (currentFilter)
	{
	case 0: m_filter = Buffer(context, begin(blur_filter), end(blur_filter), true);
		currentFilter = 1;
		currentFilterName = "Blur";
		break;
	case 1:  m_filter = Buffer(context, begin(emboss_filter), end(emboss_filter), true);
		currentFilter = 2;
		currentFilterName = "Emboss";
		break;
	case 2:  m_filter = Buffer(context, begin(sharpen_filter), end(sharpen_filter), true);
		currentFilter = 0;
		currentFilterName = "Sharpen";
		break;
	}
	m_image = Buffer(context, begin(zzzzzzzzzzzzzz), end(zzzzzzzzzzzzzz), true);

	m_result = Buffer(context, CL_MEM_WRITE_ONLY, sizeof(Color) * (imageWidth * imageHeight));

	kernel(EnqueueArgs(queue, NDRange(imageWidth, imageHeight)),
		m_image,
		m_result,
		m_filter,
		imageWidth,
		imageHeight,
		bias,
		factor);

	copy(queue, m_result, begin(result), end(result));

	cout << "Timer" << (timer.getTimeMilliseconds()) / 1.0 << endl;

	convert_Color_to_ColorRGB(result);
}

void draw() {

	for (int y = 0; y < imageHeight; y++)
		for (int x = 0; x < imageWidth; x++)
			pset(x, y, processedImage[y * imageWidth + x]);

	print("Press any key to change Filter", 1, h - 40, RGB_White, 1);
	print("Current Filter: " + currentFilterName, 1, h - 30, RGB_White, 1);

	redraw();
}

void convert_ColorRGB_to_Color(vector<ColorRGB> originalImage, vector<Color>* image) {
	vector<Color>::iterator result_i = image.begin();

	for (vector<ColorRGB>::iterator i = originalImage.begin(); i != originalImage.end(); i++) {
		(*result_i).r = (*i).r;
		(*result_i).g = (*i).g;
		(*result_i).b = (*i).b;
		result_i++;
	}
}


void convert_Color_to_ColorRGB(vector<Color> originalImage) {
	vector<ColorRGB>::iterator result_i = processedImage.begin();

	for (vector<Color>::iterator i = originalImage.begin(); i != originalImage.end(); i++) {
		(*result_i).r = (*i).r;
		(*result_i).g = (*i).g;
		(*result_i).b = (*i).b;
		result_i++;
	}
}