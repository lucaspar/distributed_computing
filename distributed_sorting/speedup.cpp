// Compares threaded bubble sort to sequential quick sort performance.
#include <functional>
#include <algorithm>
#include <iostream>
#include <utility>
#include <thread>
#include <vector>
#include <chrono>
#include <random>
#include <cmath>

#define NUM_VECTORS         1000                        // number of vectors
#define SIZE_VECTORS        1000                        // size of vectors
#define THREADING_ORDER     floor(log2(NUM_VECTORS))    // max threading order (2^THREADING_ORDER threads)

using namespace std;

void printvec(vector<int> &v) {
    cout << endl;
    for(auto e : v) {
        cout << e << " ";
    }
    cout << endl;
}

// Bubble sort. Takes a vector to order as input.
vector<int> bubblesort(vector<int> a) {
    bool swapped = true;
    int aux;
    while(swapped) {
        swapped = false;
        for (auto &it : a) {
            if ( it > *(&it+1) ) {
                aux = it;
                it = *(&it+1);
                *(&it+1) = aux;
                swapped = true;
            }
        }
    }

    return a;

}

// Aux function for quick sort
void swap(vector<int>& v, int x, int y) {
    int temp = v[x];
    v[x] = v[y];
    v[y] = temp;
}

// Quick sort. Takes a vector to order as input.
vector<int> quicksort(vector<int> vec, int L, int R) {
    int i, j, mid, piv;
    i = L;
    j = R;
    mid = L + (R - L) / 2;
    piv = vec[mid];

    while (i<R || j>L) {
        while (vec[i] < piv)
            i++;
        while (vec[j] > piv)
            j--;

        if (i <= j) {
            swap(vec, i, j);
            i++;
            j--;
        }
        else {
            if (i < R)
                quicksort(vec, i, R);
            if (j > L)
                quicksort(vec, L, j);
            break;
        }
    }

    return vec;
}

// Threaded bubble sort execution
void threaded_bubble_sort(vector<vector<int>> vec, const int NUM_THREADS) {
    std::thread th[NUM_THREADS];

    int k=0;
    for(auto v : vec) {
        //vector<int> sorted = bubblesort(v);
        // await for the current indexed thread to finish, if any
        if(th[k].joinable()) {
            std::thread::id tid = th[k].get_id();
            th[k].join();
            // cout << tid << " is finished. Thread idx " << k << endl;
        }
        th[k] = std::thread(bubblesort, v);
        k++;
        k %= NUM_THREADS;
    }
    // join remaining threads
    for(int t=0; t<NUM_THREADS; t++) {
        th[t].join();
    }
}

// Generate random vector of vectors of integers
vector<vector<int>> generate_vectors() {
    vector<vector<int>> vec;
    for(int k=0; k<NUM_VECTORS; k++) {

        random_device rnd_device;
        mt19937 mersenne_engine(rnd_device());
        uniform_int_distribution<int> dist(1, SIZE_VECTORS);

        auto gen = std::bind(dist, mersenne_engine);
        vector<int> v(SIZE_VECTORS);
        generate(begin(v), end(v), gen);

        vec.push_back(v);

    }
    return vec;
}

// Single thread quick sort execution
void sequential_quick_sort(vector<vector<int>> vec) {
    for(auto v : vec) {
        vector<int> sorted = quicksort(v, 0, vec.size()-1);
        //printvec(sorted);
    }
}

// MAIN
int main() {

    std::chrono::duration<double> elapsed;
    if(pow(2,THREADING_ORDER) > NUM_VECTORS) {
        cout << "Oops. The number of vectors should be the same or higher than the number of threads." << endl;
        exit(EXIT_FAILURE);
    }

    // generate random vector of vectors of int
    vector<vector<int>> vec = generate_vectors();

    cout << "SORTING BENCHMARK" << endl;
    cout << NUM_VECTORS << " vectors of "<< SIZE_VECTORS << " elements" << endl;
    cout << " --------------- " << endl << endl;

    // quick sort benchmark
    cout << "Quick sort time:" << endl;
    auto start = std::chrono::high_resolution_clock::now();
    sequential_quick_sort(vec);
    auto finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    cout << elapsed.count() << " s\n";

    // threaded bubble sort benchmark
    cout << "Threaded bubble sort time:" << endl;
    int num_threads;
    for (int order=THREADING_ORDER; order>=0; order--) {
        num_threads = pow(2, order);
        cout << num_threads << " threads ";
        fflush(stdout);

        start = std::chrono::high_resolution_clock::now();
        threaded_bubble_sort(vec, num_threads);
        finish = std::chrono::high_resolution_clock::now();
        elapsed = finish - start;

        cout << elapsed.count() << " s\n";
    }

	return 0;

}