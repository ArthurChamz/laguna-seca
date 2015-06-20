#include <iostream>
#include <fstream>
#include <complex>
#include <valarray>
#include <cmath>
#include <pthread.h>
#include <sched.h>

//#define N 8

using namespace std;

typedef complex<double> Complex;

void* begin(void*);
void fourier(valarray<Complex>&);


int main(int argc, char** argv){
    cpuset_t* cset;
    cpuid_t   cid;
    pthread_t thread;

    cset = cpuset_create();
    if(cset == NULL){
        cout << "Error loco." << endl;
        pthread_exit(0);
    }

    cid = 0;
    cpuset_set(cid, cset);

    pthread_setaffinity_np(thread, cpuset_size(cset), cset);

    pthread_create(&thread, NULL, begin, NULL);

    pthread_join(thread, NULL);

    cpuset_destroy(cset);

    pthread_exit(0);
}


void* begin(void* pData){
    //const Complex test[] = { 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0 };
    //valarray<Complex> data(test, N);

    int N;
    int j = 0;
    ifstream fin("FFT.txt");

    if(!fin){
        cout << "Error de archivo loco" << endl;
        pthread_exit(0);
    }

    fin >> N;

    if(N < 1){
        cout << "Mal formato." << endl;
        pthread_exit(0);
    }

    Complex* test = new Complex[N];

    while(!fin.eof()){
        fin >> test[j];
        cout << test[j++] << endl;
        if(j == N) break;
    }

    cout << endl;

    valarray<Complex> data(test, N);

    fourier(data);

    for(int i = 0; i < N; i++){
        cout << data[i] << endl;
    }

    pthread_exit(0);

}

void fourier(valarray<Complex>& x){
    int n = x.size();
    if(n <= 1) return;
    
    int half = n / 2;

    valarray<Complex> even = x[slice(0, half, 2)];
    valarray<Complex>  odd = x[slice(1, half, 2)];

    fourier(even);
    fourier(odd);

    for(int i = 0; i < half; i++){
        Complex t = polar(1.0, -2 * M_PI * i / n) * odd[i];
        x[i       ] = even[i] + t;
        x[i + half] = even[i] - t;
    }
}
