#include <iostream>
#include <fstream>
#include <vector>
#include <cerrno>
#include <cstring>
#include <cctype>
#include <pthread.h>
#include <math.h>

using namespace std;

struct NumbersStats
{
    long number;
    double mean;
    double sum;
    double std_diff;

    NumbersStats()
    {
        number = 0;
        mean = 0.0;
        sum = 0.0;
        std_diff = 0.0;
    }
};

void *load_numbers(void *arg)
{
    vector<double> numbers;
    fstream file;
    double f;
    const char *filename = "numbers.txt";
    NumbersStats * stats_pointer = (NumbersStats *)arg;
    file.open(filename, ios_base::in);
    if (!file.is_open())
    {
        cout << "Error opening file: " << strerror(errno);
        pthread_exit(NULL);
    }
    while (file >> f)
    {
        numbers.push_back(f);
    }
    stats_pointer->number = numbers.size(); // get the number of elements in the array
    // calculate the average
    for (double value: numbers)
    {
        stats_pointer->sum += value;
    }
    stats_pointer->mean = stats_pointer->sum / stats_pointer->number;
    // calculate the standard deviation
    for (double value: numbers)
    {
        double d = stats_pointer->mean - value;
        stats_pointer->std_diff += d * d;
    }
    stats_pointer->std_diff = sqrt(stats_pointer->std_diff / stats_pointer->number);
    file.close();
    pthread_exit(NULL);
}

struct TextStats
{
    long length;
    long upper;
    long lower;
    long digit;
    long alpha;
    long blank;
    long print;
    long punct;
    long space;

    TextStats()
    {
        length = 0;
        alpha = 0;
        blank = 0;
        digit = 0;
        lower = 0;
        upper = 0;
        print = 0;
        punct = 0;
        space = 0;
    }
};

void *load_chars(void *arg)
{
    vector<char> chars;
    fstream file;
    char c;
    TextStats * stats_pointer = (TextStats *)arg;
    file.open("chars.txt", ios_base::in);
    if (!file.is_open())
    {
        cout << "Error opening file: " << strerror(errno);
        pthread_exit(NULL);
    }
    while (file >> noskipws >> c)
    {
        chars.push_back(c);
    }
    stats_pointer->length = chars.size();
    for (char c: chars)
    {
        if (isalpha(c)) stats_pointer->alpha += 1;
        if (isblank(c)) stats_pointer->blank += 1;
        if (isdigit(c)) stats_pointer->digit += 1;
        if (islower(c)) stats_pointer->lower += 1;
        if (isupper(c)) stats_pointer->upper += 1;
        if (isprint(c)) stats_pointer->print += 1;
        if (ispunct(c)) stats_pointer->punct += 1;
        if (isspace(c)) stats_pointer->space += 1;
    }
    file.close();
    pthread_exit(NULL);
}

int main(int argc, char * argv[])
{
    pthread_t thread_numbers;
    NumbersStats number_stats;
    TextStats text_stats;
    int rc_numbers = pthread_create(&thread_numbers, NULL, load_numbers, (void *)&number_stats);
    pthread_t thread_chars;
    int rc_chars = pthread_create(&thread_chars, NULL, load_chars, (void *)&text_stats);
    if (rc_numbers){
        printf("ERROR; return code from pthread_create() is %d\n", rc_numbers);
        exit(-1);
    }
    if (rc_chars){
        printf("ERROR; return code from pthread_create() is %d\n", rc_numbers);
        exit(-1);
    }
    pthread_join(thread_numbers, NULL);
    cout << endl << "Statistics on numbers: " << endl;
    cout << "\tTotal number of values in the array: " << number_stats.number << endl;
    cout << "\tsum: " << number_stats.sum << endl;
    cout << "\tmu: " << number_stats.mean << endl;
    cout << "\tsigma: " << number_stats.std_diff << endl;
    pthread_join(thread_chars, NULL);
    cout << endl << "Statistics on the text: " << endl;
    cout << "\tlength: " << text_stats.length << endl;
    cout << "\tprintable: " << text_stats.print << endl;
    cout << "\talphabet characters: " << text_stats.alpha << endl;
    cout << "\tlowercase: " << text_stats.lower << endl;
    cout << "\tuppercase: " << text_stats.upper << endl;
    cout << "\tdigits: " << text_stats.digit << endl;
    cout << "\tpunctuation characters: " << text_stats.punct << endl;
    cout << "\tblank characters: " << text_stats.blank << endl;
    cout << "\twhitespace characters: " << text_stats.space << endl;
    pthread_exit(NULL);
    return 0;
}
