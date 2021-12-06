SRCS=$(wildcard */*.cpp)
OBJS=$(SRC:%.cpp=%.o)
TESTS=$(OBJS:%.o=%)

test: build/test.out
	build/test.out --gtest_shuffle

build/test.out: include/*.h lib/*.cpp test/*.cpp
	mkdir -p build
	g++ --std=c++1z test/AlgebraicTest.cpp lib/AlgebraicReal.cpp -lgtest_main -lgtest -lpthread -I include -o build/test.out 

%Test: build/test/%Test.o
	mkdir -p build/bin
	g++ $< -o build/bin/$@ build/lib/AlgebraicReal.o -lgtest_main -lgtest -lpthread
	build/bin/$@ --gtest_shuffle

build/%.o: %.cpp
	mkdir -p build/test
	g++ --std=c++1z -c $< lib/ -I include -o $@

clean:
	rm -r build
