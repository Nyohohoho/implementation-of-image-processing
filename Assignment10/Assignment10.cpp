#include <opencv2/opencv.hpp>

// Binarize a grayscale image (q10)
static void binarize(int threshval, void*) {
    cv::Mat image = cv::imread("../Images/Microbe.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat binarized_img;

    // Binarize the input image
    cv::threshold(image, binarized_img, threshval, 255, cv::THRESH_BINARY);
    cv::imshow("binarized image", binarized_img);
}

int main() {
    const int max_trackbar_value = 255;
    int current_threshold = 127;

    cv::namedWindow("binarized image", 1);
    // Create trackbar
    cv::createTrackbar("threshold", "binarized image", &current_threshold, max_trackbar_value, binarize);
    binarize(current_threshold, nullptr);

    cv::waitKey(0);
    return 0;
}