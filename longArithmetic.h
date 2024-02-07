#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

class longNumber {
    const size_t divDigits = 1000;
    const size_t sqrtDigits = 100;

    int sign;
    std::vector<int> digits;
    long exponent;

    void initFromString(const std::string& s);
    void removeZeroes();
    void normalize();

public:
    longNumber();
    longNumber(const longNumber& other);
    longNumber(long double value);
    longNumber(const std::string& s);

    std::string longNumberToString();

    longNumber& operator=(const longNumber& other);

    bool operator>(const longNumber& other) const;
    bool operator<(const longNumber& other) const;
    bool operator>=(const longNumber& other) const;
    bool operator<=(const longNumber& other) const;
    bool operator==(const longNumber& other) const;
    bool operator!=(const longNumber& other) const;

    longNumber operator-() const;

    longNumber operator+(const longNumber& other) const;
    longNumber operator-(const longNumber& other) const;
    longNumber operator*(const longNumber& other) const;
    longNumber operator/(const longNumber& other) const;
    longNumber operator++(int);
    longNumber operator--(int);

    longNumber& operator+=(const longNumber& other);
    longNumber& operator-=(const longNumber& other);
    longNumber& operator*=(const longNumber& other);
    longNumber& operator/=(const longNumber& other);


    longNumber& operator++();
    longNumber& operator--();

    longNumber abs() const;
    longNumber sqrt() const;
    longNumber inverse() const;

    bool isZero() const;

    friend std::ostream& operator<<(std::ostream& os, const longNumber& value);
};

longNumber operator ""_ln(long double val);

//std::tuple<longNumber, longNumber, longNumber> binary_split(longNumber a, longNumber b);
//longNumber piCalculation(longNumber n);

//std::tuple<long double, long double, long double> binary_split(long double a, long double b);
//long double piCalculation(long double n);