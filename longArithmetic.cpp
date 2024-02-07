#include "longArithmetic.h"

using namespace std;

void longNumber::initFromString(const string &s) {
    size_t index;

    if (s[0] == '-') {
        sign = -1;
        index = 1;
    } else {
        sign = 1;
        index = 0;
    }

    exponent = s.length() - index;

    while (index < s.length()) {
        if (s[index] == '.')
            exponent = sign == 1 ? index : index - 1;
        else
            digits.push_back(s[index] - '0');

        index++;
    }
}

void longNumber::removeZeroes() {
    size_t n = max((long) 1, exponent);

    while (digits.size() > n && digits[digits.size() - 1] == 0)
        digits.erase(digits.end() - 1);

    while (digits.size() > 1 && digits[0] == 0) {
        digits.erase(digits.begin());
        exponent--;
    }

    while (digits.size() > 1 && digits[digits.size() - 1] == 0)
        digits.erase(digits.end() - 1);

    if (isZero()) {
        exponent = 1;
        sign = 1;
    }

    normalize();
}

void longNumber::normalize() {
    size_t start = max(exponent, (long) 0);
    size_t realDigits = digits.size() - start;

    if (realDigits >= divDigits) {
        size_t count = 0;
        size_t maxCount = 0;

        size_t i = start;

        while (i < digits.size()) {
            count = 0;

            while (i < digits.size() && digits[i] == 9) {
                count++;
                i++;
            }

            if (count > maxCount)
                maxCount = count;

            i++;
        }

        if (maxCount > divDigits * 4 / 5) {
            i = digits.size() - 1;

            do {
                count = 0;

                while (i > 0 && digits[i] != 9)
                    i--;

                while (i > 0 && digits[i] == 9) {
                    count++;
                    i--;
                }
            } while (count != maxCount);

            digits.erase(digits.begin() + i + 1, digits.end());
            digits[i]++;
        }
    }
}

longNumber::longNumber() {
    sign = 1;
    digits = vector<int>(1, 0);
    exponent = 1;
}

longNumber::longNumber(const longNumber &other) {
    sign = other.sign;
    exponent = other.exponent;
    digits = vector<int>(other.digits.size());

    for (size_t i = 0; i < other.digits.size(); i++)
        digits[i] = other.digits[i];
}

longNumber::longNumber(long double value) {
    stringstream ss;
    ss << setprecision(15) << value;

    initFromString(ss.str());
    removeZeroes();
}

longNumber::longNumber(const string &s) {
    initFromString(s);
    removeZeroes();
}

longNumber &longNumber::operator=(const longNumber &other) {
    if (this == &other)
        return *this;

    sign = other.sign;
    exponent = other.exponent;
    digits = vector<int>(other.digits.size());

    for (size_t i = 0; i < other.digits.size(); i++)
        digits[i] = other.digits[i];

    return *this;
}

bool longNumber::operator>(const longNumber &other) const {
    if (sign != other.sign)
        return sign > other.sign;

    if (exponent != other.exponent) {
        if (this->isZero())
            return false;
        if (other.isZero())
            return true;
        return (exponent > other.exponent) ^ (sign == -1);
    }

    vector<int> d1(digits);
    vector<int> d2(other.digits);
    size_t size = max(d1.size(), d2.size());

    while (d1.size() != size)
        d1.push_back(0);

    while (d2.size() != size)
        d2.push_back(0);

    for (size_t i = 0; i < size; i++)
        if (d1[i] != d2[i])
            return (d1[i] > d2[i]) ^ (sign == -1);

    return false;
}

bool longNumber::operator<(const longNumber &other) const {
    return !(*this > other || *this == other);
}

bool longNumber::operator>=(const longNumber &other) const {
    return *this > other || *this == other;
}

bool longNumber::operator<=(const longNumber &other) const {
    return *this < other || *this == other;
}

