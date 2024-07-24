#ifndef COMPARABLE_H
#define COMPARABLE_H

class Comparable {
public:
    virtual ~Comparable() = default;

    virtual bool operator<(const Comparable& other) const = 0;
    bool operator>(const Comparable& other) {
        return !((*this < other) || (*this == other));
    }
    virtual bool operator==(const Comparable& other) const = 0;

    virtual bool isRemovable() const {
        return true;
    }
};

#endif // COMPARABLE_H
