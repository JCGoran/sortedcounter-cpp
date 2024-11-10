nanobind_add_module(sortedcounter STABLE_ABI pywrapper.cpp)
install(TARGETS sortedcounter LIBRARY DESTINATION sortedcounter)
