#include "../lib/longArithmetic.h"
#include <functional>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <string>
// https://www.craig-wood.com/nick/articles/pi-chudnovsky/
// The source for the algorithm pi calculation algorithm
LongNumber CalcPi(int32_t precision);

LongNumber CalcPi(int32_t precision) {
    ++precision;

    auto math_sqrt = [](const LongNumber &num) {
        LongNumber lh = 0;
        LongNumber rh = num + 1;
        LongNumber eps = 1;
        eps.setExp(-70);
        while (rh - lh > eps) {
            auto mid = lh + (rh - lh) * 0.5;

            if (mid * mid < num) {
                lh = mid;
            } else {
                rh = mid;
            }
        }
        return lh + (rh - lh) * 0.5;
    };

    auto sqrt = [&math_sqrt](const LongNumber &n, const LongNumber &one) {
        LongNumber floating_point_precision = "10000000000000000";
        auto n_float = LongNumberDiv(n * floating_point_precision, one, 0) /
                       floating_point_precision;
        auto tmp = (floating_point_precision * math_sqrt(n_float) * one);
        tmp.evalf(0);
        auto x = LongNumberDiv(tmp, floating_point_precision, 0);
        auto n_one = n * one;
        while (1) {
            auto x_old = x;
            x = LongNumberDiv(x + LongNumberDiv(n_one, x, 0), 2, 0);
            if (x == x_old) {
                break;
            }
        }
        return x;
    };

    LongNumber C3_OVER_24 = "10939058860032000";

    struct triple {
        LongNumber first;
        LongNumber second;
        LongNumber third;
    };

    std::function<triple(int64_t, int64_t)> bs;
    bs = [&bs, &C3_OVER_24](const int64_t &a, const int64_t &b) -> triple {
        LongNumber Pab = 0;
        LongNumber Qab = 0;
        LongNumber Tab = 0;
        if (b - a == 1) {
            if (a == 0) {
                Pab = Qab = 1;
            } else {
                Pab = (6 * a - 5) * (2 * a - 1) * (6 * a - 1);
                Qab = a * a * a * C3_OVER_24;
            }
            Tab = Pab * (13591409 + 545140134 * a);
            if (a & 1) {
                Tab.setSign(Tab.getSign() * -1);
            }
        } else {
            int64_t m = (a + b) / 2;
            auto res_a = bs(a, m);
            auto res_b = bs(m, b);
            Pab = res_a.first * res_b.first;
            Qab = res_a.second * res_b.second;
            Tab = res_b.second * res_a.third + res_a.first * res_b.third;
        }

        triple res = {Pab, Qab, Tab};
        return res;
    };

    LongNumber digits_per_term =
            "14.181647462725477655525521678181770863769125289828726959816854332";
    int64_t n = static_cast<int>(precision / 14.1816474627 + 1);
    auto res = bs(0, n);

    LongNumber one = 1;
    for (int i = 0; i < precision; ++i) {
        one = one * 10;
    }

    auto sqrtC = sqrt(10005 * one, one);
    auto r = res.second * sqrtC * 426880;
    auto pi = LongNumberDiv(r, res.third, 0);

    auto pi_str = pi.toString();
    while (pi_str.length() > precision) {
        pi_str.pop_back();
    }
    pi = LongNumber(pi_str);
    pi.setExp(pi.getExp() - (pi_str.length()) + 1);

    return pi;
}


using namespace std::chrono;

int main() {
    std::string pi =
            "3."
            "1415926535897932384626433832795028841971693993751058209749445923078164"
            "0628620899862803482534211706798214808651328230664709384460955058223172"
            "5359408128481117450284102701938521105559644622948954930381964428810975"
            "6659334461284756482337867831652712019091456485669234603486104543266482"
            "1339360726024914127372458700660631558817488152092096282925409171536436"
            "7892590360011330530548820466521384146951941511609433057270365759591953"
            "0921861173819326117931051185480744623799627495673518857527248912279381"
            "8301194912983367336244065664308602139494639522473719070217986094370277"
            "0539217176293176752384674818467669405132000568127145263560827785771342"
            "7577896091736371787214684409012249534301465495853710507922796892589235"
            "4201995611212902196086403441815981362977477130996051870721134999999837"
            "2978049951059731732816096318595024459455346908302642522308253344685035"
            "2619311881710100031378387528865875332083814206171776691473035982534904"
            "2875546873115956286388235378759375195778185778053217122680661300192787"
            "66111959092164201989";

    printf("Test of Pi calculation\n");

    auto check = [&pi](int digits) {
        auto time1 = clock();
        auto res = CalcPi(digits);
        auto time2 = clock();
        auto res_str = res.toString();
        std::cout << "Pi: " << res_str << "\n";
        int64_t duration =
                ((time2 - time1)) * 1000 / CLOCKS_PER_SEC;

        auto right_res_str = pi.substr(0, digits + 2);
        while (right_res_str.back() == '0') {
            right_res_str.pop_back();
        }

        if (right_res_str == res_str) {
            std::cout << "OK (" << duration << " ms)\n";
            return true;
        }
        std::cout << "Expected: " << right_res_str.c_str() << "\n";
        std::cout << "Expected: " << res_str.c_str() << "\n";
        std::cout << "Expected: " << res.toString().length() << "\n";
        std::cout << "ERROR" << duration << " ms)\n";
        return false;
    };

    int32_t precision = 0;
    std::cout << "Input the precision (int between 1 and 1000)\n";
    int8_t sc = scanf("%d", &precision);
    if (sc == 0 || precision < 1 || precision > 1000) {
        std::cout << "Wrong precision of pi\n";
        exit(1);
    }
    check(precision);

    return 0;
}
