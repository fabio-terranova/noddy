#include "Filter.h"
#include <Eigen/Dense>
#include <chrono>
#include <iostream>

using Eigen::VectorXcd;
using Noddy::Filter::Coeffs;
using Noddy::Filter::Complex;
using Noddy::Filter::ZPK;

Eigen::IOFormat g_cleanFmt(Eigen::StreamPrecision, 0, " ", "", "", "", "[",
                           "]");

class Timer {
  using Second = std::chrono::duration<double, std::ratio<1>>;
  using Clock  = std::chrono::steady_clock;

public:
  void reset() { m_beg = Clock::now(); }

  double elapsed() const {
    return std::chrono::duration_cast<Second>(Clock::now() - m_beg).count();
  }

private:
  std::chrono::time_point<Clock> m_beg{Clock::now()};
};

bool operator==(const Coeffs& first, const Coeffs& second) {
  if (first.a != second.a)
    return false;
  if (first.b != second.b)
    return false;

  return true;
}

std::ostream& operator<<(std::ostream& os, const Coeffs& coeffs) {
  os << "b: " << coeffs.b.format(g_cleanFmt) << "\n";
  os << "a: " << coeffs.a.format(g_cleanFmt) << "\n";
  return os;
}

bool test_zpk2tf() {
  using namespace Noddy::Filter;

  // Test zeros-poles-gain to polynomial coefficents
  ZPK filter{VectorXcd{2}, VectorXcd{2}, 5.0};
  filter.z << 2, 6;
  filter.p << 1, 8;

  Coeffs expected{VectorXcd{3}, VectorXcd{3}};
  expected.b << 5.0, -40.0, 60.0;
  expected.a << 1.0, -9.0, 8.0;

  Coeffs result{zpk2tf(filter)};

  std::cout << "--- ZPK to transfer function coefficients ---" << '\n';
  std::cout << "Expected coeffs:\n" << expected << '\n';
  std::cout << "Result coeffs:\n" << result << '\n';

  return (expected == result);
}

std::ostream& operator<<(std::ostream& os, const ZPK& zpk) {
  os << "k: " << zpk.k << "\n";
  os << "z: " << zpk.z.format(g_cleanFmt) << "\n";
  os << "p: " << zpk.p.format(g_cleanFmt) << "\n";
  return os;
}

bool test_filterDesign() {
  using namespace Noddy::Filter;

  const auto order{2};
  const auto fc{100.0};
  const auto fs{1000.0};
  const auto ftype{lowpass};

  { // Butterworth
    ZPK digitalFilter{iirFilter<buttap, ftype>(order, fc, fs)};

    std::cout << "--- Design Butterworth filter ---" << '\n';
    std::cout << digitalFilter << '\n';
  }

  { // Chebyshev I
    const auto rp{5.0};

    ZPK digitalFilter{iirFilter<cheb1ap, ftype>(order, fc, fs, rp)};

    std::cout << "--- Design Chebyshev I filter ---" << '\n';
    std::cout << digitalFilter << '\n';
  }

  { // Chebyshev II
    const auto rs{4.0};

    ZPK digitalFilter{iirFilter<cheb2ap, ftype>(order, fc, fs, rs)};

    std::cout << "--- Design Chebyshev II filter ---" << '\n';
    std::cout << digitalFilter << '\n';
  }

  return true;
}

int main() {
  assert(test_zpk2tf());
  test_filterDesign();

  return 0;
}
