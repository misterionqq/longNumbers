#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "longArithmetic.h"

using namespace std;

struct structForTest {
    longNumber first;
    longNumber second;
    longNumber received;
    longNumber expected;
    char operation;
    bool flag;
    double time;
};

struct boolStructForTest {
    longNumber first;
    longNumber second;
    short received;
    short expected;
    string operation;
    bool flag;
    double time;
};

boolStructForTest nowBoolTesting(const string& operation, const longNumber &first, const longNumber &second, short result) {
    boolStructForTest test;

    test.first = first;
    test.second = second;

    clock_t time1 = clock();

    if (operation == ">") {
        test.received = first > second;
    } else {
        if (operation == "<") {
            test.received = first < second;
        }
        else {
            if (operation == ">=") {
                test.received = first >= second;
            } else {
                if (operation == "<=") {
                    test.received = first <= second;
                } else {
                    if (operation == "==") {
                        test.received = first == second;
                    } else {
                        if (operation == "!=") {
                            test.received = first != second;
                        } else {

                        }
                    }
                }
            }
        }
    }

    clock_t time2 = clock();

    test.time = ((double) (time2 - time1)) * 1000 / CLOCKS_PER_SEC;
    test.expected = result;
    test.operation = operation;
    test.flag = test.expected == test.received;

    return test;
}

structForTest nowTesting(char operation, const longNumber &first, const longNumber &second, const longNumber &result) {
    structForTest test;

    test.first = first;
    test.second = second;

    clock_t time1 = clock();

    switch (operation) {
        case '+':
            test.received = first + second;
            break;

        case '-':
            test.received = first - second;
            break;

        case '*':
            test.received = first * second;
            break;

        case '/':
            test.received = first / second;
            break;
    }

    clock_t time2 = clock();

    test.time = ((double) (time2 - time1)) * 1000 / CLOCKS_PER_SEC;
    test.expected = result;
    test.operation = operation;
    test.flag = test.expected == test.received;

    return test;
}

structForTest nowTesting(char operation, const longNumber &first, const longNumber &result) {
    structForTest test;

    test.first = first;

    clock_t time1 = clock();

    switch (operation) {
        case '#':
            test.received = first.sqrt();
            break;
    }

    clock_t time2 = clock();

    test.time = ((double) (time2 - time1)) * 1000 / CLOCKS_PER_SEC;
    test.expected = result;
    test.operation = operation;
    test.flag = test.expected == test.received;

    return test;
}

void printBoolTestResults(const vector<boolStructForTest> &tests) {
    size_t results = 0;

    for (size_t i = 0; i < tests.size(); i++) {
        results += tests[i].flag;

        cout << "Test " << (i + 1) << ") ";

        if (tests[i].flag)
            cout << "OK (" << tests[i].time << " ms)";
        else {
            cout << "Failed: "
                << tests[i].first << " " << tests[i].operation << " " << tests[i].second << " = "
                << tests[i].expected
                << " != " << tests[i].received;
        }

        cout << endl;
    }

    cout << "==================================" << endl;
    cout << results << " / " << tests.size() << " (" << (100 * results / tests.size()) << "%)" << endl << endl;
}

void printTestResults(const vector<structForTest> &tests) {
    size_t results = 0;

    for (size_t i = 0; i < tests.size(); i++) {
        results += tests[i].flag;

        cout << "Test " << (i + 1) << ") ";

        if (tests[i].flag)
            cout << "OK (" << tests[i].time << " ms)";
        else {
            cout << "Failed: ";

            if (tests[i].operation != '#') {
                cout << tests[i].first << " " << tests[i].operation << " " << tests[i].second << " = "
                     << tests[i].expected
                     << " != " << tests[i].received;
            } else {
                cout << "sqrt(" << tests[i].first << ") = " << tests[i].expected << " != " << tests[i].received;
            }
        }

        cout << endl;
    }

    cout << "==================================" << endl;
    cout << results << " / " << tests.size() << " (" << (100 * results / tests.size()) << "%)" << endl << endl;
}

void testOfAddition() {
    vector<structForTest> additions;

    additions.push_back(nowTesting('+', 0, 0, 0));
    additions.push_back(nowTesting('+', 0, 31241324132, 31241324132));
    additions.push_back(nowTesting('+', longNumber("27.5"), longNumber("1.025"), longNumber("28.525")));
    additions.push_back(nowTesting('+', longNumber("-27.5"), longNumber("1.025"), longNumber("-26.475")));
    additions.push_back(nowTesting('+', longNumber("5"), longNumber("0.000000001"), longNumber("5.000000001")));
    additions.push_back(nowTesting('+', longNumber("2.7"), longNumber("0.018281828"), longNumber("2.718281828")));
    additions.push_back(nowTesting('+', 123456789, 987654321, longNumber("1111111110")));
    additions.push_back(nowTesting('+', 987654321, 123456789, longNumber("1111111110")));
    additions.push_back(nowTesting('+', -123456789, 987654321, longNumber("864197532")));
    additions.push_back(nowTesting('+', -987654321, 123456789, longNumber("-864197532")));

    cout << "Test of addition\n" << endl;
    printTestResults(additions);
}