bool longNumber::operator==(const longNumber &other) const {
    if (sign != other.sign)
        return false;

    if (exponent != other.exponent)
        return false;

    if (digits.size() != other.digits.size())
        return false;

    for (size_t i = 0; i < digits.size(); i++)
        if (digits[i] != other.digits[i])
            return false;

    return true;
}

bool longNumber::operator!=(const longNumber &other) const {
    return !(*this == other);
}

longNumber longNumber::operator-() const {
    longNumber result(*this);
    result.sign = -sign;

    return result;
}

longNumber longNumber::operator+(const longNumber &other) const {
    if (sign == other.sign) {
        long exponent1 = exponent;
        long exponent2 = other.exponent;
        long exp = max(exponent1, exponent2);

        vector<int> d1(digits);
        vector<int> d2(other.digits);

        while (exponent1 != exp) {
            d1.insert(d1.begin(), 0);
            exponent1++;
        }

        while (exponent2 != exp) {
            d2.insert(d2.begin(), 0);
            exponent2++;
        }

        size_t size = max(d1.size(), d2.size());

        while (d1.size() != size)
            d1.push_back(0);

        while (d2.size() != size)
            d2.push_back(0);

        size_t len = 1 + size;

        longNumber result;

        result.sign = sign;
        result.digits = vector<int>(len, 0);

        for (size_t i = 0; i < size; i++)
            result.digits[i + 1] = d1[i] + d2[i];

        for (size_t i = len - 1; i > 0; i--) {
            result.digits[i - 1] += result.digits[i] / 10;
            result.digits[i] %= 10;
        }

        result.exponent = exp + 1;
        result.removeZeroes();

        return result;
    }

    if (sign == -1)
        return other - (-(*this));

    return *this - (-other);
}

longNumber longNumber::operator-(const longNumber &other) const {
    if (sign == 1 && other.sign == 1) {
        bool flag = *this > other;

        long exponent1 = flag ? exponent : other.exponent;
        long exponent2 = flag ? other.exponent : exponent;
        long exp = max(exponent1, exponent2);

        vector<int> d1(flag ? digits : other.digits);
        vector<int> d2(flag ? other.digits : digits);

        while (exponent1 != exp) {
            d1.insert(d1.begin(), 0);
            exponent1++;
        }

        while (exponent2 != exp) {
            d2.insert(d2.begin(), 0);
            exponent2++;
        }

        size_t size = max(d1.size(), d2.size());

        while (d1.size() != size)
            d1.push_back(0);

        while (d2.size() != size)
            d2.push_back(0);

        size_t len = 1 + size;

        longNumber result;

        result.sign = flag ? 1 : -1;
        result.digits = vector<int>(len, 0);

        for (size_t i = 0; i < size; i++)
            result.digits[i + 1] = d1[i] - d2[i];

        for (size_t i = len - 1; i > 0; i--) {
            if (result.digits[i] < 0) {
                result.digits[i] += 10;
                result.digits[i - 1]--;
            }
        }

        result.exponent = exp + 1;
        result.removeZeroes();

        return result;
    }

    if (sign == -1 && other.sign == -1)
        return (-other) - (-(*this));

    return *this + (-other);
}

longNumber longNumber::operator*(const longNumber &other) const {
    size_t len = digits.size() + other.digits.size();

    longNumber result;

    result.sign = sign * other.sign;
    result.digits = vector<int>(len, 0);
    result.exponent = exponent + other.exponent;

    for (size_t i = 0; i < digits.size(); i++)
        for (size_t j = 0; j < other.digits.size(); j++)
            result.digits[i + j + 1] += digits[i] * other.digits[j];

    for (size_t i = len - 1; i > 0; i--) {
        result.digits[i - 1] += result.digits[i] / 10;
        result.digits[i] %= 10;
    }

    result.removeZeroes();

    return result;
}

