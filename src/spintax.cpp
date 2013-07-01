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

namespace spintax
{

inline Token::~Token()
{
}

unsigned Token::numVariants() const {
    return 0;
}

Variant::~Variant() {
    for(auto t : m_tokens) {
        t.reset();
    }
}

void Variant::addToken(const std::shared_ptr<Token>& token) {
    m_tokens.push_back(token);
}

std::string Variant::str() const {
    std::string result;
    for (auto t : m_tokens) {
        result += t->str();
    }
    return result;
}

std::string Variant::structureAsStr(const std::string& prefix) const {
    std::string result(prefix + "V:\n");
    for (auto t : m_tokens) {
        result += t->structureAsStr(prefix + "  ");
    }
    return result;
}

unsigned Variant::numVariants() const {
    return m_tokens.size();
}

std::shared_ptr<Token> Variant::token(int index) {
    return m_tokens[index];
}

const TokVec& Variant::tokens() const {
    return m_tokens;
}

Group::~Group() {
    for (auto v : m_variants) {
        v.reset();
    }
}

void Group::addVariant(const std::shared_ptr<Variant>& variant) {
    m_variants.push_back(variant);
}

std::shared_ptr<Variant> Group::lastVariant() {
    return m_variants.back();
}

std::string Group::str() const {
    std::string result;
    for (auto v : m_variants) {
        result += v->str();
    }
    return result;
}

std::string Group::structureAsStr(const std::string& prefix) const {
    std::string result(prefix + "G:\n");
    for (auto v : m_variants) {
        result += v->structureAsStr(prefix + "  ");
    }
    return result;
}

unsigned Group::numVariants() const {
    return m_variants.size();
}

std::shared_ptr<Variant> Group::variant(int index) {
    return m_variants[index];
}

const VarVec& Group::variants() const {
    return m_variants;
}

Simple::Simple(const std::string& str)
    :m_str(str)
{
}

std::string Simple::str() const {
    return m_str;
}

std::string Simple::structureAsStr(const std::string& prefix) const {
    return prefix + "S: '" + m_str + "'\n";
}


unsigned Simple::numVariants() const {
    return 1;
}

Structure::~Structure() {
    for (auto t : m_topLevelTokens) {
        t.reset();
    }
}

void Structure::writeStructure(std::ostream& out) const {
    for (auto t : m_topLevelTokens) {
        out << t->structureAsStr("");
    }
}

void Structure::addTopLevel(const std::shared_ptr<Token>& token) {
    m_topLevelTokens.push_back(token);
}

void Structure::clear() {
    m_topLevelTokens.clear();
}

void Structure::writePermutations(std::ostream& out) const {
    StrVec res;
    res.push_back(std::string());
    writePermutations(res, m_topLevelTokens);
    std::copy(res.begin(), res.end(), std::ostream_iterator<std::string>(out, "\n"));
}

void Structure::writePermutations(StrVec& res, const TokVec& tokens) const {
    for (auto token : tokens) {
        if (token->numVariants() > 1) {
            std::shared_ptr<Group> group(std::dynamic_pointer_cast<Group>(token));
            if (group) {
                StrVec perGroup;
                for (auto variant : group->variants()) {
                    StrVec perVariant;
                    perVariant.push_back(std::string());
                    writePermutations(perVariant, variant->tokens());
                    std::copy(perVariant.begin(), perVariant.end(), std::back_inserter(perGroup));
                }
                mix(res, perGroup);
            }
        } else {
            addToAll(res, token->str());
        }
    }
}

void Structure::mix(StrVec& res, StrVec& variants) const {
    const auto lengthRes(res.size());
    const auto lengthVariants(variants.size());
    for(size_t i=0; i<lengthRes; ++i) {
        for (size_t j=0; j<lengthVariants; ++j) {
            res.push_back(res[i] + variants[j]);
        }
    }
    res.erase(res.begin(), res.begin() + lengthRes);
}

void Structure::addToAll(StrVec& res, const std::string& str) const {
    for (auto it=res.begin(); it != res.end(); ++it) {
        *it += str;
    }
}

ConsoleErrorHandler Parser::defaultErrorHandler;

Parser::Parser()
    :m_errorHandler(defaultErrorHandler)
{
}

Parser::Parser(ErrorHandler& errorHandler)
    :m_errorHandler(errorHandler)
{
}

Parser::~Parser() {
}

void Parser::handleSimple(const std::string& simpleText) {
    std::shared_ptr<Simple> simple(nullptr);

    if (!simpleText.empty()) {
        simple = std::shared_ptr<Simple>(new Simple(simpleText));
    }

    if (simple) {
        if (m_groups.empty()) {
            m_structure.addTopLevel(simple);
        }

        if (!m_groups.empty()) {
            m_groups.top()->lastVariant()->addToken(simple);
        }
    }
}

void Parser::handleCheckTopLevel(std::shared_ptr<Token> token) {
    if (m_groups.empty()) {
        m_structure.addTopLevel(token);
    }
}

const Structure& Parser::parse(const std::string& input) {
    m_structure.clear();
    bool error(false);
    std::string tmp;

    const size_t inputLength(input.length());
    for (size_t i=0; i<inputLength; ++i) {
        char c = input[i];
        if (c == GROUP_START) {
            std::shared_ptr<Group> group(new Group());

            handleSimple(tmp);

            handleCheckTopLevel(group);

            group->addVariant(std::shared_ptr<Variant>(new Variant));

            if (!m_groups.empty()) {
                m_groups.top()->lastVariant()->addToken(group);
            }

            m_groups.push(group);
            tmp.clear();
        } else if (c == GROUP_END) {
            handleSimple(tmp);

            if (m_groups.empty()) {
                error = true;
                m_errorHandler.onError(ErrorHandler::BracketsMismatch,
                        "Closing group that was not opened. Brackets mismatch at position " +
                        std::to_string(i) + ".");
                break;
            }

            if (m_groups.top()->numVariants() < 2) {
                m_errorHandler.onWarning("Warning: Group with less than 2 variants ends at position " + 
                std::to_string(i) + ".");
            }

            m_groups.pop();

            tmp.clear();
        } else if (c == VARIANT_SEP) {
            handleSimple(tmp);

            m_groups.top()->addVariant(std::shared_ptr<Variant>(new Variant));

            tmp.clear();
        } else {
            tmp += c;
        }
    }

    if (!tmp.empty()) {
        m_structure.addTopLevel(std::shared_ptr<Simple>(new Simple(tmp)));
    }

    if (!m_groups.empty()) {
        error = true;
        m_errorHandler.onError(ErrorHandler::BracketsMismatch, "Brackets mismatch: " +
                std::to_string(m_groups.size()) + " group(s) have not been closed.");
    }

    if (error) {
        std::cerr << "Finishing due to errors encoutered." << std::endl;
        m_structure.clear();
        while(!m_groups.empty())
            m_groups.pop();
    }

    return m_structure;
}

}
