MCP = metil_comp -g3 --no-sep-libs -Isrc
#PRG = tests/test_SpeedyLabel.cpp
PRG = tests/test_Http.cpp
#--exec-using "valgrind --leak-check=full"

all: cmp

cmp: gen
	${MCP} -ne ${PRG}

gen:
	python src/Celo/BasicHttpRequest.py

exe: cmp
	${MCP} ${PRG}