longNumber longNumber::operator/(const longNumber &other) const {
    longNumber result = *this * other.inverse();

    size_t intPart = max((long) 0, exponent);

    if (intPart > result.digits.size() - 1)
        return result;

    size_t i = result.digits.size() - 1 - intPart;
    size_t n = max((long) 0, result.exponent);

    if (i > n && result.digits[i] == 9) {
        while (i > n && result.digits[i] == 9)
            i--;

        if (result.digits[i] == 9) {
            result.digits.erase(result.digits.begin() + n, result.digits.end());
            result = result + result.sign;
        } else {
            result.digits.erase(result.digits.begin() + i + 1, result.digits.end());
            result.digits[i]++;
        }
    }

    return result;
}

longNumber &longNumber::operator+=(const longNumber &other) {
    return (*this = *this + other);
}

longNumber &longNumber::operator-=(const longNumber &other) {
    return (*this = *this - other);
}

longNumber &longNumber::operator*=(const longNumber &other) {
    return (*this = *this * other);
}

longNumber &longNumber::operator/=(const longNumber &other) {
    return (*this = *this / other);
}

longNumber longNumber::operator++(int) {
    longNumber res(*this);
    *this = *this + 1;

    return res;
}

longNumber longNumber::operator--(int) {
    longNumber res(*this);
    *this = *this - 1;

    return res;
}

longNumber &longNumber::operator++() {
    return (*this = *this + 1);
}

longNumber &longNumber::operator--() {
    return (*this = *this - 1);
}

longNumber longNumber::inverse() const {
    if (isZero())
        throw string("longNumber longNumber::inverse() - деление на 0!");

    longNumber other(*this);
    other.sign = 1;

    longNumber divisible("1");

    longNumber result;
    result.sign = sign;
    result.exponent = 1;
    result.digits = vector<int>();

    while (other < 1) {
        other.exponent++;
        result.exponent++;
    }

    while (divisible < other)
        divisible.exponent++;

    result.exponent -= divisible.exponent - 1;

    size_t numbers = 0;
    size_t intPart = max((long) 0, result.exponent);
    size_t maxNumbers = divDigits + intPart;

    do {
        int div = 0;

        while (divisible >= other) {
            div++;
            divisible -= other;
        }

        divisible.exponent++;
        divisible.removeZeroes();

        result.digits.push_back(div);
        numbers++;
    } while (!divisible.isZero() && numbers < maxNumbers);

    return result;
}

bool longNumber::isZero() const {
    return digits.size() == 1 && digits[0] == 0;
}

string longNumber::longNumberToString() {
    string str = "";
    if (this->sign == -1)
        str += "-";

    if (this->exponent > 0) {
        size_t i = 0;
        size_t e = this->exponent;

        while (i < this->digits.size() && i < e)
            str += to_string(this->digits[i++]);

        while (i < e) {
            str += "0";
            i++;
        }

        if (i < this->digits.size()) {
            str += ".";

            while (i < this->digits.size())
                str += to_string(this->digits[i++]);
        }
    } else if (this->exponent == 0) {
        str += "0.";

        for (size_t i = 0; i < this->digits.size(); i++)
            str += to_string(this->digits[i]);
    } else {
        str += "0.";

        for (long i = 0; i < -this->exponent; i++)
            str += "0";

        for (size_t i = 0; i < this->digits.size(); i++)
            str += to_string(this->digits[i]);
    }

    return str;
}

longNumber operator ""_ln(long double val) {
    return longNumber(val);
}

ostream &operator<<(ostream &os, const longNumber &value) {
    if (value.sign == -1)
        os << '-';

    if (value.exponent > 0) {
        size_t i = 0;
        size_t e = value.exponent;

        while (i < value.digits.size() && i < e)
            os << value.digits[i++];

        while (i < e) {
            os << "0";
            i++;
        }

        if (i < value.digits.size()) {
            os << ".";

            while (i < value.digits.size())
                os << value.digits[i++];
        }
    } else if (value.exponent == 0) {
        os << "0.";

        for (size_t i = 0; i < value.digits.size(); i++)
            os << value.digits[i];
    } else {
        os << "0.";

        for (long i = 0; i < -value.exponent; i++)
            os << "0";

        for (size_t i = 0; i < value.digits.size(); i++)
            os << value.digits[i];
    }

    return os;
}

