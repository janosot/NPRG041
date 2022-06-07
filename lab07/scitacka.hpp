#include <ostream>
#include <string>

#ifndef SCITACKA_H_
#define SCITACKA_H_


template<typename T> class S {
public:
    template<typename F>
    friend S<F> operator+(const S<F>& x, const S<F>& y);
    S() : val_(T()) {}
    S(const T& val) : val_(val) {}
    void add(const T& x)
    {
        val_ += x;
    }
    T result() { return val_; }
private:
    T val_;
};

template<typename T>
S<T> operator+(const S<T>& x, const S<T>& y) 
{
    return S<T>(x.val_ + y.val_);
}

template<typename T> class fraction {
private:
    T numerator_, denominator_;
public:
    fraction() : numerator_(0), denominator_(1) {}
    fraction(const T& numerator, const T& denominator) 
    {
        numerator_ = numerator;
        denominator_ = denominator;
    }
    fraction operator+(const fraction& druhy) const 
    {
        return fraction(denominator_ * druhy.numerator_ + numerator_ * druhy.denominator_, denominator_ * druhy.denominator_);
    }
    fraction& operator+=(const fraction& druhy) 
    {
        *this = *this + druhy;
        return *this;
    }

    std::string print() const 
    {
        std::string s;
        s += std::to_string(numerator_);
        s += "/";
        s += std::to_string(denominator_);
        return s;
    }

    template<typename F>
    friend std::ostream& operator<<(std::ostream& os, const fraction<F>& z);

};

template<typename T>
std::ostream& operator<<(std::ostream& os, const fraction<T>& z) 
{
    os << z.print();
    return os;
}

#endif