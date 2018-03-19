// Compares threaded bubble sort to sequential quick sort performance.
#include <iostream>
#include <random>
#include <algorithm>
#include <functional>
#include <vector>
#include <chrono>

#define NUM_VECTORS     100
#define SIZE_VECTORS    100
#define NUM_THREADS     1

using namespace std;

// Bubble sort. Takes a vector to order as input.
vector<int> bubblesort(vector<int> a) {

    bool swap = true;
    while(swap) {
        swap = false;
        for (size_t i=0; i<a.size()-1; i++) {
            if (a[i]>a[i+1] ){
                a[i] += a[i+1];
                a[i+1] = a[i] - a[i+1];
                a[i] -=a[i+1];
                swap = true;
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

// generates vector with SIZE_VECTORS random elements
vector<int> random_vector() {

    random_device rnd_device;
    mt19937 mersenne_engine(rnd_device());
    uniform_int_distribution<int> dist(1, SIZE_VECTORS);

    auto gen = std::bind(dist, mersenne_engine);
    vector<int> vec(SIZE_VECTORS);
    generate(begin(vec), end(vec), gen);

    return vec;

}

void printvec(vector<int> &v) {
    cout << endl;
    for(auto e : v) {
        cout << e << " ";
    }
    cout << endl;
}

// MAIN
int main() {

    // generate random vector of vectors of int
    vector<vector<int>> vec;
    for(int k=0; k<NUM_VECTORS; k++) {
        vec.push_back(random_vector());
    }

    int el = 0;
    for(auto i : vec) {
        for(auto k : i) {
            el++;
            //cout << k << ", ";
        }
    }

    // assert number of values is correct
    if (el != NUM_VECTORS * SIZE_VECTORS) {
        cout << "\n\tAlgo de errado não está certo" << endl;
        exit(EXIT_FAILURE);
    }

    std::chrono::duration<double> elapsed;

    // bubble sort benchmark
    auto start = std::chrono::high_resolution_clock::now();
    for(auto v : vec) {
        vector<int> sorted = bubblesort(v);
        //printvec(sorted);
    }
    auto finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "Bubble sort time for " << NUM_VECTORS << " vectors of "<< SIZE_VECTORS << " elements: " << elapsed.count() << " s\n";

    // quick sort benchmark
    start = std::chrono::high_resolution_clock::now();
    for(auto v : vec) {
        vector<int> sorted = quicksort(v, 0, vec.size()-1);
        //printvec(sorted);
    }
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "Quick sort time for " << NUM_VECTORS << " vectors of "<< SIZE_VECTORS << " elements: " << elapsed.count() << " s\n";

	return 0;

}