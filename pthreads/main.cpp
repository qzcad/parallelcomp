#include <iostream>
#include <fstream>
#include <vector>
#include <cerrno>
#include <cstring>
#include <pthread.h>

using namespace std;
typedef struct {
    int number;
    double mean;
    double sum;
    double std_diff;
} NumbersStats;

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
    stats_pointer->number = 0;
    stats_pointer->mean = 1.0;
    stats_pointer->sum = 0.0;
    stats_pointer->std_diff = 0.0;
    file.close();
    pthread_exit(NULL);
}

void *load_chars(void *)
{
    vector<char> chars;
    fstream file;
    char c;
    file.open("chars.txt", ios_base::in);
    if (!file.is_open())
    {
        cout << "Error opening file: " << strerror(errno);
        pthread_exit(NULL);
    }
    while (file >> noskipws >> c)
    {
        cout << c;
        chars.push_back(c);
    }
    cout << endl << "Total loaded " << chars.size() << " chars. " << endl;
    file.close();
    pthread_exit(NULL);
}

int main(int argc, char * argv[])
{
    pthread_t thread_numbers;
    NumbersStats number_stats;
    int rc_numbers = pthread_create(&thread_numbers, NULL, load_numbers, (void*)&number_stats);
    pthread_t thread_chars;
    int rc_chars = pthread_create(&thread_chars, NULL, load_chars, NULL);
    if (rc_numbers){
        printf("ERROR; return code from pthread_create() is %d\n", rc_numbers);
        exit(-1);
    }
    if (rc_chars){
        printf("ERROR; return code from pthread_create() is %d\n", rc_numbers);
        exit(-1);
    }
    pthread_join(thread_numbers, NULL);
    pthread_join(thread_chars, NULL);
    cout << endl << "Total numbers: " << number_stats.number << endl;
    cout << "Numbers mean: " << number_stats.mean << endl;
    pthread_exit(NULL);
    return 0;
}
