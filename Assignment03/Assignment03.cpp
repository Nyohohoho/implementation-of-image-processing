#include <string>
using std::string;
using std::to_string;

#include <opencv2/opencv.hpp>

int main() {
    string video_path = "../Videos/JoJo_10s_video.mp4";
    cv::VideoCapture video(video_path);

    int totalFrameNumber = video.get(cv::CAP_PROP_FRAME_COUNT);
    double frameRate = video.get(cv::CAP_PROP_FPS);

    // Extract 3 frames per second
    const int framesExtractedPerSecond = 3;
    // How many frames should be skipped for extracting 3 frames per second
    int framesToSkipPerSecond = round(frameRate / double(framesExtractedPerSecond));

    cv::Mat frame;
    // Count the seconds for filename
    int counterForSecond = 1;
    // Count the frames in a second for filename
    int counterForFrame = 1;
    for (int i = 0; i < totalFrameNumber; i++) {
        // Read frame and if no frame is read, break the loop.
        if (!video.read(frame)) {
            break;
        }
        // To extract 3 frames per second
        if (i % framesToSkipPerSecond == 0) {
            string filename = "./FrameSequence/" + to_string(counterForSecond)
                + "s-" + to_string(counterForFrame) + ".jpg";
            cv::imwrite(filename, frame);
            // If 3 frames in a second has been read
            if (counterForFrame % framesExtractedPerSecond == 0) {
                counterForSecond++;
                counterForFrame = 1;
                // Skip the following increment
                continue;
            }
            counterForFrame++;
        }
    }

    return 0;
}