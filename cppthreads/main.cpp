#include <iostream>
#include <thread>
#include "numbersstats.h"
#include "charsstats.h"

using namespace std;

int main()
{
    NumbersStats numbers_stats;
    CharsStats chars_stats;
    // fork
    auto run_chars = [&]()
    {
        chars_stats.load("chars.txt");
        chars_stats.process();
        cout << "Statistics on the text: " << endl;
        cout << "\tlength: " << chars_stats.getLength() << endl;
        cout << "\tprintable: " << chars_stats.getPrint() << endl;
        cout << "\talphabet characters: " << chars_stats.getAlpha() << endl;
        cout << "\tlowercase: " << chars_stats.getLower() << endl;
        cout << "\tuppercase: " << chars_stats.getUpper() << endl;
        cout << "\tdigits: " << chars_stats.getDigit() << endl;
        cout << "\tpunctuation characters: " << chars_stats.getPunct() << endl;
        cout << "\tblank characters: " << chars_stats.getBlank() << endl;
        cout << "\twhitespace characters: " << chars_stats.getSpace() << endl;
    }; // these two lines are executed in parallel
    thread chars_thread(run_chars);

    auto run_numbers = [&]()
    {
        numbers_stats.load("numbers.txt");
        numbers_stats.process();
        cout << "Statistics on numbers: " << endl;
        cout << "\tTotal number of values in the array: " << numbers_stats.getNumber() << endl;
        cout << "\tsum: " << numbers_stats.getSum() << endl;
        cout << "\tmu: " << numbers_stats.getMean() << endl;
        cout << "\tsigma: " << numbers_stats.getStd_diff() << endl;
    }; // these two lines are executed in parallel
    thread numbers_thread(run_numbers);
    // join
    numbers_thread.join();
    chars_thread.join();
    return 0;
}
