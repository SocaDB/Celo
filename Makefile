MCP = metil_comp -O3 -g3 --no-sep-libs -Isrc
#PRG = tests/test_SpeedyLabel.cpp
PRG = tests/test_HttpRequest.cpp
#--exec-using "valgrind --leak-check=full"

all: gen
	${MCP} -ne ${PRG}

gen:
	python src/Celo/HttpRequest.py

exe:
	${MCP} ${PRG}
