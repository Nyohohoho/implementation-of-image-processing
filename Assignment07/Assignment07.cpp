#include <opencv2/opencv.hpp>

int main() {
    cv::Mat image = cv::imread("../Images/Gyro_Zeppeli.jpg", cv::IMREAD_GRAYSCALE);
    cv::imshow("Original image", image);

    // Smooth the image to reduce noise before detecting edges
    cv::Mat smoothed;
    cv::GaussianBlur(image, smoothed, cv::Size(3, 3), 0.8);

    /*--------------------------------------------------------------------------*/

    // Here is using Sobel operator to detect edges
    // Compute gradient of x and y separately
    cv::Mat grad_x, grad_y;
    // For horizontal axis, use kernel size of 3
    cv::Sobel(smoothed, grad_x, CV_16S, 1, 0, 3);
    // For vertical axis, use kernel size of 3
    cv::Sobel(smoothed, grad_y, CV_16S, 0, 1, 3);

    // Compute absolute values of gradients
    cv::Mat abs_grad_x, abs_grad_y;
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);

    // Compute grad(x,y) = |grad_x(x,y)| * 0.5 + |grad_y(x,y)| * 0.5 + 0
    cv::Mat grad;
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    cv::imshow("Sobel", grad);

    /*--------------------------------------------------------------------------*/

    // Here is using Laplacian operator to detect edges
    cv::Mat laplacian_image;
    // Use kernel size of 3
    cv::Laplacian(smoothed, laplacian_image, CV_16S, 3);

    // converting back to CV_8U
    cv::Mat abs_laplacian_image;
    cv::convertScaleAbs(laplacian_image, abs_laplacian_image);
    cv::imshow("Lapalacian", abs_laplacian_image);

    /*--------------------------------------------------------------------------*/

    // Here is using Canny operator to detect edges
    cv::Mat canny_image;
    // Use thresholds 3 and 9 and size 3 for Sobel operator
    cv::Canny(image, canny_image, 3, 9, 3);
    cv::imshow("Canny", canny_image);

    cv::waitKey(0);
    return 0;
}