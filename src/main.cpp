//
// Copyright (c) 2013 Dariusz Gadomski <dgadomski@gmail.com>
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//  * Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation and/or
// other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#include "spintax.hpp"

#include <boost/program_options.hpp>

#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <stack>
#include <vector>

using namespace spintax;

namespace po = boost::program_options;

int main(int argc, const char *argv[]) {
    po::options_description desc("Generator options");
    desc.add_options()
        ("help,h", "print this help message")
        ("input-file,i", po::value<std::string>(), "input file name (stdin is used by default)")
        ("output-file,o", po::value<std::string>(), "output file name (stdout is used by default)")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    std::istream *input = &std::cin;
    std::ostream *output = &std::cout;
    bool freeInput = false;
    bool freeOutput = false;

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    } else {
        if (vm.count("input-file")) {
            input = new std::ifstream(vm["input-file"].as<std::string>());
            freeInput = true;
        }
        if (vm.count("output-file")) {
            output = new std::ofstream(vm["output-file"].as<std::string>());
            freeOutput = true;
        }
    }

    std::string lines;
    std::string line;
    *input >> std::noskipws;
    while (std::getline(*input, line)) {
        lines += line + "\n";
    }

    Parser parser;
    parser.parse(lines).writePermutations(*output);

    if (freeInput) {
        delete input;
    }

    if (freeOutput) {
        delete output;
    }

    return 0;
}
