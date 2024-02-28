#include "longArithmetic.h"
#include <algorithm>
#include <cmath>
#include <utility>

LongNumber::LongNumber() { Init(); }

void LongNumber::printBlocks() const {
    std::cout << "Sign = " << sign_ << std::endl;
    for (auto &item: digits_) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
    std::cout << "Exp = " << exp_ << std::endl;
}

void LongNumber::Init() {
    digits_ = std::vector<uint16_t>(0);
    sign_ = 1;
    exp_ = 0;
}

void LongNumber::Init(std::string str) {
    try {
        if (str.empty()) {
            throw std::string("Empty string was given to constructor");
        }
        Init();

        if (str[0] == '-') {
            sign_ = -1;
            str.erase(str.begin());
        }
        while (str.front() == '0') {
            str.erase(str.begin());
        }
        auto dote_itr = std::find(str.begin(), str.end(), '.');
        if (dote_itr != str.end()) {
            while (str.back() == '0') {
                str.pop_back();
            }
            exp_ = std::distance(str.end(),
                                 std::find(str.begin(), str.end(), '.')) +
                   1;
            str.erase(dote_itr);
        } else {
            while (str.back() == '0') {
                str.pop_back();
                ++exp_;
            }
        }

        if (str.empty()) {
            str = "0";
            exp_ = 0;
        }

        std::reverse(str.begin(), str.end());

        SeparateToBlocks(str);
    } catch (...) {
        throw std::string("Undefined trouble with init");
    }
}

std::string LongNumber::toString(bool with_sign, bool with_dote) const {
    std::string res;
    for (auto itr = digits_.end() - 1; itr >= digits_.begin(); --itr) {
        std::string num = std::to_string(*itr);
        if (itr == digits_.end() - 1) {
            res += num;
        } else {
            res += std::string(block_size_ - num.length(), '0') + num;
        }
    }

    if (exp_ < 0 && res.length() < -exp_) {
        res = std::string((-exp_) - res.length(), '0') + res;
    }
    if (exp_ > 0) {
        res += std::string(exp_, '0');
    }

    if (with_dote && exp_ < 0) {
        res.insert(res.begin() + (res.length() + exp_), '.');
        if (res.front() == '.') {
            res = "0" + res;
        }
    }

    if (with_sign) {
        res = (sign_ == -1 ? "-" : "") + res;
    }
    return res;
}

void LongNumber::SeparateToBlocks(const std::string &str) {
    for (size_t ind = 0; ind < str.length(); ind += block_size_) {
        std::string block;
        if (str.length() - ind > block_size_) {
            block = str.substr(ind, block_size_);
        } else {
            block = str.substr(ind, str.length() - ind);
        }
        std::reverse(block.begin(), block.end());
        digits_.push_back(std::stoi(block));
    }
}

void LongNumber::ShiftToExp(int32_t new_exp) {
    if (new_exp == exp_)
        return;
    if (new_exp > exp_) {
        throw std::string("ShiftToExp can only shift to lower exponent");
    }
    decltype(exp_) diff_exp = exp_ - new_exp;

    while (diff_exp > 0) {
        if (diff_exp >= 4) {
            digits_.insert(digits_.begin(), 0);
            diff_exp -= 4;
        } else {
            int32_t mult = std::pow(10, diff_exp);
            int64_t acum = 0;
            for (auto &block: digits_) {
                int64_t temp_val = block * mult + acum;
                acum = (temp_val) / LongNumber::base_;
                block = (temp_val) % LongNumber::base_;
            }
            if (acum != 0) {
                digits_.push_back(acum);
            }
            diff_exp = 0;
        }
    }

    exp_ = new_exp;
}

LongNumber &LongNumber::operator=(LongNumber &&other) noexcept {
    if (this == &other) {
        return *this;
    }
    digits_ = std::move(other.digits_);
    sign_ = other.sign_;
    exp_ = other.exp_;
    return *this;
}

LongNumber &LongNumber::operator=(const LongNumber &other) {
    if (this == &other) {
        return *this;
    }
    *this = LongNumber(other);
    return *this;
}

LongNumber operator+(LongNumber first, LongNumber second) {
    if (first.sign_ != second.sign_) {
        if (first.sign_ == -1) {
            first.sign_ = 1;
            return second - first;
        }
        second.sign_ = 1;
        return first - second;
    }

    first.ShiftToExp(std::min(first.exp_, second.exp_));
    second.ShiftToExp(std::min(first.exp_, second.exp_));

    size_t new_num_len = std::max(first.digits_.size(), second.digits_.size());
    int64_t accum = 0;
    for (size_t ind = 0; ind < new_num_len || accum > 0; ++ind) {
        if (ind == first.digits_.size()) {
            first.digits_.push_back(0);
        }
        if (ind < second.digits_.size()) {
            first.digits_[ind] += second.digits_[ind] + accum;
        } else {
            first.digits_[ind] += accum;
        }
        accum = first.digits_[ind] / LongNumber::base_;
        first.digits_[ind] %= LongNumber::base_;
    }

    first.RemoveInsignificantZeroes();
    return first;
}

