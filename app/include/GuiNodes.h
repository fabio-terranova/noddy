#ifndef INCLUDE_INCLUDE_GUINODES_H_
#define INCLUDE_INCLUDE_GUINODES_H_

#include "Filter.h"
#include "Node.h"
#include "imgui.h"
#include <Eigen/Dense>
#include <string_view>
#include <iostream>

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
  MixerNode(std::string_view name, int inputs) : Node{name, "Mixer"}, m_inputs{inputs} {
    m_gains.reserve(m_inputs);
    for (int i = 0; i < m_inputs; ++i) {
      std::string portName = "In " + std::to_string(i + 1);
      addInput<Eigen::ArrayXd>(portName, Eigen::ArrayXd{});
      m_gains[i] = kDefaultGain;
    }

    addOutput<Eigen::ArrayXd>("Out", [this]() { return getData(); });
  }

  Eigen::ArrayXd getData() {
    Eigen::ArrayXd result;

    Eigen::Index maxSize = 0;
    for (int i = 0; i < m_inputs; ++i) {
      auto data = inputValue<Eigen::ArrayXd>("In " + std::to_string(i + 1));
      if (data.size() > maxSize) {
        maxSize = data.size();
      }
    }

    result = Eigen::ArrayXd::Zero(maxSize);
    for (int i = 0; i < m_inputs; ++i) {
      auto data = inputValue<Eigen::ArrayXd>("In " + std::to_string(i + 1));
      if (data.size() > 0) {
        // match size by padding with zeros if needed
        Eigen::ArrayXd dataResized = Eigen::ArrayXd::Zero(maxSize);
        dataResized.head(data.size()) = data;
        result += m_gains[i] * dataResized;
      }
    }

    return result;
  }

  void render() override {
    for (int i = 0; i < m_inputs; ++i) {
      ImGui::InputDouble(("Gain " + std::to_string(i + 1)).c_str(), &m_gains[i], 0.1, 1.0, "%.2f");
    }
  }


private:
  int m_inputs{};
  std::vector<double> m_gains{};
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
