#include <vector>
#include <memory>

#ifndef GUMA_H_
#define GUMA_H_

template<typename T> class Pole {
public:
    //Default ctor
    Pole(size_t chunk) : chunk_(chunk), count_(0) {}

    //Copy ctor
    Pole(const Pole& pole) 
    {
        count_ = 0;
        chunk_ = pole.chunk_;
        pole.copy_into(*this);
    }

    //Move ctor
    Pole(Pole&& pole) : hrabe_(std::move(pole.hrabe_)), chunk_(pole.chunk_), count_(pole.count_) {}

    //Copy assign
    Pole& operator=(const Pole& pole) 
    {
        count_ = 0;
        chunk_ = pole.chunk_;
        hrabe_.clear();
        pole.copy_into(*this);
        return *this;
    }

    //Move assign
    Pole& operator=(Pole&& pole) 
    {
        hrabe_ = std::move(pole.hrabe_);
        chunk_ = pole.chunk_;
        count_ = pole.count_;
        return *this;
    }

    //Dtor
    ~Pole() {}

    T& operator [] (size_t i) 
    {
        return hrabe_[i / chunk_][i % chunk_];
    }
    const T& operator [] (size_t i) const 
    {
        return hrabe_[i / chunk_][i % chunk_];
    }
    void push_back(const T& x) 
    {
        grow_if();
        operator[](count_++) = x;
    }

    class iterator {
    public:
        static const size_t end_ = -1;
        iterator() : pole_(nullptr), offset_(-1) {}
        iterator(const iterator& it) : pole_(it.pole_), offset_(it.offset_) {}

        iterator(size_t i, Pole* pole) : pole_(pole), offset_(i) {}

        T& operator* ()
        {
            return (*pole_)[offset_];
        }
        bool operator != (const iterator& x) 
        {
            return this->pole_ != x.pole_ || this->offset_ != x.offset_;
        }
        iterator& operator++ () 
        {
            if (++offset_ >= pole_->count_) 
            {
                offset_ = end_;
            }
            return *this;
        }
    private:
        Pole* pole_;
        size_t offset_;
    };

    iterator begin()
    {
        return iterator(0, this);
    }
    iterator end()
    {
        return iterator(iterator::end_, this);
    }

private:
    //Increases the size of the vector if it is full 
    //so there is space for pushing new stuff into it or for setting end iterator
    void grow_if()
    {
        if (!(count_ % chunk_)) 
        {
            //We are full
            hrabe_.push_back(std::make_unique<T[]>(chunk_));
        }
    }
    void copy_into(Pole<T>& into) const 
    {
        for (size_t i = 0; i < count_; ++i) 
        {
            into.push_back(operator[](i));
        }
    }

    std::vector<std::unique_ptr<T[]>> hrabe_;
    size_t chunk_;
    size_t count_;
};
#endif