ANALYSIS="-*,cert-*,clang-analyzer-*,modernize-*,performance-*,cppcoreguidelines-*,google-*,bugprone-*,misc-*,-google-runtime-references,-cppcoreguidelines-avoid-magic-numbers,-modernize-use-trailing-return-type,-misc-redundant-expression"

SRC=$(wildcard *.cpp)
HDR=$(wildcard *.hh)

a.out : ${SRC} ${HDR}
	${CXX} -Wall -Wextra -Wshadow -O1 -g --std=c++17 -I . ${SRC} -o a.out


test: a.out
	./a.out

help:
	@echo "make [a.out]        : compile a.out"
	@echo "make test           : compile and test a.out"
	@echo "make format         : format the source code"
	@echo "make analyze        : clang-tidy code analysis"
	@echo "make fix            : clang-tidy code fix"
	@echo "make clean          : delete all generated files"

format :
	clang-format -i -style=file *.cpp *.hh

clean :
	rm -f a.out *.orig *.plist

analyze :
	clang-tidy --extra-arg="--std=c++17" -checks=${ANALYSIS} -header-filter=.* *.cpp  -- -I .

fix :
	clang-tidy --extra-arg="--std=c++17" -checks=${ANALYSIS} -header-filter=.* *.cpp -fix  -- -I .

