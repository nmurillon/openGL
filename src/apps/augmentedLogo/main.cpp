#include "application/AugmentedLogoLayer.hpp"

#include <libs/core/Application.hpp>

#include <memory>

// #include <opencv2/aruco.hpp>
// #include <opencv2/aruco/charuco.hpp>

// static inline void detectCharucoBoardWithCalibrationPose() {
//   cv::VideoCapture inputVideo;
//   inputVideo.open(0);
//   cv::Mat cameraMatrix, distCoeffs;

//   // Calib params from blender video
//   cameraMatrix = (cv::Mat_<double>(3, 3) << 2667.0, 0., 960.0, .0, 2667.0,
//                   540.0, .0, .0, 1.0);
//   distCoeffs = (cv::Mat_<double>(1, 5) << 0.0, 0.0, 0.0, 0.0, 0.0);

//   cv::Ptr<cv::aruco::Dictionary> dictionary =
//       cv::aruco::getPredefinedDictionary(cv::aruco::DICT_7X7_250);
//   cv::Ptr<cv::aruco::CharucoBoard> board =
//       cv::aruco::CharucoBoard::create(13, 9, 0.02f, 0.016f, dictionary);
//   cv::Ptr<cv::aruco::DetectorParameters> params =
//       cv::aruco::DetectorParameters::create();
//   while (inputVideo.grab()) {
//     cv::Mat image;
//     cv::Mat imageCopy;
//     inputVideo.retrieve(image);
//     image.copyTo(imageCopy);
//     std::vector<int> markerIds;
//     std::vector<std::vector<cv::Point2f>> markerCorners;
//     cv::aruco::detectMarkers(image, board->dictionary, markerCorners,
//     markerIds,
//                              params);
//     // if at least one marker detected
//     if (markerIds.size() > 0) {
//       cv::aruco::drawDetectedMarkers(imageCopy, markerCorners, markerIds);
//       std::vector<cv::Point2f> charucoCorners;
//       std::vector<int> charucoIds;
//       cv::aruco::interpolateCornersCharuco(markerCorners, markerIds, image,
//                                            board, charucoCorners, charucoIds,
//                                            cameraMatrix, distCoeffs);
//       // if at least one charuco corner detected
//       if (charucoIds.size() > 0) {
//         cv::Scalar color = cv::Scalar(255, 0, 0);
//         cv::aruco::drawDetectedCornersCharuco(imageCopy, charucoCorners,
//                                               charucoIds, color);
//         cv::Vec3d rvec, tvec;
//         bool valid = cv::aruco::estimatePoseCharucoBoard(
//             charucoCorners, charucoIds, board, cameraMatrix, distCoeffs,
//             rvec, tvec);
//         // if charuco pose is valid
//         if (valid)
//           cv::drawFrameAxes(imageCopy, cameraMatrix, distCoeffs, rvec, tvec,
//                             0.1f);
//       }
//     }
//     cv::imshow("out", imageCopy);
//     char key = (char)cv::waitKey(30);
//     if (key == 27)
//       break;
//   }
// }

// static inline void detectCharucoBoardWithoutCalibration() {
//   cv::VideoCapture inputVideo;
//   inputVideo.open(0);
//   cv::Ptr<cv::aruco::Dictionary> dictionary =
//       cv::aruco::getPredefinedDictionary(cv::aruco::DICT_7X7_250);
//   cv::Ptr<cv::aruco::CharucoBoard> board =
//       cv::aruco::CharucoBoard::create(13, 9, 0.02f, 0.016f, dictionary);
//   cv::Ptr<cv::aruco::DetectorParameters> params =
//       cv::aruco::DetectorParameters::create();
//   params->cornerRefinementMethod = cv::aruco::CORNER_REFINE_NONE;
//   while (inputVideo.grab()) {
//     cv::Mat image, imageCopy;
//     inputVideo.retrieve(image);
//     image.copyTo(imageCopy);
//     std::vector<int> markerIds;
//     std::vector<std::vector<cv::Point2f>> markerCorners;
//     cv::aruco::detectMarkers(image, board->dictionary, markerCorners,
//     markerIds,
//                              params);
//     // or
//     // cv::aruco::detectMarkers(image, dictionary, markerCorners, markerIds,
//     // params);
//     //  if at least one marker detected
//     if (markerIds.size() > 0) {
//       cv::aruco::drawDetectedMarkers(imageCopy, markerCorners, markerIds);
//       std::vector<cv::Point2f> charucoCorners;
//       std::vector<int> charucoIds;
//       cv::aruco::interpolateCornersCharuco(markerCorners, markerIds, image,
//                                            board, charucoCorners,
//                                            charucoIds);
//       // if at least one charuco corner detected
//       if (charucoIds.size() > 0)
//         cv::aruco::drawDetectedCornersCharuco(
//             imageCopy, charucoCorners, charucoIds, cv::Scalar(255, 0, 0));
//     }
//     cv::imshow("out", imageCopy);
//     char key = (char)cv::waitKey(30);
//     if (key == 27)
//       break;
//   }
// }

int main(int argc, char **argv) {
  // detectCharucoBoardWithCalibrationPose();
  libs::core::Application app{"Augmented Logo Application"};
  app.addLayer(std::make_shared<AugmentedLogoLayer>());

  app.run();
  return 0;
}