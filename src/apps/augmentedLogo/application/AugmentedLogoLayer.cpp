#include "AugmentedLogoLayer.hpp"

#include <libs/events/EventDispatcher.hpp>
#include <libs/io/ProgramPath.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <format>
#include <iostream>
#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <stb_image/stb_image.h>

unsigned int loadTexture(const std::string &textureFile,
                         GLenum textureUnit = GL_TEXTURE0,
                         GLint wrapping = GL_REPEAT) {
  int width, height, nChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data =
      stbi_load(textureFile.c_str(), &width, &height, &nChannels, 0);

  unsigned int texture;
  glGenTextures(1, &texture);

  // Bind the texture to work on it
  glActiveTexture(textureUnit);
  glBindTexture(GL_TEXTURE_2D, texture);

  // Set texture wrapping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "WARNING::TEXTURE::FAIL_TO_OPEN" << std::endl;
  }

  stbi_image_free(data);

  return texture;
}

AugmentedLogoLayer::AugmentedLogoLayer(const std::string &name) : Layer{name} {

  std::filesystem::path resourcesBaseDir =
      libs::io::ProgramPath::getInstance().getProgramDir() /
      AUGMENTED_LOGO_RESOURCES_FOLDER;

  m_shaderManager.addShader(
      "background",
      std::format("{}/shaders/backgroundVert.glsl", resourcesBaseDir.string()),
      std::format("{}/shaders/backgroundFrag.glsl", resourcesBaseDir.string()));

  m_shaderManager.addShader(
      "logo",
      std::format("{}/shaders/LogoVert.glsl", resourcesBaseDir.string()),
      std::format("{}/shaders/LogoFrag.glsl", resourcesBaseDir.string()));

  auto texture = loadTexture(
      std::format("{}/textures/totk.jpg", resourcesBaseDir.string()),
      GL_TEXTURE0, GL_REPEAT);

  // Setup background quad
  glGenVertexArrays(1, &m_vaoBackground);
  glGenBuffers(1, &m_vboBackground);
  glGenBuffers(1, &m_ebo);

  glBindBuffer(GL_ARRAY_BUFFER, m_vboBackground);
  glBufferData(GL_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(sizeof(float) * m_vertices.size()),
               m_vertices.data(), GL_STATIC_DRAW);

  glBindVertexArray(m_vaoBackground);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        reinterpret_cast<void *>(sizeof(float) * 3));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(m_indices.size() * sizeof(int)),
               m_indices.data(), GL_STATIC_DRAW);

  // Setup Logo quad
  glGenVertexArrays(1, &m_vaoLogo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vboBackground);
  glBindVertexArray(m_vaoLogo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        reinterpret_cast<void *>(sizeof(float) * 3));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  m_inputVideo.open(0);
  // initialize window size from the capture if available
  if (m_inputVideo.isOpened()) {
    int w = static_cast<int>(m_inputVideo.get(cv::CAP_PROP_FRAME_WIDTH));
    int h = static_cast<int>(m_inputVideo.get(cv::CAP_PROP_FRAME_HEIGHT));
    if (w > 0 && h > 0) {
      m_windowSize.width = w;
      m_windowSize.height = h;
    }
  }
  // compute projection matrix now that we know window/frame size
  m_projectionMatrix = projectionFromCameraMatrix();

  glGenTextures(1, &m_backgroundTexture);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  updateBackgroundTexture();
}

void AugmentedLogoLayer::onUpdate() {
  updateBackgroundTexture();
  auto model = getModelFromPos();
  auto view = glm::mat4(1.0f);

  auto shader = m_shaderManager.getShader("background");
  shader->use();
  shader->setInt("iChannel0", 1);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBindVertexArray(m_vaoBackground);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()),
                 GL_UNSIGNED_INT, 0);

  auto logo = m_shaderManager.getShader("logo");
  logo->use();
  logo->setInt("iChannel0", 0);
  logo->setMat4f("model", model);
  logo->setMat4f("view", view);
  logo->setMat4f("projection", glm::mat4(1.0f));
  glBindVertexArray(m_vaoLogo);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()),
                 GL_UNSIGNED_INT, 0);
}

void AugmentedLogoLayer::onEvent(libs::events::Event &event) {
  libs::events::EventDispatcher dispatcher{event};
  dispatcher.dispatch<libs::events::WindowResizeEvent>(
      LOGL_BIND_EVENT_FN(AugmentedLogoLayer::onWindowResized));
}

void AugmentedLogoLayer::updateBackgroundTexture() {
  cv::Mat image;
  m_inputVideo.retrieve(image);
  auto data = image.data;

  // Bind the texture to work on it
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_backgroundTexture);

  // Set texture wrapping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
}

