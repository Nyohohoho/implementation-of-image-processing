#include <string>
using std::string;

#include <opencv2/opencv.hpp>

int main() {
    const int DELAY_TIME = 30;
    string video_path = "../Videos/JoJo_5s_video.mp4";
    cv::VideoCapture video(video_path);
    cv::Mat frame;

    // Go on reading frame until all frames are read.
    while (video.read(frame)) {
        imshow("JoJo", frame);
        cv::waitKey(DELAY_TIME);
    }
    video.release();

    cv::waitKey(0);
    return 0;
}