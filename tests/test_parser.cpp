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

#include <boost/test/parameterized_test.hpp>
#include <boost/test/included/unit_test.hpp>

using namespace spintax;
using namespace boost::unit_test;

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

void test_data(const std::pair<std::string, size_t>& data) {
    size_t result(test_parser(data.first));
    BOOST_CHECK_EQUAL(result, data.second);
}

test_suite *init_unit_test_suite(int argc, char *argv[]) {
    test_suite *ts = BOOST_TEST_SUITE("parser");
    std::vector<std::pair<std::string, size_t> > params;
    for (int i=1; i<argc; i+=2) {
        std::string fname(argv[i]);
        std::istringstream iss(argv[i+1]);
        size_t expectedLength(0);
        iss >> expectedLength;
        params.push_back(std::pair<std::string, size_t>(fname, expectedLength));

    }
    ts->add(BOOST_PARAM_TEST_CASE(&test_data, params.begin(), params.end()));
    return ts;
}

