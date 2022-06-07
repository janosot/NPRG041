#include "jparser.hpp"
#include <codecvt>
#include <locale>
#include <sstream>

namespace jsonparser
{
    // Recursive descent parsing
    Json parse(const std::string_view json)
    {
        Parser parser(json);
        return parser.parse();
    }

    void Parser::consume_keyword(const std::string_view keyword)
    {   
        // iterate over keyword chars
        for (auto c : keyword)
        {
            if (is_current_matching(c))
            {
                consume();
            }
            else
            {
                throw prepare_exception(ExceptionMessage::UNEXPECTED_TOKEN);
            }
        }
    }

    JSArray Parser::parse_jsarray()
    {
        // Collection of values
        // Starts with left bracket 
        JSArray array;
        consume(); // [
        consume_whitespace();
        if (is_current_matching(']'))
        {
            consume();
            return array;
        }

        while (true)
        {
            auto value = parse_json();
            array.push_back(value);
            // Separated with comma 
            if (is_current_matching(','))
            {
                consume();
            }
            // Ends with right bracket
            else if (is_current_matching(']'))
            {
                consume();
                return array;
            }
            else
            {
                throw prepare_exception(ExceptionMessage::UNEXPECTED_END_OF_JSON);
            }
        }
    }

    JSObject Parser::parse_jsobject()
    {
        JSObject object;
        consume(); // {
        consume_whitespace();
        if (is_current_matching('}'))
        {
            consume(); // }
            return object;
        }

        while (true)
        {
            consume_whitespace();
            auto key = parse_jsstring();
            consume_whitespace();
            if (!is_current_matching(':'))
            {
                throw prepare_exception(ExceptionMessage::UNEXPECTED_END_OF_JSON);
            }
            consume(); // :
            auto value = parse_json();
            object[key] = value;
            if (is_current_matching(','))
            {
                consume(); // ,
            }
            else if (is_current_matching('}'))
            {
                consume(); // }
                return object;
            }
            else
            {
                throw prepare_exception(ExceptionMessage::UNEXPECTED_END_OF_JSON);
            }
        }
    }

    JSString Parser::parse_jsstring()
    {
        auto consume_four_hex_digits = [&]() -> std::string
        {
            std::string retval;
            for (int i = 0; i < 4; i++)
            {
                if (is_current_in("0123456789abcdefABCDEF"))
                {
                    retval += consume();
                }
                else
                {
                    throw prepare_exception(ExceptionMessage::UNEXPECTED_END_OF_JSON);
                }
            }
            return retval;
        };

        consume(); // Opening quote
        std::string str;

        while (true)
        {
            if (is_current_matching('"'))
            {
                consume(); // Closing quote
                return JSString(str);
            }
            // Escape sequences
            else if (is_current_matching('\\'))
            {
                consume(); // backslash
                if (is_current_in("\"\\/bfnrt"))
                {
                    std::unordered_map<std::string_view, char> escape_sequences = {
                        {"b", '\b'},
                        {"f", '\f'},
                        {"n", '\n'},
                        {"r", '\r'},
                        {"t", '\t'},
                        {"\\", '\\'},
                        {"/", '/'}};
                    str += escape_sequences[consume()];
                }
                else if (is_current_matching('u'))
                {
                    // Unicode escape sequence
                    consume(); // u
                    auto codepoint_str = consume_four_hex_digits();
                    auto codepoint = std::stoi(codepoint_str, nullptr, 16);
                    // Deprecated in C++17 but alternatives require external dependencies,
                    // as there does not seem to be a standard way to do this.
                    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
                    std::string utf8 = convert.to_bytes(codepoint);
                    str += utf8;
                }
                else
                {
                    throw prepare_exception(ExceptionMessage::UNEXPECTED_ESCAPED_CHARACTER, std::string(consume()));
                }
            }
            else
            {
                str += consume();
            }
        }
    }

