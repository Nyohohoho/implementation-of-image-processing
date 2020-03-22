#include <string>
using std::string;

#include <opencv2/opencv.hpp>

int main() {
    string imagePath = "../Images/Jotaro.jpg";
    cv::Mat image = cv::imread(imagePath);
    cv::imshow("Original image", image);

    cv::Mat imageGrayscale;
    cv::cvtColor(image, imageGrayscale, cv::COLOR_RGB2GRAY);
    cv::imshow("Image converted to grayscale", imageGrayscale);

    cv::waitKey(0);
    return 0;
}