bool AugmentedLogoLayer::onWindowResized(
    libs::events::WindowResizeEvent &event) {
  // m_windowSize.width = event.getWidth();
  // m_windowSize.height = event.getHeight();
  return event.handle();
}

// Build OpenGL projection matrix from camera intrinsics
glm::mat4 AugmentedLogoLayer::projectionFromCameraMatrix(float near,
                                                         float far) {
  double fx = m_cameraMatrix.at<double>(0, 0);
  double fy = m_cameraMatrix.at<double>(1, 1);
  double cx = m_cameraMatrix.at<double>(0, 2);
  double cy = m_cameraMatrix.at<double>(1, 2);
  // use the current window/frame size so projection matches the video/image
  int width = m_windowSize.width;
  int height = m_windowSize.height;

  glm::mat4 proj(0.0f);
  // Note: glm is column-major; proj[col][row]
  // Map camera intrinsics to OpenGL projection. We flip Y to convert from
  // image coordinates (origin top-left) to GL NDC (origin center, Y up).
  proj[0][0] = static_cast<float>(2.0 * fx / width);
  proj[1][1] = static_cast<float>(-2.0 * fy / height);
  proj[0][2] = static_cast<float>(2.0 * cx / width - 1.0);
  proj[1][2] = static_cast<float>(1.0 - 2.0 * cy / height);
  proj[2][2] = static_cast<float>(-(far + near) / (far - near));
  proj[2][3] = -1.0f;
  proj[3][2] = static_cast<float>(-2.0 * far * near / (far - near));
  return proj;
}

// Convert OpenCV rvec/tvec into glm model matrix (board-to-world)
glm::mat4
AugmentedLogoLayer::modelMatrixFromCvPose(const cv::Vec3d &rvec,
                                          const cv::Vec3d &tvec) const {
  cv::Mat R;
  cv::Rodrigues(rvec, R); // 3x3 rotation matrix

  // OpenCV: X right, Y down, Z forward.
  // We want model matrix that maps from object (board) coordinates to camera
  // coordinates (then to world/opengl space). Many pipelines use camera as
  // origin; we want a model matrix placing the board in world so the camera
  // view matrix is identity. A working approach is to construct a 4x4 with R
  // and t, then convert to glm and adjust handedness if needed.

  glm::mat4 model(1.0f);
  // Copy rotation (note OpenCV uses row-major Mat, glm is column-major)
  for (int r = 0; r < 3; ++r) {
    for (int c = 0; c < 3; ++c)
      model[c][r] = static_cast<float>(R.at<double>(r, c));
  }
  // translation
  model[3][0] = static_cast<float>(tvec[0]);
  model[3][1] = static_cast<float>(tvec[1]);
  model[3][2] = static_cast<float>(tvec[2]);
  // Usually you need to rotate around X to flip Y axis: preview and adjust if
  // the logo is mirrored or upside-down. For example: glm::scale(model,
  // glm::vec3(1, -1, -1)) or multiply by correction matrix if needed.

  // model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.f, 0.0f,
  // 0.0f)); model = glm::scale(model, glm::vec3(1, -1, -1));

  return model;
}

// see https://docs.opencv.org/3.4/df/d4a/tutorial_charuco_detection.html

glm::mat4 AugmentedLogoLayer::getModelFromPos() {
  cv::Ptr<cv::aruco::Dictionary> dictionary =
      cv::aruco::getPredefinedDictionary(cv::aruco::DICT_7X7_250);
  cv::Ptr<cv::aruco::CharucoBoard> board =
      cv::aruco::CharucoBoard::create(13, 9, 0.02f, 0.016f, dictionary);
  cv::Ptr<cv::aruco::DetectorParameters> params =
      cv::aruco::DetectorParameters::create();

  if (!m_inputVideo.grab()) {
    return glm::mat4(1.0f);
  }

  cv::Mat image;
  m_inputVideo.retrieve(image);
  std::vector<int> markerIds;
  std::vector<std::vector<cv::Point2f>> markerCorners;
  cv::aruco::detectMarkers(image, board->dictionary, markerCorners, markerIds,
                           params);
  // if at least one marker detected
  if (markerIds.size() > 0) {
    std::vector<cv::Point2f> charucoCorners;
    std::vector<int> charucoIds;
    cv::aruco::interpolateCornersCharuco(markerCorners, markerIds, image, board,
                                         charucoCorners, charucoIds,
                                         m_cameraMatrix, m_distCoeffs);
    // if at least one charuco corner detected
    if (charucoIds.size() > 0) {
      cv::Scalar color = cv::Scalar(255, 0, 0);
      cv::Vec3d rvec, tvec;
      bool valid = cv::aruco::estimatePoseCharucoBoard(
          charucoCorners, charucoIds, board, m_cameraMatrix, m_distCoeffs, rvec,
          tvec);

      return modelMatrixFromCvPose(rvec, tvec);
    }
  }
}