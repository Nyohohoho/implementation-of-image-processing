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

    // Define the rotation center
    cv::Point centerOfImage(image.cols / 2, image.rows / 2);
    // Define the rotation angle
    double leftAngle = 45.0;
    double rightAngle = -45.0;
    // Define the scale
    double scaleOfImage = 0.6;
    // Define the rotation matrix
    cv::Mat leftRotationMatrix = cv::getRotationMatrix2D(centerOfImage,
        leftAngle, scaleOfImage);
    cv::Mat rightRotationMatrix = cv::getRotationMatrix2D(centerOfImage,
        rightAngle, scaleOfImage);

    cv::Mat rotatedImage;
    cv::warpAffine(image, rotatedImage, leftRotationMatrix, image.size());
    cv::namedWindow("Left rotated image", 0);
    cv::resizeWindow("Left rotated image", cv::Size(640, 360));
    cv::imshow("Left rotated image", rotatedImage);

    cv::warpAffine(image, rotatedImage, rightRotationMatrix, image.size());
    cv::namedWindow("Right rotated image", 0);
    cv::resizeWindow("Right rotated image", cv::Size(640, 360));
    cv::imshow("Right rotated image", rotatedImage);

    cv::waitKey(0);
    return 0;
}