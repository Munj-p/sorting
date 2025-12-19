#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <fstream> 

void quickFirst(int arr[], int low, int high) {
    if (low >= high) return;
    int p = arr[low];
    int i = low, j = high;
    while (i <= j) {
        while (arr[i] < p) i++;
        while (arr[j] > p) j--;
        if (i <= j) { std::swap(arr[i], arr[j]); i++; j--; }
    }
    if (low < j) quickFirst(arr, low, j);
    if (i < high) quickFirst(arr, i, high);
}

void quickLast(int arr[], int low, int high) {
    if (low >= high) return;
    int p = arr[high];
    int i = low, j = high;
    while (i <= j) {
        while (arr[i] < p) i++;
        while (arr[j] > p) j--;
        if (i <= j) { std::swap(arr[i], arr[j]); i++; j--; }
    }
    if (low < j) quickLast(arr, low, j);
    if (i < high) quickLast(arr, i, high);
}

void quickMiddle(int arr[], int low, int high) {
    if (low >= high) return;
    int mid = low + (high - low) / 2;
    int p = arr[mid];
    int i = low, j = high;
    while (i <= j) {
        while (arr[i] < p) i++;
        while (arr[j] > p) j--;
        if (i <= j) { std::swap(arr[i], arr[j]); i++; j--; }
    }
    if (low < j) quickMiddle(arr, low, j);
    if (i < high) quickMiddle(arr, i, high);
}

void quickRandom(int arr[], int low, int high) {
    if (low >= high) return;
    int pivotIndex = low + rand() % (high - low + 1);
    int p = arr[pivotIndex];
    int i = low, j = high;
    while (i <= j) {
        while (arr[i] < p) i++;
        while (arr[j] > p) j--;
        if (i <= j) { std::swap(arr[i], arr[j]); i++; j--; }
    }
    if (low < j) quickRandom(arr, low, j);
    if (i < high) quickRandom(arr, i, high);
}

void runTest(int n, std::ofstream &outFile) {
    int *arr = new int[n];
    int *copy = new int[n];

    for (int i = 0; i < n; i++) arr[i] = rand() % n + 1;

    auto measure = [&](void (*func)(int[], int, int), const std::string& variant, const std::string& caseName) {
        std::copy(arr, arr + n, copy);
        auto start = std::chrono::high_resolution_clock::now();
        func(copy, 0, n - 1);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        outFile << variant << "," << n << "," << caseName << "," << duration.count() << "\n";
    };

    measure(quickFirst, "QuickFirst", "Random");
    measure(quickLast, "QuickLast", "Random");
    measure(quickMiddle, "QuickMiddle", "Random");
    measure(quickRandom, "QuickRandom", "Random");

    std::sort(arr, arr + n);
    measure(quickFirst, "QuickFirst", "SortedAsc");
    measure(quickLast, "QuickLast", "SortedAsc");
    measure(quickMiddle, "QuickMiddle", "SortedAsc");
    measure(quickRandom, "QuickRandom", "SortedAsc");

    std::sort(arr, arr + n, std::greater<int>());
    measure(quickFirst, "QuickFirst", "SortedDes");
    measure(quickLast, "QuickLast", "SortedDes");
    measure(quickMiddle, "QuickMiddle", "SortedDes");
    measure(quickRandom, "QuickRandom", "SortedDes");

    delete[] arr;
    delete[] copy;
}

int main() {
    std::ofstream outFile("quicksort_variants_results.csv");

    if (outFile.is_open()) {
        outFile << "Variant,InputSize,Case,TimeSeconds\n";

        std::cout << "Testing QuickSort variants... results saving to quicksort_variants_results.csv" << std::endl;

        runTest(10000, outFile);
        runTest(50000, outFile);
        runTest(100000, outFile);

        outFile.close();
        std::cout << "Tests complete!" << std::endl;
    } else {
        std::cerr << "Unable to create CSV file!" << std::endl;
    }

    return 0;
}