LongNumber operator-(LongNumber first, LongNumber second) {
    if (first.sign_ == -1 && second.sign_ == -1) {
        second.sign_ = 1;
        first.sign_ = 1;
        return second - first;
    }
    if (first.sign_ == 1 && second.sign_ == -1) {
        second.sign_ = 1;
        return first + second;
    }
    if (first.sign_ == -1 && second.sign_ == 1) {
        first.sign_ = 1;
        LongNumber res = first + second;
        res.sign_ = -1;
        return res;
    }

    first.ShiftToExp(std::min(first.exp_, second.exp_));
    second.ShiftToExp(std::min(first.exp_, second.exp_));

    bool is_change_sign = false;
    if (first < second) {
        is_change_sign = true;
        swap(first, second);
    }

    for (size_t ind = 0; ind < second.digits_.size(); ++ind) {
        if (first.digits_[ind] >= second.digits_[ind]) {
            first.digits_[ind] -= second.digits_[ind];
        } else {
            for (size_t ind_f = ind + 1; ind_f < first.digits_.size();
                 ++ind_f) {
                if (first.digits_[ind_f] != 0) {
                    --first.digits_[ind_f];
                    break;
                }
                first.digits_[ind_f] = LongNumber::base_ - 1;
            }
            first.digits_[ind] += LongNumber::base_ - second.digits_[ind];
        }
    }

    if (is_change_sign) {
        first.sign_ = -1;
    }

    first.RemoveInsignificantZeroes();
    return first;
}

LongNumber::LongNumber(const LongNumber &other) {
    digits_ = other.digits_;
    sign_ = other.sign_;
    exp_ = other.exp_;
}

bool operator<(const LongNumber &first, const LongNumber &second) {
    auto isLower = [](const LongNumber &first, const LongNumber &second,
                      bool debug = false) {
        if (first.sign_ < second.sign_)
            return true;
        if (first.sign_ > second.sign_)
            return false;

        auto max_precision = std::min(first.exp_, second.exp_);
        auto lh = first;
        auto rh = second;

        if (debug) {
            std::cout << "first: \n";
            first.printBlocks();

            std::cout << "second: \n";
            second.printBlocks();
        }

        lh.ShiftToExp(max_precision);
        rh.ShiftToExp(max_precision);

        if (debug) {
            std::cout << "lh: \n";
            lh.printBlocks();

            std::cout << "rh: \n";
            rh.printBlocks();
        }

        for (int ind = std::max(lh.digits_.size(), rh.digits_.size()) - 1;
             ind >= 0; --ind) {
            if (ind >= lh.digits_.size()) {
                if (rh.digits_[ind] != 0) {
                    return true;
                }
            } else if (ind >= rh.digits_.size()) {
                if (lh.digits_[ind] != 0) {
                    return false;
                }
            } else {
                if (lh.digits_[ind] > rh.digits_[ind]) {
                    if (debug) {
                        std::cout << "LOG (oper <)3: " << lh.toString() << " < "
                                  << rh.toString() << " = " << false
                                  << std::endl;
                    }
                    return false;
                }
                if (lh.digits_[ind] < rh.digits_[ind]) {
                    if (debug) {
                        std::cout << "LOG (oper <)4: " << lh.toString() << " < "
                                  << rh.toString() << " = " << true
                                  << std::endl;
                    }
                    return true;
                }
            }
        }

        if (debug) {
            std::cout << "LOG (oper <)5: " << lh.toString() << " < "
                      << rh.toString() << " = " << false << std::endl;
        }

        return false;
    };

    auto res2 = isLower(first, second);
    return res2;
}

LongNumber::LongNumber(LongNumber &&other) noexcept {
    digits_ = std::move(other.digits_);
    sign_ = other.sign_;
    exp_ = other.exp_;
}

LongNumber operator*(const LongNumber &first, const LongNumber &second) {
    if (first < second) {
        return second * first;
    }
    LongNumber res = 0;
    int cur_block_ind = 0;
    for (auto block: second.digits_) {
        uint64_t accum = 0;
        LongNumber inter_term;
        for (auto block_in_first: first.digits_) {
            auto temp_val = block * block_in_first + accum;
            inter_term.digits_.push_back(temp_val % LongNumber::base_);
            accum = (temp_val) / LongNumber::base_;
        }
        if (accum != 0) {
            inter_term.digits_.push_back(accum);
        }
        inter_term.exp_ = cur_block_ind++ * LongNumber::block_size_;
        res = res + inter_term;
    }

    res.sign_ = first.sign_ * second.sign_;
    res.exp_ += first.exp_ + second.exp_;

    res.RemoveInsignificantZeroes();
    return res;
}

