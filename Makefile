MCP = metil_comp -O3 -g3 --no-sep-libs -Isrc
PRG = tests/test_Http.cpp
INSTALL_DIR = `pwd`

all: cmp

cmp: inst_dir Sipe
	make -C Sipe
	${MCP} -ne -o listener_generator src/Celo/listener_generator.cpp

Sipe:
	git clone git@gitosis.lmt.ens-cachan.fr:Sipe

exe:
	${MCP} ${PRG}

inst_dir:
	export D="#define INSTALL_DIR \"${INSTALL_DIR}\""; grep "$$D" src/Celo/InstallDir.h || echo "$$D" > src/Celo/InstallDir.h

.PHONY: inst_dir

