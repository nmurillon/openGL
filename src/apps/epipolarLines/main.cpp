#include <format>
#include <libs/io/ProgramPath.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

const std::vector<std::string> images{"image_0011.jpg", "image_0007.jpg",
                                      "image_0001.jpg", "image_0006.jpg"};

void drawLines(cv::Mat &img1, cv::Mat &img2,
               const std::vector<cv::Point3f> &lines,
               const std::vector<cv::Point2f> &pts1,
               const std::vector<cv::Point2f> &pts2) {
  auto r = img1.rows;
  auto c = img1.cols;

  for (std::size_t i{0}; i < pts1.size(); ++i) {
    cv::Scalar color{static_cast<double>(rand() % 256),
                     static_cast<double>(rand() % 256),
                     static_cast<double>(rand() % 256)};

    auto line = lines.at(i);
    auto pt1 = pts1.at(i);
    auto pt2 = pts2.at(i);

    int x0, y0, x1, y1;
    x0 = 0;
    y0 = int(-line.z / line.y);
    x1 = c;
    y1 = int(-(line.z + line.x * c) / line.y);

    cv::line(img1, cv::Point{x0, y0}, cv::Point{x1, y1}, color);
    cv::circle(img1, pt1, 5, color);
    cv::circle(img2, pt2, 5, color);
  }
}

/**
 * Images have a logo at the bottom right, this mask removes it
 */
cv::Mat getMask(int rows, int cols) {
  cv::Mat mask = cv::Mat::zeros(rows, cols, CV_8UC1);
  cv::rectangle(mask, {0, 0}, {cols, static_cast<int>(0.92 * rows)}, 255, -1);

  return mask;
}

int main(int argc, char **argv) {
  const auto resourcesDir =
      libs::io::ProgramPath::getInstance().getProgramDir() /
      EPIPOLAR_LINES_RESOURCES_FOLDER / "images";

  auto i1 = images[0];
  for (std::size_t i{1}; i < images.size(); ++i) {

    auto i2 = images[i];

    auto img1 = cv::imread((resourcesDir / i1).string());
    auto img2 = cv::imread((resourcesDir / i2).string());

    auto sift = cv::SIFT::create();

    std::vector<cv::KeyPoint> kp1, kp2;
    cv::Mat des1, des2;
    auto mask = getMask(img1.rows, img1.cols);

    sift->detectAndCompute(img1, mask, kp1, des1);
    sift->detectAndCompute(img2, mask, kp2, des2);

    // FLANN params
    auto indexParams = cv::makePtr<cv::flann::KDTreeIndexParams>(5);
    auto searchParams = cv::makePtr<cv::flann::SearchParams>(50);

    cv::FlannBasedMatcher flann{indexParams, searchParams};
    // cv::FlannBasedMatcher flann{};

    std::vector<std::vector<cv::DMatch>> matches;
    flann.knnMatch(des1, des2, matches, 2);

    std::vector<cv::Point2f> points1{};
    std::vector<cv::Point2f> points2{};

    for (const auto &match : matches) {
      auto m = match.at(0);
      auto n = match.at(1);
      if (m.distance < 0.8 * n.distance) {
        points2.emplace_back(kp2.at(m.trainIdx).pt);
        points1.emplace_back(kp1.at(m.queryIdx).pt);
      }
    };

    std::vector<uchar> masks;
    masks.reserve(points1.size());
    double ransacReprojThreshold = 3.;
    double confidence = 0.99;
    auto F = cv::findFundamentalMat(points1, points2, masks, cv::FM_RANSAC,
                                    ransacReprojThreshold, confidence);

    std::vector<cv::Point2f> pts1{};
    std::vector<cv::Point2f> pts2{};

    for (std::size_t i = 0; i < points1.size(); ++i) {
      if (masks[i] == 1) {
        pts1.emplace_back(points1.at(i));
        pts2.emplace_back(points2.at(i));
      }
    }

    std::vector<cv::Point3f> lines;
    cv::computeCorrespondEpilines(pts2, 2, F, lines);
    drawLines(img1, img2, lines, pts1, pts2);

    std::vector<cv::Point3f> lines2;
    cv::computeCorrespondEpilines(pts1, 1, F, lines2);
    drawLines(img2, img1, lines2, pts2, pts1);

    cv::imwrite((resourcesDir / std::format("{}_{}.jpg", i1, i2)).string(),
                img1);
    cv::imwrite((resourcesDir / std::format("{}_{}.jpg", i2, i1)).string(),
                img2);
  }

  return 0;
}