#pragma once
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <iostream>
#include <iomanip>
#include <sstream>

class PasswordService
{
private: 
    static constexpr int SALT_SIZE = 32;    
    static constexpr int KEY_LEN = 32;
    static constexpr int ITERATIONS = 100000;
public:
    static std::string hashPassword(const std::string& password);
    static bool verifyPassword(const std::string& password, const std::string& storedHash);
    static std::string bytesToHex(const unsigned char* data, size_t len);
    static bool hexToBytes(const std::string& hex, unsigned char* out, size_t outLen);
};