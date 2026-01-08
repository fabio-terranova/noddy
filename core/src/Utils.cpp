#include "Utils.h"

namespace Noddy {
namespace Utils {
ArrayXi arange(const int start, int stop, const int step) {
  if (step == 0)
    return ArrayXi{};

  const int num{(stop - start + (step > 0 ? step - 1 : step + 1)) / step};
  if (num <= 0)
    return ArrayXi{};

  const int actualStop{start + (num - 1) * step};

  return ArrayXi::LinSpaced(num, start, actualStop);
}
} // namespace Utils
} // namespace Noddy
