nanobind_add_module(sortedcounter impl.cpp)
install(TARGETS sortedcounter COMPONENT Runtime DESTINATION sortedcounter)
