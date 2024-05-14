clean:
	rm -rf build

build:
	cmake -S . -B ./build -G Ninja
	cmake --build ./build
	chmod -R 777 ./build

all:
	$(MAKE) clean
	$(MAKE) build