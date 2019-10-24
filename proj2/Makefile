all:
	@(if [ ! -d build ]; then mkdir build; fi; cd build;\
	  if [ ! -d bin ]; then mkdir bin-example; fi; cmake ..; make)

clean:
	rm -rf build
