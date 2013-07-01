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

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <spintax.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestParser
#include <boost/test/unit_test.hpp>

using namespace spintax;

size_t test_parser(std::string fname) {
    std::ifstream input(fname);
    std::string line;
    std::getline(input, line);
    std::ostringstream ostr;
    Parser parser;
    parser.parse(line).writePermutations(ostr);
    std::string output(ostr.str());
    return std::count(output.begin(), output.end(), '\n');
}

BOOST_AUTO_TEST_CASE( full_parser )
{
    std::vector<std::string> result;

    std::vector<std::string> inputFilenames = { "test0.txt", "test1.txt", "test2.txt" };
    std::vector<size_t> expectedLenghts =     {          16,          80,      40600  };

    assert(expectedLenghts.size() == inputFilenames.size());

    for (size_t i=0; i<inputFilenames.size(); ++i) {
        BOOST_CHECK_EQUAL(test_parser(inputFilenames[i]), expectedLenghts[i]);
    }
}
