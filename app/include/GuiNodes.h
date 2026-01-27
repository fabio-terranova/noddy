#ifndef INCLUDE_INCLUDE_GUINODES_H_
#define INCLUDE_INCLUDE_GUINODES_H_

#include "Filter.h"
#include "Node.h"
#include "imgui.h"
#include <Eigen/Dense>
#include <string_view>

namespace Nodex::App {
using namespace Nodex::Filter;
using namespace Nodex::Core;

// Default parameters (general)
constexpr double kDefaultSamplingFreq = 1000.0;
constexpr int    kDefaultSamples      = 1000;

// Default parameters (MixerNode)
constexpr double kDefaultGain = 1.0;

// Default parameters (SineNode)
constexpr double kDefaultOffset    = 0.0;
constexpr double kDefaultAmplitude = 1.0;
constexpr double kDefaultFrequency = 50.0;
constexpr double kDefaultPhase     = 0.0;

// Default parameters (FilterNode)
constexpr Filter::Mode kDefaultFilterMode  = Filter::Mode::lowpass;
constexpr Filter::Type kDefaultFilterType  = Filter::Type::butter;
constexpr int          kDefaultFilterOrder = 2;
constexpr double       kDefaultCutoffFreq  = 100.0;

struct DragDropState {
  Port*  draggedPort{nullptr};
  ImVec2 dragStartPos{
      ImVec2{0, 0}
  };
  bool isDragging{false};
};

void graphWindow(Graph& graph);

class MixerNode : public Node {
public:
  MixerNode(std::string_view name);

  Eigen::ArrayXd getData(double k1, double k2);
  void           render() override;

private:
  double m_gain1{kDefaultGain};
  double m_gain2{kDefaultGain};
};

class ViewerNode : public Node {
public:
  ViewerNode(std::string_view name);

  void render() override;
};

class RandomDataNode : public Node {
public:
  RandomDataNode(std::string_view name, int size = kDefaultSamples);

  void render() override;

private:
  int            m_samples{};
  Eigen::ArrayXd m_data{};
};

class SineNode : public Node {
public:
  SineNode(std::string_view name);

  void render() override;

private:
  Eigen::ArrayXd generateWave();

  int    m_samples{kDefaultSamples};
  double m_frequency{kDefaultFrequency};
  double m_amplitude{kDefaultAmplitude};
  double m_phase{kDefaultPhase};
  double m_samplingFreq{kDefaultSamplingFreq};
  double m_offset{kDefaultOffset};
};

class DataNode : public Node {
public:
  DataNode(std::string_view name, Eigen::ArrayXd& data);

  void render() override;

private:
  Eigen::ArrayXd m_data{};
};

class FilterNode : public Node {
public:
  FilterNode(std::string_view name);

  void render() override;

private:
  Nodex::Filter::Mode m_filterMode{kDefaultFilterMode};
  Nodex::Filter::Type m_filterType{kDefaultFilterType};
  int                 m_filterOrder{kDefaultFilterOrder};
  double              m_cutoffFreq{kDefaultCutoffFreq};
  double              m_samplingFreq{kDefaultSamplingFreq};
};
} // namespace Nodex::App

#endif // INCLUDE_INCLUDE_GUINODES_H_
