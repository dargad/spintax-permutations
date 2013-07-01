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

#ifndef SPINTAX_H
#define SPINTAX_H

#include "errors.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <stack>
#include <string>
#include <vector>

namespace spintax {

class Token;
class Variant;

typedef std::vector<std::string>                StrVec;
typedef std::vector<std::shared_ptr<Token>>     TokVec;
typedef std::vector<std::shared_ptr<Variant>>   VarVec;

//! Basic spintax entity - token.
/*!
 * It is a base class for each spintax framework classes.
 */
class Token {
public:
    virtual ~Token() = 0;

    //! Returns string content of this token.
    virtual std::string str() const = 0;
    //! Returns string representing structure of this token.
    virtual std::string structureAsStr(const std::string& prefix) const = 0;

    //! Returns number of child entities of this token.
    virtual unsigned numVariants() const;
};

//! Single spintax group variant.
/*!
 * Represents a single variant ("fork") of a spintax group.
 * Each group consists of 2 (1 is considered an error) or more variants.
 * Contains a group of tokens. May consist of groups containing more variants.
 * \sa Token, Group
 */
class Variant : public Token {
    TokVec m_tokens;

public:
    ~Variant();

    std::string str() const;
    std::string structureAsStr(const std::string& prefix) const;

    unsigned numVariants() const;
    //! Adds a child token to this variant.
    void addToken(const std::shared_ptr<Token>& token);
    //! Returns a child token of this variant at index.
    std::shared_ptr<Token> token(int index);
    //! Returns all child tokens of this variant.
    const TokVec& tokens() const;
};

//! Spintax group.
/*!
 * Represents a group - set of spintax alternatives or Variants.
 * \sa Variant
 */
class Group : public Token {
    VarVec m_variants;

public:
    ~Group();

    std::string str() const;
    std::string structureAsStr(const std::string& prefix) const;

    std::shared_ptr<Variant> lastVariant();
    unsigned numVariants() const;
    //! Adds a variant to this group (inserts at the end).
    void addVariant(const std::shared_ptr<Variant>& variant);
    //! Get a variant of this group at index.
    std::shared_ptr<Variant> variant(int index);
    //! Returns all variants of this group.
    const VarVec& variants() const;

};

//! Simple token.
/*!
 * Represents the simplest spintax entity - a string.
 * \sa Token
 */
class Simple : public Token {
    std::string m_str;

public:
    explicit Simple(const std::string& str);

    std::string str() const;
    std::string structureAsStr(const std::string& prefix) const;

    unsigned numVariants() const;
};

//! Spintax structure.
/*!
 * Contains parsed information on the spintax structure,
 * grouped into a set of top-level tokens which may contain
 * more complex structure internally.
 * \sa Parser
 */
class Structure {
    TokVec m_topLevelTokens;

    //! Mixes the output vector with variants generated.
    /*!
     * Basically it takes each element of res and creates a variants.size()
     * copies of this element and appends subsequent elements from
     * variants to each of the copies.
     */
    virtual void mix(StrVec& res, StrVec& variants) const;
    //! Adds str at the end of each elements of res.
    virtual void addToAll(StrVec& res, const std::string& str) const;

protected:
    //! Write all permutations of provided tokens to res.
    void writePermutations(StrVec& res, const TokVec& tokens) const;

public:
    virtual ~Structure();
    //! Write structure to the provided output stream.
    void writeStructure(std::ostream& out) const;

    //! Write all permutations of this structure to the provided output stream.
    void writePermutations(std::ostream& out=std::cout) const;

    //! Adds a top level token to this structure.
    void addTopLevel(const std::shared_ptr<Token>& token);
    //! Removes all tokens from this structure.
    void clear();
};

//! Spintax parser.
/*!
 * Performs parsing of the spintax string.
 * Prepares a structure.
 *
 * This object is not thread-safe (as it uses internal structures altered
 * during parsing), it is suggested to use a separate instances of Parser
 * in each thread.
 * \sa Structure
 */
class Parser {
    static const char GROUP_START = '{';
    static const char GROUP_END   = '}';
    static const char VARIANT_SEP = '|';

    static ConsoleErrorHandler defaultErrorHandler;

    std::stack<std::shared_ptr<Group>>  m_groups;
    Structure                           m_structure;
    ErrorHandler&                       m_errorHandler;

protected:
    //! Handle encountered simple entity.
    virtual void handleSimple(const std::string& simpleText);
    //! Check whether encoutered token should be places as a top-level entity.
    virtual void handleCheckTopLevel(std::shared_ptr<Token> token);

public:
    Parser();
    Parser(ErrorHandler& errorHandler);
    virtual ~Parser();

    //! Converts the input string to spintax Structure.
    const Structure& parse(const std::string& input);
};

}

#endif /* SPINTAX_H */
