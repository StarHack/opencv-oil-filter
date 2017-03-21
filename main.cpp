/*
 * OpenCV Oil Filter + mask detection
 *
 * @author: MuetzenJunge
 *
 * Use responsibly and at own risk. Please consider the environment before printing this code.
 *
 */
#include <iostream>
#include <fstream>
#include <sys/stat.h>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

Mat filterImage(const Mat& inputImage,
				int radius,
				int levels){

	Mat output = Mat::zeros(inputImage.size(), inputImage.type());

	unsigned int imageWidth = inputImage.size().width;
	unsigned int imageHeight = inputImage.size().height;
	unsigned int length = levels + 1;

	for(unsigned int x=0; x<imageWidth; x++){
		for(unsigned int y=0; y<imageHeight; y++){
			unsigned int histogramm[length], r[length], g[length], b[length];

			for(unsigned int i=0; i<length; i++){
				histogramm[i] = r[i] = g[i] = b[i] = 0;
			}

			signed int xStart = x - radius;
			signed int yStart = y - radius;
			unsigned int xEnd = x + radius;
			unsigned int yEnd = y + radius;

			unsigned int maxValue = 1;
			unsigned int maxIndex = 0;

			xStart = xStart < 0 ? 0 : xStart;
			yStart = yStart < 0 ? 0 : yStart;
			xEnd = xEnd > imageWidth ? imageWidth : xEnd;
			yEnd = yEnd > imageHeight ? imageHeight : yEnd;

			for(unsigned int frameX=xStart; frameX<xEnd; frameX++){
				for(unsigned int frameY=yStart; frameY<yEnd; frameY++) {
					Vec3b pixel = inputImage.at<Vec3b>(frameY, frameX);

					unsigned int intensity = floor( ((pixel[0] + pixel[1] + pixel[2]) / 3.0f) * (levels / 255.0f) );
					
					histogramm[intensity]++;

					if(histogramm[intensity] > maxValue){
						maxValue = histogramm[intensity];
						maxIndex = intensity;
					}

					r[intensity] += pixel[0];
					g[intensity] += pixel[1];
					b[intensity] += pixel[2];
				}
			}

			unsigned int maxR = r[maxIndex] / maxValue;
			unsigned int maxG = g[maxIndex] / maxValue;
			unsigned int maxB = b[maxIndex] / maxValue;

			output.at<Vec3b>(y, x) = Vec3b(maxR, maxG, maxB);
		}
	}

	return output;
}

Mat computeMask( const Mat &inputImage,
				 const Rect &maskLocation,
				 int threshold,
				 int dilation_size){

	Mat mask = Mat::zeros(inputImage.size(), CV_8UC1);

	Mat inputImageHsv = Mat::zeros(inputImage.size(), CV_LOAD_IMAGE_COLOR);
	cvtColor(inputImage, inputImageHsv, CV_BGR2HSV);

	unsigned int maskFromX = maskLocation.x;
	unsigned int maskFromY = maskLocation.y;
	unsigned int maskToX = maskFromX + maskLocation.width;
	unsigned int maskToY = maskFromY + maskLocation.height;

	for(unsigned int x=maskFromX; x<maskToX; x++){
		for(unsigned int y=maskFromY; y<maskToY; y++){
			Vec3b pixel = inputImageHsv.at<Vec3b>(y, x);
			
			if(pixel[0] < threshold){
				mask.at<unsigned char>(y,x) = 0xFF;
			}
		}
	}

	dilate(mask, mask, getStructuringElement(CV_SHAPE_ELLIPSE, Size(dilation_size, dilation_size)));

	return mask;
}

Mat combineFilteredImageAndMask(const Mat& originalImage,
								const Mat& filteredImage,
								const Mat& mask) {

	Mat output = Mat::zeros(originalImage.size(), originalImage.type());

	unsigned int imageWidth = originalImage.size().width;
	unsigned int imageHeight = originalImage.size().height;

	for(unsigned int x=0; x<imageWidth; x++){
		for(unsigned int y=0; y<imageHeight; y++){
			unsigned char maskValue = mask.at<unsigned char>(y, x);
			output.at<Vec3b>(y, x) = maskValue != 0 ? originalImage.at<Vec3b>(y, x) : filteredImage.at<Vec3b>(y, x);
		}
	}

	return output;
}

int main() {
	Mat originalImage = imread("input/solo.jpg", CV_LOAD_IMAGE_COLOR);

	Mat filteredImage = filterImage(originalImage, 5, 20);
	imwrite("output/filtered.png", filteredImage);

	Mat filteredMask = computeMask(originalImage, Rect(280,580,220,160), 10, 17);
	imwrite("output/mask.png", filteredMask);

	Mat combinedImage = combineFilteredImageAndMask(originalImage, filteredImage, filteredMask);
	imwrite("output/combined.png", combinedImage);

	return 0;
}