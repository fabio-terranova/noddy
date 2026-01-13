#include "Filter.h"
#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(noddy_bind, m, py::mod_gil_not_used()) {
  using namespace Noddy::Filter;

  m.doc() = "pybind11 bindings";

  m.def(
      "fft_filter",
      [](const VectorXd& b, const VectorXd& a, const VectorXd& x,
         double epsilon, int max_length) {
        VectorXd output{
            Noddy::Filter::fftFilter({b, a}, x, epsilon, max_length)};

        return output;
      },
      py::arg("b"), py::arg("a"), py::arg("x"), py::arg("epsilon") = 1e-12,
      py::arg("max_length") = 10000);

  m.def(
      "lfilter",
      [](const VectorXd& b, const VectorXd& a, const VectorXd& x) {
        Coeffs filter(b, a);

        VectorXd output{Noddy::Filter::linearFilter(filter, x)};

        return output;
      },
      py::arg("b"), py::arg("a"), py::arg("x"));
}
