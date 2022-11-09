#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;

double rand_d(double min, double max)
{
    int r = rand();
    double f = (double)r / RAND_MAX;
//    cout << r << " " << f << " " << min << " " << max << " ";
    return min + f * (max - min);
}

vector<vector<double>> random_matrix(int n, double min = 0.0, double max = 10.0)
{
    vector<vector<double>> m(n, vector<double>(n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++) {
            m[i][j] = rand_d(min, max);
        }
    }
    return m;
}

void print_matrix(const vector<vector<double>> &m)
{
    for (int i = 0; i < m.size(); ++i) {
        for (int j = 0; j < m[i].size(); ++j) {
            cout << m[i][j] << " ";
        }
        cout << endl;
    }
}

const vector<vector<double>> multiply_matrix(const vector<vector<double>> &a, const vector<vector<double>> &b)
{
    int rows = a.size();
    int cols_a = a[0].size();
    int rows_b = b.size();
    int cols = b[0].size();
    vector<vector<double>> m(rows, vector<double>(cols));
    if (cols_a != rows_b)
    {
        cerr << "ERROR: " << cols_a << " <> " << rows_b << endl;
        return m;
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            m[i][j] = 0.0;
            for (int k = 0; k < cols_a; ++k) {
                m[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return m;
}

const vector<vector<double>> omp_multiply_matrix(const vector<vector<double>> &a, const vector<vector<double>> &b)
{
    int rows = a.size();
    int cols_a = a[0].size();
    int rows_b = b.size();
    int cols = b[0].size();
    vector<vector<double>> m(rows, vector<double>(cols));
    if (cols_a != rows_b)
    {
        cerr << "ERROR: " << cols_a << " <> " << rows_b << endl;
        return m;
    }
#pragma omp parallel for
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            m[i][j] = 0.0;
            for (int k = 0; k < cols_a; ++k) {
                m[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return m;
}

int main()
{
    int size_min = 50;
    int size_max = 1000;
    int size_step = 50;
    srand((unsigned int)time(NULL)); // set a seed
    ofstream report("report.csv");
    cout << "Number of cores (procesors): " << omp_get_num_procs() << endl;
    report << "size;";
    for (int size = size_min; size <= size_max; size += size_step)
    {
        report << size << ";";
    }
    report << endl;
    report << "1;";
    for (int size = size_min; size <= size_max; size += size_step) {
        vector<vector<double>> a = random_matrix(size);
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        vector<vector<double>> m = multiply_matrix(a, a);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        int duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        cout << size << ": " << duration << endl;
        report << duration << ";";
    }
    report << endl;
    for (int cores = 2; cores <= omp_get_num_procs(); cores++) {
        omp_set_num_threads(cores);
        cout << "Cores: " << cores << endl;
        report << cores << ";";
        for (int size = size_min; size <= size_max; size += size_step) {
            vector<vector<double>> a = random_matrix(size);
            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            vector<vector<double>> m = omp_multiply_matrix(a, a);
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            int duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
            cout << "\t" << size << ": " << duration << endl;
            report << duration << ";";
        }
        report << endl;
    }
    report.close();
    return 0;
}
