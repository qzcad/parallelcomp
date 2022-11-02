#include <iostream>
#include <fstream>
#include <vector>
#include <cerrno>
#include <cstring>
#include <cctype>
#include <math.h>
#include "numbersstats.h"

NumbersStats::NumbersStats()
{
    this->number = 0;
    this->mean = 0.0;
    this->sum = 0.0;
    this->std_diff = 0.0;
}

void NumbersStats::load(const char *filename)
{
    std::fstream file;
    double f;
    file.open(filename, std::ios_base::in);
    if (!file.is_open())
    {
        std::cout << "Error opening file: " << strerror(errno);
        pthread_exit(NULL);
    }
    while (file >> f)
    {
        this->data.push_back(f);
    }
}

void NumbersStats::process()
{
    this->number = this->data.size(); // get the number of elements in the array
    // calculate the average
    for (double value: this->data)
    {
        this->sum += value;
    }
    this->mean = this->sum / this->number;
    // calculate the standard deviation
    for (double value: this->data)
    {
        double d = this->mean - value;
        this->std_diff += d * d;
    }
    this->std_diff = sqrt(this->std_diff / this->number);
}

long NumbersStats::getNumber() const
{
    return number;
}

double NumbersStats::getMean() const
{
    return mean;
}

double NumbersStats::getSum() const
{
    return sum;
}

double NumbersStats::getStd_diff() const
{
    return std_diff;
}
