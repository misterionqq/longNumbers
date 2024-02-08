#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

class longNumber {
    const size_t divDigits = 1000; // Точность для деления
    const size_t sqrtDigits = 100; // Точность для корня

    int sign; // Знак числа
    std::vector<int> digits; // Вектор цифр числа (и целая, и дробная части)
    long exponent; // Значение сдвига точки

    void initFromString(const std::string& s); // Инициализация из строки
    void removeZeroes(); // Удаление бесполезных нулей в записи числа

public:
    longNumber(); // Стандартный конструктор
    longNumber(const longNumber& other); // Копирование числа
    longNumber(long double value); // Конструктор из long double
    longNumber(const std::string& s); // Конструктор из строки

    std::string longNumberToString(); // Преобразование длинного числа в строку

    longNumber& operator=(const longNumber& other); // Перегрузка оператора приравнивания

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
