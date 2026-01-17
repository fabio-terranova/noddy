set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(pynodex ./python/src/module.cpp)
target_link_libraries(pynodex PRIVATE nodex_core)