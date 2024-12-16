#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <random>

using namespace std;

class oneD_FFT{ 

    private:
    vector<complex<double>> V;
    vector<complex<double>> sol; 

    public:
    oneD_FFT(vector<complex<double>> x) : V(x) {}

    vector<complex<double>> getInput(){
        return V;
    }

    vector<complex<double>> getResult(){
        return sol;
    }

    void compute(){

        int n = V.size();
        int logn = log2(n);

        //reverse bit ordering
        for (int i = 0; i < n; ++i) {
            int reversed_index = 0;
            for (int j = 0; j < logn; ++j) {
                if (i & (1 << j)) {
                    reversed_index |= (1 << (logn - j - 1));
                }
            }
            if (reversed_index > i) {
                swap(V[i], V[reversed_index]);
            }
        }

        sol = V;
        
        //computing fft
        for (int size = 2; size <= n; size *= 2) {
            int half_size = size / 2;
            double angle = -2.0 * M_PI / size; 
            for (int start = 0; start < n; start += size) {
                for (int i = 0; i < half_size; ++i) {
                    int j = start + i;
                    int k = j + half_size;
                    complex<double> t = exp(complex<double>(0, angle * i)) * sol[k];
                    sol[k] = sol[j] - t;
                    sol[j] += t;
                } 
            
            }
        }
    }

};
