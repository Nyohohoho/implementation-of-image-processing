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
    for (int i = 1; i <= 14; i++) {
        objectPointsArrary.push_back(objectPoints);
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
    }

    cv::Mat cameraMatrix, distCoeffs;
    vector<cv::Mat> rvecs, tvecs;
    cv::calibrateCamera(objectPointsArrary, cornersArray, imageSize,
        cameraMatrix, distCoeffs, rvecs, tvecs);

    double totalError = 0.0;
    for (int i = 0; i < 14; i++) {
        vector<cv::Point3f> spacePoint = objectPointsArrary[i];
        vector<cv::Point2f> reprojectedPoint;
        cv::projectPoints(spacePoint, rvecs[i], tvecs[i], cameraMatrix, distCoeffs, reprojectedPoint);

        // Compute error
        double error = cv::norm(cornersArray[i], reprojectedPoint, cv::NORM_L2);
        std::cout << "Reprojection Error " << i + 1 << ": " << error << std::endl;
        totalError += error;

        string filename = (i < 9 ? "chessboard0" : "chessboard") + to_string(i + 1);
        string imagePath = rootPath + filename + ".jpg";
        cv::Mat imageWithReprojected = cv::imread(imagePath);
        // Draw reprojected points
        cv::drawChessboardCorners(imageWithReprojected, patternSize, reprojectedPoint, false);
        cv::imwrite("./Reprojected/reprojected_point_of_" +
            filename + ".jpg", imageWithReprojected); // Save the image with reprojected point
    }
    std::cout << "Average Reprojection Error: " << totalError / 14;

    return 0;
}