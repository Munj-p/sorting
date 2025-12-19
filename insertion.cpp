#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <fstream> 
void insertion(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void runTest(int n, std::ofstream &outFile) {
    int *arr = new int[n];

    for (int i = 0; i < n; i++)
        arr[i] = rand() % n + 1;

    auto start1 = std::chrono::high_resolution_clock::now();
    insertion(arr, n);
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration1 = end1 - start1;

    auto start2 = std::chrono::high_resolution_clock::now();
    insertion(arr, n);
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration2 = end2 - start2;

    for (int i = 0; i < n / 2; i++)
        std::swap(arr[i], arr[n - 1 - i]);

    auto start3 = std::chrono::high_resolution_clock::now();
    insertion(arr, n);
    auto end3 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration3 = end3 - start3;

    outFile << "Insertion," << n << ",Random," << duration1.count() << "\n";
    outFile << "Insertion," << n << ",SortedAsc," << duration2.count() << "\n";
    outFile << "Insertion," << n << ",WorstCase," << duration3.count() << "\n";

    delete[] arr;
}

int main() {
    const std::string filename = "sorting_results.csv";
    
    std::ifstream checkFile(filename);
    bool isEmpty = checkFile.peek() == std::ifstream::traits_type::eof();
    checkFile.close();

    std::ofstream outFile(filename, std::ios::app);

    if (outFile.is_open()) {
        if (isEmpty) {
            outFile << "Algorithm,InputSize,Case,TimeSeconds\n";
        }

        runTest(10000, outFile);
        runTest(50000, outFile);
        runTest(100000, outFile);

        outFile.close();
        std::cout << "Results appended to " << filename << std::endl;
    } else {
        std::cerr << "Error opening file!" << std::endl;
    }

    return 0;
}