void testOfSubstaction() {
    vector<structForTest> subtractions;

    subtractions.push_back(nowTesting('-', 0, 0, 0));
    subtractions.push_back(nowTesting('-', 0, -100, 100));
    subtractions.push_back(nowTesting('-', -100, 0, -100));
    subtractions.push_back(nowTesting('-', 12.5, -1.45, 13.95));
    subtractions.push_back(nowTesting('-', longNumber("3.14159265357989"), longNumber("3.14"), longNumber("0.00159265357989")));
    subtractions.push_back(nowTesting('-', longNumber("2.718281828"), longNumber("0.018281828"), longNumber("2.7")));
    subtractions.push_back(nowTesting('-', longNumber("5.000000001"), longNumber("0.000000001"), longNumber("5")));
    subtractions.push_back(nowTesting('-', 123456789, 987654321, longNumber("-864197532")));
    subtractions.push_back(nowTesting('-', -123456789, -987654321, longNumber("864197532")));
    subtractions.push_back(nowTesting('-', -987654321, -123456789, longNumber("-864197532")));

    cout << "Test of subtraction\n" << endl;
    printTestResults(subtractions);
}

void testOfMultiplication() {
    vector<structForTest> multiplications;

    multiplications.push_back(nowTesting('*', 0, 0, 0));
    multiplications.push_back(nowTesting('*', 0, 1331.534, 0));
    multiplications.push_back(nowTesting('*', -1, -141341.5431, 141341.5431));
    multiplications.push_back(nowTesting('*', 12.5, 2.05, 25.625));
    multiplications.push_back(nowTesting('*', -12.5, -2.05, 25.625));
    multiplications.push_back(nowTesting('*', 123456789, 987654321, longNumber("121932631112635269")));
    multiplications.push_back(nowTesting('*', 987654321, 123456789, longNumber("121932631112635269")));
    multiplications.push_back(nowTesting('*', -123456789, 987654321, longNumber("-121932631112635269")));
    multiplications.push_back(nowTesting('*', -987654321, 123456789, longNumber("-121932631112635269")));
    multiplications.push_back(nowTesting('*', -123456789, -987654321, longNumber("121932631112635269")));

    cout << "Test of multiplication\n" << endl;
    printTestResults(multiplications);
}

void testOfDivision() {
    vector<structForTest> divisions;

    divisions.push_back(nowTesting('/', 0, 5, 0));
    divisions.push_back(nowTesting('/', 0, longNumber(0.413413413), 0));
    divisions.push_back(nowTesting('/', 1, 2, 0.5));
    divisions.push_back(nowTesting('/', 3, 5, 0.6));
    divisions.push_back(nowTesting('/', 3.96, 4, 0.99));
    divisions.push_back(nowTesting('/', 1500, 300, 5));
    divisions.push_back(nowTesting('/', 2957325, 655, 4515));
    divisions.push_back(nowTesting('/', longNumber("121932631112635269"), longNumber("-987654321"), longNumber("-123456789")));
    divisions.push_back(nowTesting('/', longNumber("121932631112635269"), longNumber("987654321"), longNumber("123456789")));
    divisions.push_back(nowTesting('/', longNumber("-121932631112635269"), longNumber("987654321"), longNumber("-123456789")));

    cout << "Test of division\n" << endl;
    printTestResults(divisions);
}

void testOfRoots() {
    std::vector<structForTest> sqrts;
    sqrts.push_back(nowTesting('#', 0, 0));
    sqrts.push_back(nowTesting('#', 1, 1));
    sqrts.push_back(nowTesting('#', 16, 4));
    sqrts.push_back(nowTesting('#', 9, 3));
    sqrts.push_back(nowTesting('#', 100, 10));
    sqrts.push_back(nowTesting('#', longNumber(361), 19));
    sqrts.push_back(nowTesting('#', 225, 15));
    sqrts.push_back(nowTesting('#', 1.44, 1.2));
    sqrts.push_back(nowTesting('#', 0.01, 0.1));
    sqrts.push_back(nowTesting('#', 0.0001, 0.01));

    cout << "Test of roots\n" << endl;
    printTestResults(sqrts);
}

void testOfBools() {
    std::vector<boolStructForTest> bools;
    bools.push_back(nowBoolTesting(">", 0, 1, 0));
    bools.push_back(nowBoolTesting("<", 1321, -21321, 0));
    bools.push_back(nowBoolTesting(">=", longNumber("21321431541342.3413343241"), longNumber("21321431541342.3413343241"), 1));
    bools.push_back(nowBoolTesting("<=", longNumber("21321431541342.9"), longNumber("21321431541342.8"), 0));
    bools.push_back(nowBoolTesting("==", 314134, 314314312, 0));
    bools.push_back(nowBoolTesting("!=", longNumber("13143214231.413241234"), longNumber("13143214231.413241234"), 0));

    cout << "Test of bools\n" << endl;
    printBoolTestResults(bools);
}

int main() {
    testOfAddition();
    testOfSubstaction();
    testOfMultiplication();
    testOfDivision();
    testOfBools();
    testOfRoots();
}
