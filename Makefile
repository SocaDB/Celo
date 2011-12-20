MCP = metil_comp --no-sep-libs -Isrc
PRG = tests/test_Http.cpp

all:
	${MCP} -ne ${PRG}

exe:
	${MCP} ${PRG}
