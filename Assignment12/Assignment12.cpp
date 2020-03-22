#include <string>
using std::string;
using std::to_string;
#include <vector>
using std::vector;

#include <opencv2/opencv.hpp>

int main() {
    string rootPath = "../CamCalib/";
    cv::Size patternSize(6, 4);
    vector<vector<cv::Point2f>> cornersArray; // Save corners for each image
    vector<vector<cv::Point3f>> objectPointsArrary;
    // Initilaize a set of object points
    vector<cv::Point3f> objectPoints;
    for (int i = 0; i < patternSize.height; i++) {
        for (int j = 0; j < patternSize.width; j++) {
            objectPoints.push_back(cv::Point3f(i, j, 0.0f));
        }
    }

    cv::Size imageSize;

    // Total 14 images for finding corners
    for (int i = 1; i <= 14; i++) {
        string filename = (i < 10 ? "chessboard0" : "chessboard") + to_string(i);
        string imagePath = rootPath + filename + ".jpg";

        cv::Mat image = cv::imread(imagePath);
        imageSize = image.size();

        cv::Mat imageGrayscale;
        cv::cvtColor(image, imageGrayscale, cv::COLOR_RGB2GRAY);
        vector<cv::Point2f> corners;
        bool patternWasFound = cv::findChessboardCorners(imageGrayscale, patternSize, corners);
        if (patternWasFound) {
            // Criteria: iteration 30 or epsilson 0.1
            cv::cornerSubPix(imageGrayscale, corners, cv::Size(5, 5), cv::Size(-1, -1),
                cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 30, 0.1));
        }
        cornersArray.push_back(corners);
        objectPointsArrary.push_back(objectPoints);

        cv::Mat imageWithCorners = image.clone();
        // Draw corners
        cv::drawChessboardCorners(imageWithCorners, patternSize, corners, patternWasFound);
        cv::imwrite("./Saved/CamCalibResults/Corners/corners_of_" +
            filename + ".jpg", imageWithCorners); // Save the image with corners
    }

    cv::Mat cameraMatrix, distCoeffs, map1, map2;
    vector<cv::Mat> rvecs, tvecs;
    cv::calibrateCamera(objectPointsArrary, cornersArray, imageSize,
        cameraMatrix, distCoeffs, rvecs, tvecs);
    cv::initUndistortRectifyMap(cameraMatrix, distCoeffs,
        cv::Mat(), cv::Mat(), imageSize, CV_32F, map1, map2);

    /*
    cout << cameraMatrix << "\n\n";
    cv::Mat rotationMatrix;
    cv::Rodrigues(rvecs[0], rotationMatrix);
    cout << rotationMatrix << endl << tvecs[0] << "\n\n";
    cv::Rodrigues(rvecs[7], rotationMatrix);
    cout << rotationMatrix << endl << tvecs[7] << "\n\n";
    cv::Rodrigues(rvecs[11], rotationMatrix);
    cout << rotationMatrix << endl << tvecs[11] << "\n\n";
    */

    // Remap each image
    for (int i = 1; i <= 14; i++) {
        string filename = (i < 10 ? "chessboard0" : "chessboard") + to_string(i);
        string imagePath = rootPath + filename + ".jpg";

        cv::Mat image = cv::imread(imagePath);
        cv::Mat calibratedImage;
        cv::remap(image, calibratedImage, map1, map2, cv::INTER_LINEAR);
        cv::imwrite("./Remapped/remapped_" +
            filename + ".jpg", calibratedImage); // Save the calibrated image
    }

    return 0;
}