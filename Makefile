MCP = metil_comp -O3 -g3 --no-sep-libs -Isrc
PRG = tests/test_Http.cpp

all: cmp

cmp:
	${MCP} -ne -o listener_generator src/Celo/listener_generator.cpp

exe:
	${MCP} ${PRG}
