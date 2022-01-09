build: build_if

build_if:
	cd compiler && $(MAKE) ANTLR4_BINDIR=/usr/bin LIBDIR=/shares/public/tp/ANTLR4-CPP/lib INCDIR=/shares/public/tp/ANTLR4-CPP/antlr4-runtime

build_perso:
	cd compiler && $(MAKE) ANTLR4_BINDIR=/usr/bin ANTLR4_INCDIR=/usr/include/antlr4-runtime ANTLR4_LIBDIR=/usr/lib/x86_64-linux-gnu/

test:
	cd tests && ./test_if.sh

tests: test

clean:
	cd compiler && $(MAKE) clean
	@rm -rf tests/pld-test-output
	@rm -rf compiler/ifcc