void swap(LongNumber &lhs, LongNumber &rhs) {
    std::swap(lhs.digits_, rhs.digits_);
    std::swap(lhs.exp_, rhs.exp_);
    std::swap(lhs.sign_, rhs.sign_);
}

bool operator!=(const LongNumber &first, const LongNumber &second) {
    return !(first == second);
}

bool operator==(const LongNumber &first, const LongNumber &second) {
    if (first.sign_ != second.sign_) {
        return false;
    }

    auto max_precision = std::min(first.exp_, second.exp_);
    auto lh = first;
    auto rh = second;
    lh.ShiftToExp(max_precision);
    rh.ShiftToExp(max_precision);

    for (int ind = std::max(lh.digits_.size(), rh.digits_.size()) - 1; ind >= 0;
         --ind) {
        if (ind >= rh.digits_.size()) {
            if (lh.digits_[ind] != 0) {
                return false;
            }
        } else if (ind >= lh.digits_.size()) {
            if (rh.digits_.size() != 0) {
                return false;
            }
        } else {
            if (lh.digits_[ind] != rh.digits_[ind]) {
                return false;
            }
        }
    }

    return true;
}

bool operator>(const LongNumber &first, const LongNumber &second) {
    return second < first && second != first;
}

bool operator>=(const LongNumber &first, const LongNumber &second) {
    return second < first || second == first;
}

bool operator<=(const LongNumber &first, const LongNumber &second) {
    return first < second || first == second;
}

void LongNumber::RemoveInsignificantZeroes() {
    bool isAllZero = true;
    for (auto &elem: digits_) {
        if (elem != 0) {
            isAllZero = false;
        }
    }
    if (isAllZero) {
        Init("0");
        return;
    }

    if (exp_ >= 0) {
        while (digits_.back() == 0) {
            digits_.pop_back();
        }
        while (digits_.front() == 0) {
            digits_.erase(digits_.begin());
            exp_ += LongNumber::block_size_;
        }
        if (digits_.empty()) {
            digits_.push_back(0);
        }
    } else {
        size_t covered_blocks =
                (-exp_) / block_size_ + ((-exp_) % block_size_ == 0 ? 0 : 1);
        while (digits_.size() > covered_blocks) {
            if (digits_.back() != 0)
                break;
            digits_.pop_back();
        }
    }
}

LongNumber LongNumberDiv(LongNumber first, const LongNumber &second, int32_t precision,
                     bool debug) {
    if (precision < 0)
        return 0;
    if (first == 0)
        return 0;

    LongNumber res = 0;
    LongNumber divider = 0;
    LongNumber lh = 0;
    LongNumber rh = first + 2;
    rh.evalf(0);

    if (debug) {
        std::cout << std::endl;
        std::cout << "LOG: first = " << first.toString()
                  << "   second = " << second.toString() << std::endl;
        std::cout << "LOG: calculating divider ..." << std::endl;
    }

    if (first >= second) {
        int cnt = 0;
        while (rh - lh > 1) {
            auto mid = lh + (rh - lh) * 0.5_ln;
            mid.evalf(0);

            if (debug) {
                cnt++;
                if (cnt > 300) {
                    exit(1);
                }
                std::cout << "LOG (BINSEARCH): lh = " << lh.toString()
                          << "\tmid = " << mid.toString()
                          << "\trh = " << rh.toString() << std::endl;
            }

            if (second * mid > first) {
                rh = mid;
            } else if (second * mid <= first) {
                lh = mid;
            }
        }
        divider = lh;
    }

    if (debug) {
        std::cout << "LOG: divider = " << divider.toString() << std::endl;
    }
    if (divider == 0) {
        first = first * 10;
        if (debug) {
            std::cout << "LOG: divider == 0   ==>   res = " << first.toString()
                      << " / " << second.toString() << std::endl;
        }
        res = LongNumberDiv(std::move(first), second, precision - 1, debug);
        --res.exp_;
    } else {
        first = first - second * divider;
        if (debug) {
            std::cout << "LOG: divider == " << divider.toString()
                      << "  ==>   res = " << first.toString() << " / "
                      << second.toString() << std::endl;
        }
        res = divider + LongNumberDiv(std::move(first), second, precision, debug);
    }

    res.RemoveInsignificantZeroes();
    return res;
}

LongNumber operator/(LongNumber first, const LongNumber &second) {
    return LongNumberDiv(std::move(first), second, LongNumber::division_accuracy);
}

void LongNumber::evalf(int32_t precision) {
    while (precision > exp_) {
        if (precision - exp_ >= 4) {
            digits_.erase(digits_.begin());
            exp_ += 4;
        } else {
            *digits_.begin() -=
                    digits_.front() %
                    static_cast<int>(std::pow(10, precision - exp_));
            break;
        }
    }
}

LongNumber operator "" _ln(const char *val) {
    return {val};
}
