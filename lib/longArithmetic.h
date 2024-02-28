#ifndef HEADER_GUARD_LONGARITHMETIC_HPP_INCLUDED
#define HEADER_GUARD_LONGARITHMETIC_HPP_INCLUDED
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <bitset>
#include <cmath>
#include <concepts>
#include <type_traits>

#include <cstddef>
#include <cstdint>

// Templates for initialization
template <typename T>
concept Number = std::integral<T> || std::floating_point<T>;

template <typename T>
concept ConvertableToString = std::constructible_from<std::string, T>;

template <typename T>
concept HasToString = requires(T t) {
    { t.toString() } -> std::same_as<std::string>;
};

class LongNumber {
private:
    std::vector<uint16_t> digits_; // Digits of number
    int sign_; // Sign of number
    static const uint64_t base_ = 10000; // Optimization base of number
    int32_t exp_; // Exponent of number
    static const int block_size_ = 4; // Optimization part of number
    static const int32_t division_accuracy = 100; // Accuracy for division

    void Init(); // Standard constructor
    void Init(std::string str); // Constructor from string

    void SeparateToBlocks(const std::string &str); // Separate to blocks (optimization part)
    void RemoveInsignificantZeroes(); // Deleting of useless zeroes

public:
    LongNumber();

    // Templates for initialization
    template <HasToString T> LongNumber(T val) { Init(val.toString()); }
    template <ConvertableToString T> LongNumber(T val) { Init(std::string(val)); }
    template <Number T> LongNumber(T val) { Init(std::to_string(val)); }

    // Initialization from other number
    LongNumber(const LongNumber &other);
    LongNumber(LongNumber &&other) noexcept;

    // Assigment operator overloading
    LongNumber &operator=(LongNumber &&other) noexcept;
    LongNumber &operator=(const LongNumber &other);

    // Blocks print (for debug)
    void printBlocks() const;

    // Shift number to new exponent
    void ShiftToExp(int32_t new_exp);

    // Converting number to string
    [[nodiscard]] std::string toString(bool with_sign = true, bool with_dote = true) const;

    int getSign() { return sign_; } // Getting sign
    std::vector<uint16_t> getBlocks() { return digits_; } // Getting blocks
    int32_t getExp() { return exp_; } // Getting exponent

    void setSign(int sign) { this->sign_ = sign; } // Setting sign
    void setExp(int32_t exp) { this->exp_ = exp; } // Setting exponent

    void evalf(int32_t precision); // Bringing to given accuracy

    // Swap of numbers
    friend void swap(LongNumber &lhs, LongNumber &rhs);

    // Arithmetic operators overloading
    friend LongNumber operator+(LongNumber, LongNumber);
    friend LongNumber operator-(LongNumber, LongNumber);
    friend LongNumber operator/(LongNumber, const LongNumber &);
    friend LongNumber operator*(const LongNumber &, const LongNumber &);
    friend LongNumber LongNumberDiv(LongNumber first, const LongNumber &second,
                            int32_t precision, bool debug);

    // Bool operators overloading
    friend bool operator<(const LongNumber &, const LongNumber &);
    friend bool operator<=(const LongNumber &, const LongNumber &);
    friend bool operator!=(const LongNumber &, const LongNumber &);
    friend bool operator==(const LongNumber &, const LongNumber &);
    friend bool operator>(const LongNumber &, const LongNumber &);
    friend bool operator>=(const LongNumber &, const LongNumber &);
};

LongNumber LongNumberDiv(LongNumber first, const LongNumber &second, int32_t precision,
                 bool debug = false);

LongNumber operator"" _ln(const char *val);

#endif // HEADER_GUARD_LONGARITHMETIC_HPP_INCLUDED