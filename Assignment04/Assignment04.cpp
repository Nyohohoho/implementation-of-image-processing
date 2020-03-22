#include <string>
using std::string;
using std::to_string;
#include <vector>
using std::vector;

#include <opencv2/opencv.hpp>

int main() {
    string imagePath = "../Images/JoJo_25th_aniversary.jpg";
    cv::Mat image = cv::imread(imagePath);
    cv::namedWindow("Original image", 0);
    cv::resizeWindow("Original image", cv::Size(640, 360));
    cv::imshow("Original image", image);

    // Prepare a vector of Mat for 3 channels
    vector<cv::Mat> rgbChannels(3);
    // Split channels and save them separately
    cv::split(image, rgbChannels);
    // Swap channel green and blue
    swap(rgbChannels[1], rgbChannels[2]);
    cv::Mat swappedImage;
    // Merge channels to re-produce an image
    cv::merge(rgbChannels, swappedImage);
    cv::namedWindow("Swapped image", 0);
    cv::resizeWindow("Swapped image", cv::Size(640, 360));
    cv::imshow("Swapped image", swappedImage);

    cv::waitKey(0);
    return 0;
}