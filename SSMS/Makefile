all: clean run_cmake
	cd build; make ; make test

.PHONY: clean run_cmake app test

clean:
	rm -rf build

run_cmake:
	mkdir build; cd build; cmake ..;

app: clean run_cmake
	cd build; make;

test: clean run_cmake
	cd build; make test;
