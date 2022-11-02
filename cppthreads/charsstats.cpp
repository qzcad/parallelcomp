#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstring>
#include "charsstats.h"

CharsStats::CharsStats()
{
    this->length = 0;
    this->alpha = 0;
    this->blank = 0;
    this->digit = 0;
    this->lower = 0;
    this->upper = 0;
    this->print = 0;
    this->punct = 0;
    this->space = 0;
}

void CharsStats::load(const char *filename)
{
    std::fstream file;
    char c;
    file.open(filename, std::ios_base::in);
    if (!file.is_open())
    {
        std::cout << "Error opening file: " << strerror(errno);
        pthread_exit(NULL);
    }
    while (file >> std::noskipws >> c)
    {
        this->data.push_back(c);
    }
}

void CharsStats::process()
{
    this->length = this->data.size();
    for (char c: this->data)
    {
        if (isalpha(c)) this->alpha += 1;
        if (isblank(c)) this->blank += 1;
        if (isdigit(c)) this->digit += 1;
        if (islower(c)) this->lower += 1;
        if (isupper(c)) this->upper += 1;
        if (isprint(c)) this->print += 1;
        if (ispunct(c)) this->punct += 1;
        if (isspace(c)) this->space += 1;
    }
}

long CharsStats::getLength() const
{
    return length;
}

long CharsStats::getUpper() const
{
    return upper;
}

long CharsStats::getLower() const
{
    return lower;
}

long CharsStats::getDigit() const
{
    return digit;
}

long CharsStats::getAlpha() const
{
    return alpha;
}

long CharsStats::getBlank() const
{
    return blank;
}

long CharsStats::getPrint() const
{
    return print;
}

long CharsStats::getPunct() const
{
    return punct;
}

long CharsStats::getSpace() const
{
    return space;
}
