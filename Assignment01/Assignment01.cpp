#include <string>
using std::string;

#include <opencv2/opencv.hpp>

int main() {
    string image_path = "../Images/JoJo_characters.jpg";
    cv::Mat image = cv::imread(image_path);
    cv::imshow("JoJo", image);

    cv::waitKey(0);
    return 0;
}