#include <vector>
using std::vector;

#include <opencv2/opencv.hpp>

int main() {
    cv::Mat image = cv::imread("../Images/Geometric_shapes.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat binarized_img, labeled_img;

    cv::namedWindow("Origin grayscale image", 0);
    cv::resizeWindow("Origin grayscale image", cv::Size(640, 360));
    cv::imshow("Origin grayscale image", image);

    // Binarize the input image with thresholding value 127
    cv::threshold(image, binarized_img, 127, 255, cv::THRESH_BINARY);
    // Label the binarized image with connectivity=8
    int nLabels = cv::connectedComponents(binarized_img, labeled_img, 8);

    // Create color for each connected component
    vector<cv::Vec3b> colors(nLabels);
    // White background
    colors[0] = cv::Vec3b(255, 255, 255);
    for (int i = 1; i < nLabels; i++) {
        // Assign color randomly
        colors[i] = cv::Vec3b((uint(rand()) % 255), (uint(rand()) % 255), (uint(rand()) % 255));
    }

    cv::Mat color_labeled_img(image.size(), CV_8UC3);
    for (int i = 0; i < color_labeled_img.rows; i++) {
        for (int j = 0; j < color_labeled_img.cols; j++) {
            int label = labeled_img.at<int>(i, j);
            // Make the labeled image colorful
            color_labeled_img.at<cv::Vec3b>(i, j) = colors[label];
        }
    }

    cv::namedWindow("Connected components", 0);
    cv::resizeWindow("Connected components", cv::Size(640, 360));
    cv::imshow("Connected components", color_labeled_img);

    cv::waitKey(0);
    return 0;
}