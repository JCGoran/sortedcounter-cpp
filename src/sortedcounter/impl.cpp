#include <map>
#include <initializer_list>
#include <iostream>

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/unordered_map.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;
using namespace nb::literals;

template <typename T>
class SortedCounter {
    using size_type = std::uint64_t;
    private:
        size_type m_size;
        std::map<T, size_type> m_container;

    public:
        ~SortedCounter() = default;
        SortedCounter() : m_size(0) {}
        // TODO figure out other types as well
        template<typename Container>
        explicit SortedCounter(const Container& items) {
            m_size = 0;
            // does not need to be super-optimized
            for (const auto& item: items){
                if (!m_container.count(item)) {
                    m_container[item] = 1;
                }
                else{
                    m_container[item] += 1;
                }
                ++m_size;
            }
        }
        explicit SortedCounter(const std::unordered_map<T, int>& items) {
            m_size = 0;
            for (const auto& [key, value]: items) {
                if (value <= 0) {
                    throw nb::key_error("The count must be > 0.");
                }
                m_container[key] = value;
                m_size += value;
            }
        }
        SortedCounter(const std::initializer_list<T>& items) {
            m_size = 0;
            for (const auto& item: items){
                if (!m_container.count(item)) {
                    m_container[item] = 1;
                }
                else{
                    m_container[item] += 1;
                }
                ++m_size;
            }
        }
        void add(const T key, const std::int64_t times = 1) {
            if (times <= 0) {
                throw nb::key_error("Cannot add <= 0 items to container");
            }
            const auto present = m_container.count(key);
            if (!present){
                m_container[key] = times;
            }
            else{
                m_container[key] += times;
            }
            m_size += times;
        }
        void remove(const T key, const std::int64_t times = 1) {
            if (times <= 0) {
                throw nb::key_error("Cannot remove <= 0 items to container");
            }
            const auto present = m_container.count(key);
            if (!present){
                throw nb::key_error("Cannot remove non-existing value");
            }
            else{
                if (static_cast<std::int64_t>(m_container[key]) - times <= 0){
                    const auto remaining = m_container[key];
                    m_container.erase(key);
                    m_size -= remaining;
                }
                else{
                    m_container[key] -= times;
                    m_size -= times;
                }
            }
        }
        void extend(const std::vector<T>& items) {
            for (const auto& item: items) {
                const auto present = m_container.count(item);
                if (!present) {
                    m_container[item] = 1;
                }
                else {
                    m_container[item] += 1;
                }
                ++m_size;
            }
        }
        T maximum() {
            if (m_container.empty()) {
                throw nb::value_error("Container is empty!");
            }
            return (*m_container.rbegin()).first;
        }
        T minimum() {
            if (m_container.empty()) {
                throw nb::value_error("Container is empty!");
            }
            return (*m_container.begin()).first;
        }
        size_type size() const {
            return m_size;
        }
        const std::map<T, size_type>& data() const {
            return m_container;
        }
        size_type get(const T& key) {
            return m_container[key];
        }
};



NB_MODULE(sortedcounter, my_mod) {
    using DoubleSortedCounter = SortedCounter<double>;
    nb::class_<DoubleSortedCounter>(my_mod, "SortedCounter")
        .def(nb::init<>())
        .def(nb::init<std::vector<double>>())
        .def(nb::init<std::unordered_map<double, int>>())
        .def("maximum", &DoubleSortedCounter::maximum)
        .def("minimum", &DoubleSortedCounter::minimum)
        .def("__getitem__", &DoubleSortedCounter::get)
        .def("__len__", &DoubleSortedCounter::size)
        .def("__repr__", [](const DoubleSortedCounter& c){
                std::string result = "SortedCounter({";
                auto data = c.data();
                auto size = data.size();
                auto counter = 0;
                for (const auto& [key, value]: data) {
                    result += std::to_string(key) + ": " + std::to_string(value);
                    if (counter < size - 1) {
                        result += ", ";
                    }
                    ++counter;
                }
                result += "})";
                return result;
            }
        )
        .def("add", &DoubleSortedCounter::add, "key"_a, "times"_a = 1)
        .def("remove", &DoubleSortedCounter::remove, "key"_a, "times"_a = 1)
        .def("extend", &DoubleSortedCounter::extend);
}
