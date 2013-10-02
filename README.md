spintax-permutations
====================
spintax parser and permutations generator

Usage (application)
-------------------

The application shipped with the framework was designed to take input from a single source
containing spintax syntax and generate all possible permutations of the input.

It reads from stdin and writes to stdout by default, but the use can use input/output files as well
by using th `-i` and `-o` options respectively.

A typical call could look like:

    spintax-permutations -i input.txt -o output.txt

or (using input/output redirection)

    spintax-permutations < input.txt > output.txt

Usage (API)
-----------

`src/main.cpp` and `tests/test_parser.cpp` are generally straightforward examples of the usage.
A simple use case could look like this:

    #include <spintax.hpp>
    #include <iostream>
    // ...
    int main(int argc, const char* argv[]) {
        spintax::Parser parser;
        std::string line;
        // file line wlith requested input
        spintax::Structure spinStruct(parser.parse(line));
        if (spinStruct.isValid()) {
            spinStruct.writePermutations(std::cout);
        }
    }

It should dump all permutations of the provided spintax to stdout - one permutation per line.

It is also possible to dump a simple tree representation of the spintax input structure with:

    spinStruct.writeStructure(std::cout);

Using a line from `tests/data/test0.txt` as input the following structure is dumped:

    G:
      V:
        S: 'Spintaxes'
      V:
        S: 'spintaxes'
      V:
        S: 'Spintices'
      V:
        S: 'spintices'
    S: ' are '
    G:
      V:
        S: 'fun'
      V:
        S: 'easy'
      V:
        S: 'interesting'
      V:
        S: 'hilarious'
    S: '!'

where `G` symbolizes a `Group`, `V` - a `Variant` and `S` a `Simple` token.

# Build

This is a CMake-based project, so building it is as simple as running:

    cmake .
    make

Additionally you may choose to run testsuite with:

    make tests
    make check

# Internals

## Parser

The parser recognizes and connects some structures internally. Those include:

+ `Token` - base element for all spintax syntax elements
+ `Group` - this is the parsed content between `{` and `}` in the input (may contain other nested `Group`s
+ `Variant` - it is a single variant of a `Group` - e.g. in case of input `{v1|v2|v3}`, `v1`, `v2`, `v3` are variants of the same `Group`. Variant may consist of `Group`s and `Simple`s.
+ `Simple` - this is the simplest token type containing a simple string (no nested tokens)
