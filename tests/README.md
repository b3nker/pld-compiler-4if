# C Compiler test framework

This directory contains scripts to test your compiler along with
a collection of C language source codes that can be tested.

## Prerequisites

Docker must be installed in order to execute these scripts. The compiler has
to be present in the directory `../compiler/ifcc`. Compiler options and location
can be customized in the `pld-wrapper.sh` script.

## Files

`test.sh` is the base file to launch. It will call the appropriate Docker
image and execute the `pld-test.py` script. Results are put in a new directory
`pld-test-outputs`.

The tests are in the `tests` subfolder.

# Tests cases differing from GCC

Among the tests, some of them are considered failures because the results is
different than gcc's result whereas it is the intended implementation of our
compiler. These tests are :
- DeclarationMissingVar: gcc accepts the code `int ;` whereas we deem it an
error.
- ...
