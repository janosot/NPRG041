#include <vector>
#include <memory>
#include <string>
#include <iostream>

#ifndef SEZNAM_HPP_
#define SEZNAM_HPP_

class AbstractVal {
public:
    virtual void print() = 0;
    virtual ~AbstractVal() {}
    virtual std::unique_ptr<AbstractVal> clone() = 0;
};
/*
class IntVal : public AbstractVal {
private:
    int x;
public:
    IntVal(int y) :x(y) {}
    void print()
    {
        std::cout << x;
    }
    std::unique_ptr<AbstractVal> clone()
    {
        return std::make_unique<IntVal>(*this);
    }
};

class StringVal : public AbstractVal {
private:
    std::string x;
public:
    StringVal(std::string y) :x(y) {}
    void print()
    {
        std::cout << x;
    }
    std::unique_ptr<AbstractVal> clone()
    {
        return std::make_unique<StringVal>(*this);
    }
};
*/
template<typename T> class ConcreteVal : public AbstractVal {
private:
    T x;
public:
    ConcreteVal(T y) :x(y) {}
    void print() 
    {
        std::cout << x;
    }
    std::unique_ptr<AbstractVal> clone() 
    {
        return std::make_unique<ConcreteVal>(*this);
    }
};

class Seznam {
private:
    std::vector<std::unique_ptr<AbstractVal>> pole_;
    void clone(const Seznam& other)
    {
        for (auto&& x : other.pole_)
        {
            pole_.push_back(x->clone());
        }
    }
public:
    void add(std::unique_ptr<AbstractVal>&& p);
    void print();
    Seznam() {};
    Seznam(const Seznam& other);
    Seznam& operator=(const Seznam& other);
};

#endif
