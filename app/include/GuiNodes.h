#ifndef INCLUDE_INCLUDE_GUINODES_H_
#define INCLUDE_INCLUDE_GUINODES_H_

#include "Filter.h"
#include "Node.h"
#include "imgui.h"
#include "nlohmann/json_fwd.hpp"
#include <Eigen/Dense>
#include <cstddef>
#include <string_view>
#include <vector>

namespace Nodex::App {
using namespace Nodex::Filter;
using namespace Nodex::Core;

// Default parameters (general)
constexpr double kDefaultSamplingFreq = 1000.0;
constexpr int    kDefaultSamples      = 1000;

// Default parameters (MixerNode)
constexpr int    kNumInputs   = 2;
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
  MixerNode(const std::string_view name, const std::size_t inputs = 2,
            const std::vector<double>& gains = std::vector<double>{})
      : Node{name, "Mixer"}, m_inputs{inputs}, m_gains{gains} {
    if (m_gains.empty()) {
      m_gains = std::vector<double>(m_inputs, kDefaultGain);
    }

    for (int i{0}; i < m_inputs; ++i) {
      std::string portName = "In " + std::to_string(i + 1);
      addInput<Eigen::ArrayXd>(portName, Eigen::ArrayXd{});
    }

    addOutput<Eigen::ArrayXd>("Out", [this]() { return getData(); });
  }

  Eigen::ArrayXd getData() {
    Eigen::ArrayXd result;

    Eigen::Index maxSize = 0;
    for (std::size_t i{0}; i < m_inputs; ++i) {
      auto data = inputValue<Eigen::ArrayXd>("In " + std::to_string(i + 1));
      if (data.size() > maxSize) {
        maxSize = data.size();
      }
    }

    result = Eigen::ArrayXd::Zero(maxSize);
    for (std::size_t i{0}; i < m_inputs; ++i) {
      auto data = inputValue<Eigen::ArrayXd>("In " + std::to_string(i + 1));
      if (data.size() > 0) {
        // match size by padding with zeros if needed
        Eigen::ArrayXd dataResized    = Eigen::ArrayXd::Zero(maxSize);
        dataResized.head(data.size()) = data;
        result += m_gains[i] * dataResized;
      }
    }

    return result;
  }

  void render() override {
    for (std::size_t i{0}; i < m_inputs; ++i) {
      ImGui::InputDouble(("Gain " + std::to_string(i + 1)).c_str(), &m_gains[i],
                         0.1, 1.0, "%.2f");
    }
  }

  nlohmann::json serialize() const override {
    nlohmann::json j = Node::serialize();

    j["type"]       = "MixerNode";
    j["parameters"] = {
        {"inputs", m_inputs},
        { "gains",  m_gains}
    };

    return j;
  }

private:
  std::size_t         m_inputs{};
  std::vector<double> m_gains{};
};

class ViewerNode : public Node {
public:
  ViewerNode(const std::string_view name);

  void render() override;

  nlohmann::json serialize() const override {
    nlohmann::json j = Node::serialize();
    j["type"]        = "ViewerNode";

    return j;
  }
};

class RandomDataNode : public Node {
public:
  RandomDataNode(const std::string_view name, const int size = kDefaultSamples);

  void render() override;

  nlohmann::json serialize() const override {
    nlohmann::json j = Node::serialize();
    j["type"]        = "RandomDataNode";
    j["parameters"]  = {
        {"samples", m_samples}
    };

    return j;
  }

private:
  int            m_samples{};
  Eigen::ArrayXd m_data{};
};

class SineNode : public Node {
public:
  SineNode(const std::string_view name, int size = kDefaultSamples,
           const double frequency = kDefaultFrequency,
           const double amplitude = kDefaultAmplitude,
           const double phase     = kDefaultPhase,
           const double fs        = kDefaultSamplingFreq,
           const double offset    = kDefaultOffset);

  void render() override;

  nlohmann::json serialize() const override {
    nlohmann::json j = Node::serialize();

    j["type"]       = "SineNode";
    j["parameters"] = {
        {  "samples",      m_samples},
        {"frequency",    m_frequency},
        {"amplitude",    m_amplitude},
        {    "phase",        m_phase},
        {       "fs", m_samplingFreq},
        {   "offset",       m_offset}
    };

    return j;
  }

private:
  Eigen::ArrayXd generateWave();

  int    m_samples{};
  double m_frequency{};
  double m_amplitude{};
  double m_phase{};
  double m_samplingFreq{};
  double m_offset{};
};

class FilterNode : public Node {
public:
  FilterNode(const std::string_view name,
             const Filter::Mode     mode         = kDefaultFilterMode,
             const Filter::Type     type         = kDefaultFilterType,
             const int              order        = kDefaultFilterOrder,
             const double           cutoffFreq   = kDefaultCutoffFreq,
             const double           samplingFreq = kDefaultSamplingFreq);

  void render() override;

  nlohmann::json serialize() const override {
    nlohmann::json j = Node::serialize();
    j["type"]        = "FilterNode";
    j["parameters"]  = {
        {  "filter_mode", static_cast<int>(m_filterMode)},
        {  "filter_type", static_cast<int>(m_filterType)},
        { "filter_order",                  m_filterOrder},
        {  "cutoff_freq",                   m_cutoffFreq},
        {"sampling_freq",                 m_samplingFreq}
    };

    return j;
  }

private:
  Nodex::Filter::Mode m_filterMode{kDefaultFilterMode};
  Nodex::Filter::Type m_filterType{kDefaultFilterType};
  int                 m_filterOrder{kDefaultFilterOrder};
  double              m_cutoffFreq{kDefaultCutoffFreq};
  double              m_samplingFreq{kDefaultSamplingFreq};
};
} // namespace Nodex::App

#endif // INCLUDE_INCLUDE_GUINODES_H_
