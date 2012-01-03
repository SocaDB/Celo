MCP = metil_comp --exec-using "valgrind --leak-check=full" -g3 --no-sep-libs -Isrc
PRG = tests/test_Http.cpp
#--exec-using "valgrind --leak-check=full"

all: gen
	${MCP} -ne ${PRG}

gen:
	python src/Celo/HttpRequest.py

exe:
	${MCP} ${PRG}
