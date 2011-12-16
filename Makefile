 
all:
	metil_comp --no-sep-libs -Isrc -ne tests/test_Timer.cpp

exe:
	metil_comp --no-sep-libs -Isrc tests/test_Timer.cpp
