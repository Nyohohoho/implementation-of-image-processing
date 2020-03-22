#include <opencv2/opencv.hpp>

// Add Gaussian noise to an image (q8)
void add_gaussian_noise(const cv::Mat& input, cv::Mat& output) {
    const int row_len = input.rows;
    const int col_len = input.cols;

    cv::Mat gaussian_noise = cv::Mat::zeros(row_len, col_len, input.type());
    cv::RNG rng;
    // Create Gaussian noise with mean of 20 and variance of 30
    rng.fill(gaussian_noise, cv::RNG::NORMAL, 20, 30);
    output = input + gaussian_noise;
}

// Add salt-and-pepper noise to an image (q8)
void add_salt_pepper_noise(const cv::Mat& input, cv::Mat& output) {
    output = input.clone();
    const int row_len = input.rows;
    const int col_len = input.cols;

    // 20% of pixels are polluted by salt and pepper
    int salt_num = row_len * col_len * 0.1;
    int pepper_num = row_len * col_len * 0.1;
    srand((unsigned)time(NULL));
    // Salt
    for (int i = 0; i < salt_num; i++) {
        int r = ((unsigned)rand()) % row_len;
        int c = ((unsigned)rand()) % col_len;
        output.at<uchar>(r, c) = 250;
    }
    // Pepper
    for (int i = 0; i < pepper_num; i++) {
        int r = ((unsigned)rand()) % row_len;
        int c = ((unsigned)rand()) % col_len;
        output.at<uchar>(r, c) = 5;
    }
}

int main() {
    cv::Mat image_grayscale = cv::imread("../Images/JoJolionV10.jpg", cv::IMREAD_GRAYSCALE);
    cv::imshow("Original image", image_grayscale);

    cv::Mat gaussian_image;
    add_gaussian_noise(image_grayscale, gaussian_image);
    cv::imshow("Gaussian noise added", gaussian_image);

    cv::Mat salt_pepper_image;
    add_salt_pepper_noise(image_grayscale, salt_pepper_image);
    cv::imshow("Salt-and-pepper noise added", salt_pepper_image);

    cv::waitKey(0);
    return 0;
}