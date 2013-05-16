.DEFAULT: engine
.PHONY: engine

engine: 
	rm -rf build
	mkdir build
	(cd build && cmake .. && make)
	cp build/DiaballikBot engine

