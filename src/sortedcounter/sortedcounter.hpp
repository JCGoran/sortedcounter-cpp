#include <initializer_list>
#include <map>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <vector>

// template class that's like Python's \c dict, but with O(log n) operations
// (including min/max)
template <typename T> class SortedCounter {
  using size_type = std::uint64_t;
  using map_type = std::map<T, size_type>;

private:
  // size of the container
  size_type m_size{0};

  // implementation of the container
  map_type m_container;

  // Tag structs
  struct SequenceTag {};
  struct MapTag {};

  // Helper to identify sequence containers
  template <typename Container>
  using is_sequence_container =
      std::disjunction<std::is_same<Container, std::vector<T>>,
                       std::is_same<Container, std::set<T>>,
                       std::is_same<Container, std::initializer_list<T>>>;

  // Helper to identify map-like containers
  template <typename MapType>
  using is_map_container =
      std::conjunction<std::is_same<typename MapType::mapped_type, int>>;

  // template for STL array-like types
  template <typename Container>
  explicit SortedCounter(const Container &items, SequenceTag) {
    // does not need to be super-optimized
    for (const auto &item : items) {
      if (!m_container.count(item)) {
        m_container[item] = 1;
      } else {
        m_container[item] += 1;
      }
      ++m_size;
    }
  }

  // template for mapped types
  template <typename MapType>
  explicit SortedCounter(const MapType &items, MapTag) {
    for (const auto &[key, value] : items) {
      if (value <= 0) {
        throw std::invalid_argument("The count must be > 0.");
      }
      m_container[key] = value;
      m_size += value;
    }
  }

public:
  // default ctor
  SortedCounter() = default;

  // default dtor
  ~SortedCounter() = default;

  // Constructor for sequence types
  template <typename Container>
  explicit SortedCounter(
      const Container &items,
      std::enable_if_t<is_sequence_container<Container>::value, int> = 0)
      : SortedCounter(items, SequenceTag{}) {}

  // Constructor for map types
  template <typename MapType>
  explicit SortedCounter(
      const MapType &items,
      std::enable_if_t<is_map_container<MapType>::value, int> = 0)
      : SortedCounter(items, MapTag{}) {}

  // add a key to the container (possibly multiple times)
  void add(const T key, const std::int64_t times = 1) {
    if (times <= 0) {
      throw std::invalid_argument("Cannot add <= 0 items to container");
    }
    const auto present = m_container.count(key);
    if (!present) {
      m_container[key] = times;
    } else {
      m_container[key] += times;
    }
    m_size += times;
  }

  // remove a key from the container (possibly multiple times)
  void remove(const T key, const std::int64_t times = 1) {
    if (times <= 0) {
      throw std::invalid_argument("Cannot remove <= 0 items to container");
    }
    const auto present = m_container.count(key);
    if (!present) {
      throw std::invalid_argument("Cannot remove non-existing value");
    } else {
      if (static_cast<std::int64_t>(m_container[key]) - times <= 0) {
        const auto remaining = m_container[key];
        m_container.erase(key);
        m_size -= remaining;
      } else {
        m_container[key] -= times;
        m_size -= times;
      }
    }
  }

  // template for adding new items to the container
  template <typename Container,
            typename = std::enable_if_t<
                std::is_same_v<typename Container::value_type,
                               typename std::vector<T>::value_type> ||
                std::is_same_v<typename Container::value_type,
                               typename std::initializer_list<T>::value_type>>>
  void extend(const Container &items) {
    for (const auto &item : items) {
      const auto present = m_container.count(item);
      if (!present) {
        m_container[item] = 1;
      } else {
        m_container[item] += 1;
      }
      ++m_size;
    }
  }

  // return the maximum element in the container
  T maximum() {
    if (m_container.empty()) {
      throw std::out_of_range("Container is empty!");
    }
    return (*m_container.rbegin()).first;
  }

  // return the minimum element in the container
  T minimum() {
    if (m_container.empty()) {
      throw std::out_of_range("Container is empty!");
    }
    return (*m_container.begin()).first;
  }

  // return the size of the container
  size_type size() const noexcept { return m_size; }

  // return the underlying data
  const map_type &data() const { return m_container; }

  // return the number of occurrences of element \c key
  size_type get(const T &key) { return m_container[key]; }
};
