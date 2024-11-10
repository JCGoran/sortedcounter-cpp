#include <nanobind/nanobind.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/set.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/unordered_map.h>
#include <nanobind/stl/vector.h>

#include "sortedcounter.hpp"

namespace nb = nanobind;
using namespace nb::literals;

// interface to useful types
NB_MODULE(sortedcounter, my_module) {
  using value_type = double;
  using ValueSortedCounter = SortedCounter<value_type>;
  nb::class_<ValueSortedCounter>(my_module, "SortedCounter")
      .def(nb::init<>())
      .def(nb::init<std::vector<value_type>>())
      .def(nb::init<std::set<value_type>>())
      .def(nb::init<std::map<value_type, int>>())
      .def(nb::init<std::unordered_map<value_type, int>>())
      .def("maximum", &ValueSortedCounter::maximum)
      .def("minimum", &ValueSortedCounter::minimum)
      .def("__copy__", &ValueSortedCounter::copy)
      .def("__deepcopy__", &ValueSortedCounter::copy)
      .def("__getitem__", &ValueSortedCounter::get)
      .def("__len__", &ValueSortedCounter::size)
      .def("__repr__",
           [](const ValueSortedCounter &c) {
             std::string result = "SortedCounter({";
             auto data = c.data();
             auto size = data.size();
             auto counter = 0;
             for (const auto &[key, value] : data) {
               result += std::to_string(key) + ": " + std::to_string(value);
               if (counter < size - 1) {
                 result += ", ";
               }
               ++counter;
             }
             result += "})";
             return result;
           })
      .def("add", &ValueSortedCounter::add, "key"_a, "times"_a = 1)
      .def("remove", &ValueSortedCounter::remove, "key"_a, "times"_a = 1)
      .def("extend", &ValueSortedCounter::extend<std::vector<value_type>>)
      .def("extend", &ValueSortedCounter::extend<std::set<value_type>>)
      .def("extend", &ValueSortedCounter::extend<std::map<value_type, int>>)
      .def("extend",
           &ValueSortedCounter::extend<std::unordered_map<value_type, int>>)
      .def(
          "to_dict",
          [](const ValueSortedCounter &c) {
            nb::dict result;
            for (const auto &[key, value] : c.data()) {
              result[nb::cast(key)] = nb::cast(value);
            }
            return result;
          },
          "Return a copy of the SortedCounter as a dictionary");
}
