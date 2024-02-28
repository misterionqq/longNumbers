#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "../lib/longArithmetic.h"

using namespace std;

struct structForTest {
    LongNumber first;
    LongNumber second;
    LongNumber received;
    LongNumber expected;
    char operation;
    bool flag;
    double time;
};

struct boolStructForTest {
    LongNumber first;
    LongNumber second;
    short received;
    short expected;
    string operation;
    bool flag;
    double time;
};

boolStructForTest nowBoolTesting(const string& operation, const LongNumber &first, const LongNumber &second, short result) {
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

structForTest nowTesting(char operation, const LongNumber &first, const LongNumber &second, const LongNumber &result) {
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


void printBoolTestResults(const vector<boolStructForTest> &tests) {
    size_t results = 0;

    for (size_t i = 0; i < tests.size(); i++) {
        results += tests[i].flag;

        cout << "Test " << (i + 1) << ") ";

        if (tests[i].flag)
            cout << "OK (" << tests[i].time << " ms)";
        else {
            cout << "Failed: "
                 << tests[i].first.toString() << " " << tests[i].operation << " " << tests[i].second.toString() << " = "
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
                cout << (tests[i].first).toString() << " " << tests[i].operation << " " << (tests[i].second).toString() << " = "
                     << (tests[i].expected).toString()
                     << " != " << (tests[i].received).toString();
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
    additions.push_back(nowTesting('+', LongNumber("27.5"), LongNumber("1.025"), LongNumber("28.525")));
    additions.push_back(nowTesting('+', LongNumber("-27.5"), LongNumber("1.025"), LongNumber("-26.475")));
    additions.push_back(nowTesting('+', LongNumber("5"), LongNumber("0.000000001"), LongNumber("5.000000001")));
    additions.push_back(nowTesting('+', LongNumber("2.7"), LongNumber("0.018281828"), LongNumber("2.718281828")));
    additions.push_back(nowTesting('+', 123456789, 987654321, LongNumber("1111111110")));
    additions.push_back(nowTesting('+', 987654321, 123456789, LongNumber("1111111110")));
    additions.push_back(nowTesting('+', -123456789, 987654321, LongNumber("864197532")));
    additions.push_back(nowTesting('+', -987654321, 123456789, LongNumber("-864197532")));

    cout << "Test of addition\n" << endl;
    printTestResults(additions);
}

void testOfSubstaction() {
    vector<structForTest> subtractions;

    subtractions.push_back(nowTesting('-', 0, 0, 0));
    subtractions.push_back(nowTesting('-', 0, -100, 100));
    subtractions.push_back(nowTesting('-', -100, 0, -100));
    subtractions.push_back(nowTesting('-', 12.5, -1.45, 13.95));
    subtractions.push_back(nowTesting('-', LongNumber("3.14159265357989"), LongNumber("3.14"), LongNumber("0.00159265357989")));
    subtractions.push_back(nowTesting('-', LongNumber("2.718281828"), LongNumber("0.018281828"), LongNumber("2.7")));
    subtractions.push_back(nowTesting('-', LongNumber("5.000000001"), LongNumber("0.000000001"), LongNumber("5")));
    subtractions.push_back(nowTesting('-', 123456789, 987654321, LongNumber("-864197532")));
    subtractions.push_back(nowTesting('-', -123456789, -987654321, LongNumber("864197532")));
    subtractions.push_back(nowTesting('-', -987654321, -123456789, LongNumber("-864197532")));

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
    multiplications.push_back(nowTesting('*', 123456789, 987654321, LongNumber("121932631112635269")));
    multiplications.push_back(nowTesting('*', 987654321, 123456789, LongNumber("121932631112635269")));
    multiplications.push_back(nowTesting('*', -123456789, 987654321, LongNumber("-121932631112635269")));
    multiplications.push_back(nowTesting('*', -987654321, 123456789, LongNumber("-121932631112635269")));
    multiplications.push_back(nowTesting('*', -123456789, -987654321, LongNumber("121932631112635269")));

    cout << "Test of multiplication\n" << endl;
    printTestResults(multiplications);
}

void testOfDivision() {
    vector<structForTest> divisions;

    divisions.push_back(nowTesting('/', 0, 5, 0));
    divisions.push_back(nowTesting('/', 0, LongNumber(0.413413413), 0));
    divisions.push_back(nowTesting('/', 1, 2, 0.5));
    divisions.push_back(nowTesting('/', 3, 5, 0.6));
    divisions.push_back(nowTesting('/', 3.96, 4, 0.99));
    divisions.push_back(nowTesting('/', 1500, 300, 5));
    divisions.push_back(nowTesting('/', 2957325, 655, 4515));

    cout << "Test of division\n" << endl;
    printTestResults(divisions);
}

void testOfBools() {
    std::vector<boolStructForTest> bools;
    bools.push_back(nowBoolTesting(">", 0, 1, 0));
    bools.push_back(nowBoolTesting("<", 1321, -21321, 0));
    bools.push_back(nowBoolTesting(">=", LongNumber("21321431541342.3413343241"), LongNumber("21321431541342.3413343241"), 1));
    bools.push_back(nowBoolTesting("<=", LongNumber("21321431541342.9"), LongNumber("21321431541342.8"), 0));
    bools.push_back(nowBoolTesting("==", 314134, 314314312, 0));
    bools.push_back(nowBoolTesting("!=", LongNumber("13143214231.413241234"), LongNumber("13143214231.413241234"), 0));

    cout << "Test of bools\n" << endl;
    printBoolTestResults(bools);
}

int main() {
    testOfAddition();
    testOfSubstaction();
    testOfMultiplication();
    testOfDivision();
    testOfBools();
}
