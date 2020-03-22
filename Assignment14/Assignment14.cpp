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

    // Initialize X, Y, Z axes
    vector<cv::Point3f> axis = { cv::Point3f(3, 0, 0),
        cv::Point3f(0, 3, 0), cv::Point3f(0, 0, -3) };

    /*------------------------------------------------------------------------------------*/
    // Camera Calibration only for geting camera matrix and distortion coefficents
    cv::Size calibImageSize;

    // Total 14 images for finding corners
    for (int i = 1; i <= 14; i++) {
        string filename = (i < 10 ? "chessboard0" : "chessboard") + to_string(i);
        string imagePath = rootPath + filename + ".jpg";

        cv::Mat image = cv::imread(imagePath);
        calibImageSize = image.size();

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
    vector<cv::Mat> calibRvecs, calibTvecs; // They will not be used for pose of camera.
    cv::calibrateCamera(objectPointsArrary, cornersArray, calibImageSize,
        cameraMatrix, distCoeffs, calibRvecs, calibTvecs);
    /*------------------------------------------------------------------------------------*/


    for (int i = 1; i <= 14; i++) {
        string filename = (i < 10 ? "chessboard0" : "chessboard") + to_string(i);
        string imagePath = rootPath + filename + ".jpg";

        cv::Mat image = cv::imread(imagePath);
        cv::Size imageSize = image.size();

        cv::Mat imageGrayscale;
        cv::cvtColor(image, imageGrayscale, cv::COLOR_RGB2GRAY);
        vector<cv::Point2f> corners;
        bool patternWasFound = cv::findChessboardCorners(imageGrayscale, patternSize, corners);
        if (patternWasFound) {
            // Criteria: iteration 30 or epsilson 0.1
            cv::cornerSubPix(imageGrayscale, corners, cv::Size(5, 5), cv::Size(-1, -1),
                cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 30, 0.1));
        }

        // Use solvePnP to get pose of camera
        cv::Mat rvec, tvec;
        cv::solvePnP(objectPointsArrary[i - 1], corners,
            cameraMatrix, distCoeffs, rvec, tvec);
        // Project X, Y, Z axes to image
        vector<cv::Point2f> axisInImage;
        cv::projectPoints(axis, rvec, tvec,
            cameraMatrix, distCoeffs, axisInImage);

        // Draw object-coordinate system on the first corner
        cv::Point2f firstCorner = corners[0];
        cv::Mat imageWithCoordinate = image.clone();
        // Draw X in red (B, G, R)
        cv::line(imageWithCoordinate, firstCorner, axisInImage[0], cv::Scalar(0, 0, 255), 5);
        // Draw Y in green (B, G, R)
        cv::line(imageWithCoordinate, firstCorner, axisInImage[1], cv::Scalar(0, 255, 0), 5);
        // Draw Z in blue (B, G, R)
        cv::line(imageWithCoordinate, firstCorner, axisInImage[2], cv::Scalar(255, 0, 0), 5);

        cv::imwrite("./Coordinate/coordinate_of_" +
            filename + ".jpg", imageWithCoordinate); // Save the image with object-coordinate system
    }

    return 0;
}