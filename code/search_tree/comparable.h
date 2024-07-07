#ifndef COMPARABLE_H
#define COMPARABLE_H

class Comparable {
public:
    virtual ~Comparable() = default;

    virtual bool operator<(const Comparable& other) const = 0;
    virtual bool operator>(const Comparable& other) const = 0;
    virtual bool operator==(const Comparable& other) const = 0;
};

#endif // COMPARABLE_H
