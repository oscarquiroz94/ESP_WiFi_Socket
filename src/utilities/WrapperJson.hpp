#pragma once

#include <string>
#include "ESPadapter.hpp"
#include "Compiletype.hpp"

// #ifdef DEPLOY
// #include <Arduino.h>
// #include <ArduinoJson.h>
// #else
// #include <string>
// #include <boost/json.hpp>
// #endif
#include <ArduinoJson.h>

namespace WrapperJson 
{
    
#ifdef DEPLOY
    using JsonDocument = JsonDocument;
    using DeserializationError = DeserializationError;
#else
    using JsonDocument = JsonDocument;
    using DeserializationError = DeserializationError;
    size_t serializeJson(const JsonDocument& doc, String &output);
    DeserializationError deserializeJson(JsonDocument &doc, const char *payload);

    // class JsonValueRef 
    // {
    //     boost::json::value& val_;
    // public:
    //     explicit JsonValueRef(boost::json::value& val) : val_(val) {}

    //     JsonValueRef operator[](const std::string& key) {
    //         if (!val_.is_object()) {
    //             val_ = boost::json::object();
    //         }
    //         return JsonValueRef(val_.as_object()[key]);
    //     }

    //     JsonValueRef operator[](const char* key) {
    //         return (*this)[std::string(key)];
    //     }

    //     // Getters
    //     std::string as_string() const { return std::string(val_.as_string()); }
    //     int as_int() const { return static_cast<int>(val_.as_int64()); }
    //     double as_double() const { return val_.as_double(); }

    //     // Conversión implícita
    //     operator std::string() const { return as_string(); }
    //     operator int() const { return as_int(); }
    //     //operator double() const { return as_double(); }

    //     // Setters para varios tipos
    //     JsonValueRef& operator=(const std::string& s) { val_ = s; return *this; }
    //     JsonValueRef& operator=(const char* s) { val_ = std::string(s); return *this; }
    //     JsonValueRef& operator=(int i) { val_ = i; return *this; }
    //     JsonValueRef& operator=(float f) { val_ = static_cast<double>(f); return *this; }
    //     JsonValueRef& operator=(double d) { val_ = d; return *this; }
    //     JsonValueRef& operator=(bool b) { val_ = b; return *this; }

    //     template<typename T> bool is() const {return false;}
    //     template<typename T> T as() const;
    // };

    // template<> inline bool JsonValueRef::is<int16_t>() const {return val_.is_int64();}
    // template<> inline std::string JsonValueRef::as<std::string>() const 
    //     {return std::string(val_.as_string());}

    // class JsonDocument 
    // {
    //     boost::json::object obj_;
    // public:
    //     JsonDocument() = default;
    //     JsonDocument(const boost::json::object& obj) : obj_(obj) {}
    //     JsonDocument& operator=(const boost::json::object& obj) { obj_ = obj; return *this; }

    //     JsonValueRef operator[](const std::string& key) {
    //         return JsonValueRef(obj_[key]);
    //     }

    //     JsonValueRef operator[](const char* key) {
    //         return (*this)[std::string(key)];
    //     }

    //     std::string serialize() const {
    //         return boost::json::serialize(obj_);
    //     }

    //     // Add this method to access the underlying object
    //     const boost::json::object& raw() const { return obj_; }

        
    // };

    // class DeserializationError 
    // {
    //     public:
    //         enum Code {
    //             Ok,
    //             EmptyInput,
    //             IncompleteInput,
    //             InvalidInput,
    //             NoMemory,
    //             TooDeep
    //         };

    //         DeserializationError() {}
    //         DeserializationError(Code c) : _code(c) {}
            
    //         Code code() const {return _code;}
    //     private:
    //         Code _code;
    // };

    // inline DeserializationError deserializeJson
    //     (JsonDocument& doc, const std::string& jsonStr) 
    // {
    //     try {
    //         doc = boost::json::parse(jsonStr).as_object();
    //         return DeserializationError::Ok;
    //     } 
    //     catch (const boost::json::system_error& e) 
    //     {
    //         if (e.code() == boost::json::error::incomplete) {
    //             return DeserializationError::IncompleteInput;
    //         } else if (e.code() == boost::json::error::input_error) {
    //             return DeserializationError::InvalidInput;
    //         } else if (e.code() == boost::json::error::too_deep) {
    //             return DeserializationError::TooDeep;
    //         } else if (e.code() == boost::json::error::object_too_large)
    //             return DeserializationError::NoMemory;
    //         else { return DeserializationError::InvalidInput;}

    //     }
    // }

    // inline void serializeJson(const JsonDocument &doc, String &output) 
    // {
    //     output = String(boost::json::serialize(doc.raw()).c_str());
    // }

#endif
};