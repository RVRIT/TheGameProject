#include "PasswordService.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdexcept>
#include <sstream>
#include <iomanip>
std::string PasswordService::bytesToHex(const unsigned char* data, size_t len) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < len; ++i) {
        ss << std::setw(2) << static_cast<int>(data[i]);
    }
    return ss.str();
}
bool PasswordService::hexToBytes(const std::string& hex, unsigned char* out, size_t outLen) {
    if (hex.length() != outLen * 2) return false;
    for (size_t i = 0; i < outLen; ++i) {
        std::string byteStr = hex.substr(i * 2, 2);
        out[i] = static_cast<unsigned char>(std::stoi(byteStr, nullptr, 16));
    }
    return true;
}
std::string PasswordService::hashPassword(const std::string& password) {
    unsigned char salt[SALT_SIZE];
    unsigned char derivedKey[KEY_LEN];
    if (!RAND_bytes(salt, SALT_SIZE)) {
        throw std::runtime_error("Failed to generate salt");
    }

    if (!PKCS5_PBKDF2_HMAC(
        password.c_str(), static_cast<int>(password.length()),
        salt, SALT_SIZE,
        ITERATIONS,
        EVP_sha256(),
        KEY_LEN, derivedKey)) {
        throw std::runtime_error("PBKDF2 failed");
    }

    return bytesToHex(salt, SALT_SIZE) + ":" + bytesToHex(derivedKey, KEY_LEN);
}

bool PasswordService::verifyPassword(const std::string& password, const std::string& storedHash) {
    size_t pos = storedHash.find(':');
    if (pos == std::string::npos) return false;

    std::string saltHex = storedHash.substr(0, pos);
    std::string hashHex = storedHash.substr(pos + 1);

    unsigned char salt[SALT_SIZE];
    unsigned char derivedKey[KEY_LEN];

    if (!hexToBytes(saltHex, salt, SALT_SIZE)) return false;

    if (!PKCS5_PBKDF2_HMAC(
        password.c_str(), static_cast<int>(password.length()),
        salt, SALT_SIZE,
        ITERATIONS,
        EVP_sha256(),
        KEY_LEN, derivedKey)) {
        return false;
    }

    std::string computedHash = bytesToHex(derivedKey, KEY_LEN);
    return computedHash == hashHex;
}