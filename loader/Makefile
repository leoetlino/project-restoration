export SHELL:=/bin/bash
export SHELLOPTS:=$(if $(SHELLOPTS),$(SHELLOPTS):)pipefail:errexit

.PHONY: build

.ONESHELL:
build:
		mkdir build || true
		cd build
		cmake .. -DCMAKE_TOOLCHAIN_FILE=../../DevkitArm3DS.cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCODEADDR=$(CODEADDR) -DDATAADDR=$(DATAADDR)
		make
		cp *.sym *.bin ../

clean:
		rm -rf build
