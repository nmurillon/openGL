#pragma once

#include <libs/core/Layer.hpp>
#include <libs/events/WindowEvent.hpp>
#include <libs/renderer/ShaderManager.hpp>

#include <opencv2/opencv.hpp>
#include <vector>

struct Size {

  int width, height;
};

class AugmentedLogoLayer : public libs::core::Layer {
public:
  AugmentedLogoLayer(const std::string &name = "AugmentedLogoLayer");

  virtual ~AugmentedLogoLayer() = default;

  virtual void onUpdate() override;
  virtual void onEvent(libs::events::Event &event) override;

private:
  void updateBackgroundTexture();
  bool onWindowResized(libs::events::WindowResizeEvent &event);
  glm::mat4 projectionFromCameraMatrix(float near = 0.01f, float far = 100.0f);
  glm::mat4 modelMatrixFromCvPose(const cv::Vec3d &rvec,
                                  const cv::Vec3d &tvec) const;
  glm::mat4 getModelFromPos();
  void refreshBackgroundTexture();

  Size m_windowSize{800, 600};
  unsigned int m_vaoBackground, m_vaoLogo, m_vboBackground, m_ebo;
  std::vector<float> m_vertices{
      -1.0f, -1.0f, 0.0f, 0.f, 0.f, -1.0f, 1.0f,  0.0f, 0.f, 1.f,
      1.0f,  1.0f,  0.0f, 1.f, 1.f, 1.0f,  -1.0f, 0.0f, 1.f, 0.f,
  };
  std::vector<int> m_indices{0, 1, 2, 0, 2, 3};
  libs::renderer::ShaderManager m_shaderManager{};

  // Camera parameters
  const cv::Mat m_cameraMatrix = (cv::Mat_<double>(3, 3) << 2667.0, 0., 960.0,
                                  .0, 2667.0, 540.0, .0, .0, 1.0);
  const cv::Mat m_distCoeffs =
      (cv::Mat_<double>(1, 5) << 0.0, 0.0, 0.0, 0.0, 0.0);
  glm::mat4 m_projectionMatrix;

  // Video capture
  cv::VideoCapture m_inputVideo;

  // Texture background
  unsigned int m_backgroundTexture;
};