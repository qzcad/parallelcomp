#ifndef CHARSSTATS_H
#define CHARSSTATS_H

#include <vector>


class CharsStats
{
public:
    CharsStats();

    void load(const char * filename);

    void process();

    long getLength() const;

    long getUpper() const;

    long getLower() const;

    long getDigit() const;

    long getAlpha() const;

    long getBlank() const;

    long getPrint() const;

    long getPunct() const;

    long getSpace() const;

private:
    long length;
    long upper;
    long lower;
    long digit;
    long alpha;
    long blank;
    long print;
    long punct;
    long space;
    std::vector<char> data;
};

#endif // CHARSSTATS_H