longNumber longNumber::abs() const {
    longNumber result(*this);
    result.sign = 1;

    return result;
}

longNumber longNumber::sqrt() const {
    if (sign == -1)
        throw string("longNumber longNumber::sqrt() - number is negative");

    if (isZero())
        return 0;

    longNumber x0;
    longNumber p("0.5");
    longNumber xk("0.5");
    longNumber eps;
    eps.digits = vector<int>(1, 1);
    eps.exponent = 1 - sqrtDigits;

    do {
        x0 = xk;
        xk = p * (x0 + *this / x0);
    } while ((x0 - xk).abs() > eps);

    xk.digits.erase(xk.digits.begin() + max((long) 0, xk.exponent) + sqrtDigits, xk.digits.end());
    xk.removeZeroes();

    return xk;
}

// Жалкая попытка через свой класс
/*
longNumber piCalculation(longNumber n) {
    tuple<longNumber, longNumber, longNumber> pepe;
    pepe = binary_split(1, n);
    longNumber P1n = get<0>(pepe), Q1n = get<1>(pepe), R1n = get<2>(pepe);
    return (longNumber(426880) * longNumber(10005).sqrt() * Q1n) / (longNumber(13591409)*Q1n + R1n);
}

tuple<longNumber, longNumber, longNumber> binary_split(longNumber a, longNumber b) {
    longNumber Pab, Qab, Rab;
    tuple<longNumber, longNumber, longNumber> pepe;
    if (b == a + longNumber(1)) {
        Pab = -(longNumber(6) * a - longNumber(5)) * (longNumber(2) * a - longNumber(1)) * \
        (longNumber(6) * a - longNumber(1));
        Qab = longNumber(10939058860032000) * a * a * a;
        Rab = Pab * (longNumber(545140134) * a + longNumber(13591409));
    } else {
        longNumber m = (a + b) / longNumber(2);
        pepe = binary_split(a, m);
        longNumber Pam = get<0>(pepe), Qam = get<1>(pepe), Ram = get<2>(pepe);
        pepe = binary_split(m, b);
        longNumber Pmb = get<0>(pepe), Qmb = get<1>(pepe), Rmb = get<2>(pepe);
        Pab = Pam * Pmb;
        Qab = Qam * Qmb;
        Rab = Qmb * Ram + Pam * Rmb;
    }
    return {Pab, Qab, Rab};
} */
// Гигачадная попытка через long double
/*
#include <cmath>

long double piCalculation(long double n) {
    tuple<long double, long double, long double> pepe;
    pepe = binary_split(1, n);
    long double P1n = get<0>(pepe), Q1n = get<1>(pepe), R1n = get<2>(pepe);
    return (426880 * sqrt(10005) * Q1n) / (13591409 * Q1n + R1n);
}

tuple<long double, long double, long double> binary_split(long double a, long double b) {
    long double Pab, Qab, Rab;
    tuple<long double, long double, long double> pepe;
    if (b == a + 1) {
        Pab = -(6 * a - 5) * (2 * a - 1) * \
        (6 * a - 1);
        Qab = 10939058860032000 * a * a * a;
        Rab = Pab * (545140134 * a + 13591409);
    } else {
        long double m = (a + b) / 2;
        pepe = binary_split(a, m);
        long double Pam = get<0>(pepe), Qam = get<1>(pepe), Ram = get<2>(pepe);
        pepe = binary_split(m, b);
        long double Pmb = get<0>(pepe), Qmb = get<1>(pepe), Rmb = get<2>(pepe);
        Pab = Pam * Pmb;
        Qab = Qam * Qmb;
        Rab = Qmb * Ram + Pam * Rmb;
    }
    return {Pab, Qab, Rab};
}
*/






















