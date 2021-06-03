# OMPT 5.0+ Edge Cases

This repository contains a few simple OpenMP 5.0+ program examples that should indicate to inconsistencies inside
LLVM OMPT 5.0+ implementation.

## Prerequisites
This repository assumes that the user have installed C compiler (GCC or Clang),
which executable path must be set as a value of `CC` variable inside provided Makefile.
It is also assumed that the OpenMP runtime from llvm-project repository is installed.
The `OLIB` variable, present in the Makefile, should point to the installation directory of the runtime.

## Running Test Examples
In order to run the test examples, execute the following command:
```shell script
make
```

