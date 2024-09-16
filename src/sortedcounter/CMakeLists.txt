nanobind_add_module(sortedcounter STABLE_ABI impl.cpp)
install(TARGETS sortedcounter LIBRARY DESTINATION sortedcounter)
