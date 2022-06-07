#include <variant>
#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <memory>
#include <string_view>
#include <exception>
#include <any>

#ifndef JPARSER_H
#define JPARSER_H

namespace jsonparser
{
    using JSNull = std::nullptr_t;
    using JSBool = bool;
    using JSNumber = double;
    using JSString = std::string;

    // We use some template magic so JSArray and JSObject can contain themselves
    // https://stackoverflow.com/a/53504373
    template<class T>
    using Json_t = std::variant<JSNull, JSBool, JSNumber, JSString, std::vector<T>, std::unordered_map<std::string, T>>;

    template<template<class> class K>
    struct Fix : K<Fix<K>>
    {
        using K<Fix<K>>::K;
    };
    
    using Json = Fix<Json_t>;
    using JSArray = std::vector<Json>;
    using JSObject = std::unordered_map<std::string, Json>;

    Json parse(const std::string_view json);
    std::string serialize(const Json &value);
 
    class ParsingError : public std::exception
    {
        public:
            ParsingError(const std::string &msg) : msg(msg) {}
            const char *what() const noexcept override { return msg.c_str(); }
        private:
            // We need to store the message because the default implementation of what() is not virtua
            std::string msg;
    };   
   
    
    class Parser
    {
    public:
        Parser(const std::string_view j)
        {
            end = j.size();
            json = j;
        }
        Json parse()
        {
            try
            {
                return parse_json();
            }
            catch (const std::out_of_range &e)
            {
                throw prepare_exception(ExceptionMessage::UNEXPECTED_END_OF_JSON);
            }
        }

    private:
        std::string_view json;
        size_t position = 0;
        size_t end;

        // Returns and consumes the next character in the string
        std::string_view consume()
        {
            return json.substr(position++, 1);
        }

        std::string_view current()
        {
            return json.substr(position, 1);
        }

        bool is_at_end()
        {
            return position >= end;
        }

        bool is_current_digit()
        {
            return std::isdigit(current()[0]);
        }

        std::string consume_digits()
        {
            std::string result;
            while (!is_at_end() && is_current_digit())
            {
                result += consume();
            }
            return result;
        }

        bool is_current_matching(const char c)
        {
            return !is_at_end() && current()[0] == c;
        }

        bool is_current_in(const std::string_view s)
        {
            if (is_at_end() || (s.find(current()) == std::string_view::npos))
            {
                return false;
            }
            return true;
        }

        void consume_whitespace()
        {
            while (is_current_in(" \n\t\r"))
            {
                consume();
            }
        };

        void consume_keyword(const std::string_view keyword);
        JSNull parse_jsnull();
        JSBool parse_jsbool();
        JSNumber parse_jsnumber();
        JSString parse_jsstring();
        JSArray parse_jsarray();
        JSObject parse_jsobject();
        Json parse_json();
        enum ExceptionMessage
        {
            UNEXPECTED_END_OF_JSON,
            UNEXPECTED_TOKEN,
            PRECISION_LOSS,
            UNEXPECTED_ESCAPED_CHARACTER,
        };

        ParsingError prepare_exception(ExceptionMessage message, std::string info = "");
    };
}

#endif