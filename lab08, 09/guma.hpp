#include <vector>
#include <memory>

#ifndef GUMA_H_
#define GUMA_H_


template<typename T> class Pole {
public:
    //cv09 "big 6" Extension copy constructor and move constructor for Gumové pole 
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
    }

    //Move assign
    Pole& operator=(Pole&& pole) 
    {
        hrabe_ = std::move(pole.hrabe_);
        chunk_ = pole.chunk_;
        count_ = pole.count_;
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

    class iterator 
    {
    public:
        iterator() :item_(nullptr), pole_(nullptr), offset_(-1) {}

        iterator(const iterator& it) : item_(it.item_), pole_(it.pole_), offset_(it.offset_) {}

        iterator(size_t i, Pole* pole) : item_(&((*pole)[i])), pole_(pole), offset_(i) {}

        T& operator* () 
        {
            return *item_;
        }
        bool operator != (const iterator& x) 
        {
            return  item_ != x.item_;
        }
        iterator& operator++ () 
        {
            item_ = &((*pole_)[++offset_]);
            return *this;
        }
    private:
        T* item_;
        Pole* pole_;
        size_t offset_;
    };
    //Extension for cv09 end(), begin()
    iterator begin() 
    {
        return iterator(0, this);
    }
    iterator end() 
    {
        //This has to be done in order to function with my operator++
        grow_if();
        return iterator(count_, this);
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