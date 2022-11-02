#ifndef NUMBERSSTATS_H
#define NUMBERSSTATS_H

#include<vector>


class NumbersStats
{
public:
    NumbersStats();

    void load(const char * filename);

    void process();

    long getNumber() const;

    double getMean() const;

    double getSum() const;

    double getStd_diff() const;

private:
    std::vector<double> data;
    long number;
    double mean;
    double sum;
    double std_diff;
};

#endif // NUMBERSSTATS_H
