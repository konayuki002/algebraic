test: build/test.out
	build/test.out --gtest_shuffle

build/test.out: include/*.h lib/*.cpp test/*.cpp
	mkdir -p build
	g++ --std=c++1z test/AlgebraicTest.cpp lib/AlgebraicReal.cpp -lgtest_main -lgtest -lpthread -I include -o build/test.out 

clean:
	rm -r build