    JSNumber Parser::parse_jsnumber()
    {
        auto try_convert_to_number = [&](const std::string& str) -> JSNumber
        {
           try
            {
                return std::stod(str);
            }
            catch (const std::out_of_range &e)
            {
                throw prepare_exception(ExceptionMessage::PRECISION_LOSS);
            }
        };	
        std::string num_str;
        // First, attempt to parse the possible negative sign
        if (is_current_matching('-'))
        {
            num_str += consume();
        }

        // Then, check if the first digit is zero, as it is a special case
        if (is_current_matching('0'))
        {
            num_str += consume();
            // Only fraction, exponent or end of number is allowed now
            if (is_current_in(".eE") ||
                !is_current_digit() // the end of the number
            )
            {
                return try_convert_to_number(num_str);
            }
            else
            {
                throw prepare_exception(ExceptionMessage::UNEXPECTED_END_OF_JSON);
            }
        }

        num_str += consume_digits();

        // Fraction
        if (is_current_matching('.'))
        {
            num_str += consume();
            // Ensure at least one digit is present after the decimal point
            if (!is_current_digit())
            {
                throw prepare_exception(ExceptionMessage::UNEXPECTED_END_OF_JSON);
            }
            num_str += consume_digits();
        }

        // Exponent
        if (is_current_in("eE"))
        {
            num_str += consume();
            // Optional sign
            if (is_current_in("+-"))
            {
                num_str += consume();
            }
            // Enforce at least one digit is present after the exponent
            if (!is_current_digit())
            {
                throw prepare_exception(ExceptionMessage::UNEXPECTED_END_OF_JSON);
            }
            num_str += consume_digits();
        }

        // Check if the parsed number can be converted to a double without loss of information
        return try_convert_to_number(num_str);
    }

    Json Parser::parse_json()
    {
        Json retval;
        consume_whitespace();
        // We observe each of six possible JSON values is
        // uniquely identified by it's first character
        // https://datatracker.ietf.org/doc/html/rfc7159#section-3
        // in other words, JSON's grammar is ambiguous, LL(1) and requires only
        // a single character lookahead
        if (is_current_matching('n')) {
            consume_keyword("null");
            retval = JSNull();
        }
        else if (is_current_matching('t')) {
            consume_keyword("true");
            retval = true;
        }
        else if (is_current_matching('f')) {
            consume_keyword("false");
            retval = false;
        }
        else if (is_current_matching('"'))
            retval = parse_jsstring();
        else if (is_current_matching('['))
            retval = parse_jsarray();
        else if (is_current_matching('{'))
            retval = parse_jsobject();
        else if (is_current_in("-0123456789"))
            retval = parse_jsnumber();
        else
            throw prepare_exception(ExceptionMessage::UNEXPECTED_TOKEN);

        consume_whitespace();
        return retval;
    };

    std::string serialize(const Json &value)
    {
        struct Visitor
        {
            std::string operator()(const JSNull &) { return "null"; }
            std::string operator()(const JSBool &b) { return b ? "true" : "false"; }
            std::string operator()(const JSNumber &n)
            {
                std::stringstream ss;
                ss << n;
                return ss.str();
            }
            std::string operator()(const JSString &str) { return std::string("\"") + str + "\""; }
            std::string operator()(const JSObject &obj)
            {
                std::string retval = "{";
                for (auto it = obj.begin(); it != obj.end(); ++it)
                {
                    retval += serialize(it->first) + ": " + serialize(it->second);
                    if (std::next(it) != obj.end())
                    {
                        retval += ", ";
                    }
                }
                retval += "}";
                return retval;
            }
            std::string operator()(const JSArray &arr)
            {
                std::string retval = "[";
                for (auto it = arr.begin(); it != arr.end(); ++it)
                {
                    retval += serialize(*it);
                    if (std::next(it) != arr.end())
                    {
                        retval += ", ";
                    }
                }
                retval += "]";
                return retval;
            }
        };

        return std::visit(Visitor(), value);
    }

    ParsingError Parser::prepare_exception(ExceptionMessage message, std::string info)
    {
        // Unfortunately, https://en.cppreference.com/w/cpp/utility/format is not implemented in some compilers
        // Notably, Apple Clang and GCC
        std::string err;
        switch (message)
        {
        case ExceptionMessage::UNEXPECTED_END_OF_JSON:
            return ParsingError("Unexpected end of JSON input");
        case ExceptionMessage::PRECISION_LOSS:
            err = "Number " + info + " around position " + std::to_string(position) + " cannot be represented as a double";
            return ParsingError(err);
        case ExceptionMessage::UNEXPECTED_TOKEN:
            err = "Unexpected token " + info + " at position " + std::to_string(position);
            return ParsingError(err);
        case ExceptionMessage::UNEXPECTED_ESCAPED_CHARACTER:
            err = "Unexpected escaped character " + info + " at position " + std::to_string(position);
            return ParsingError(err);
        }
    }
}
