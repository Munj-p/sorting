#include <cstdlib>
#include <iostream>
#include <chrono>
#include <vector>
#include <fstream> 
void merge(std::vector<int>& array, int low, int mid, int high) {
    std::vector<int> temperory;
    int left = low;
    int right = mid + 1;

    while (left <= mid && right <= high) {
        if (array[left] <= array[right]) {
            temperory.push_back(array[left]);
            left++;
        } else {
            temperory.push_back((array[right]));
            right++;
        }
    }
    while (left <= mid) {
        temperory.push_back(array[left]);
        left++;
    }
    while (right <= high) {
        temperory.push_back(array[right]);
        right++;
    }

    for (int i = 0; i < temperory.size(); i++) {
        array[low + i] = temperory[i];
    }
}

void divide(std::vector<int>& array, int low, int high) {
    if (low >= high) {
        return;
    }
    int mid = low + (high - low) / 2;

    divide(array, low, mid);
    divide(array, mid + 1, high);
    merge(array, low, mid, high);
}

void runtest(int n, std::ofstream &outFile) {
    std::vector<int> array(n);
    for (int i = 0; i < n; i++) {
        array[i] = rand() % n + 1;
    }

    auto start1 = std::chrono::high_resolution_clock::now();
    divide(array, 0, n - 1);
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration1 = end1 - start1;

    auto start2 = std::chrono::high_resolution_clock::now();
    divide(array, 0, n - 1);
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration2 = end2 - start2;

    for (int i = 0; i < n / 2; i++)
        std::swap(array[i], array[n - 1 - i]);

    auto start3 = std::chrono::high_resolution_clock::now();
    divide(array, 0, n - 1);
    auto end3 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration3 = end3 - start3;

    outFile << "MergeSort," << n << ",Random," << duration1.count() << "\n";
    outFile << "MergeSort," << n << ",SortedAsc," << duration2.count() << "\n";
    outFile << "MergeSort," << n << ",WorstCase," << duration3.count() << "\n";
}

int main() {
    std::ofstream mergeFile("merge_results.csv");

    if (mergeFile.is_open()) {
        mergeFile << "Algorithm,InputSize,Case,TimeSeconds\n";

        std::cout << "Running Merge Sort tests... results saving to merge_results.csv" << std::endl;

        runtest(10000, mergeFile);
        runtest(50000, mergeFile);
        runtest(100000, mergeFile);

        mergeFile.close();
        std::cout << "Test complete!" << std::endl;
    } else {
        std::cerr << "Unable to create CSV file!" << std::endl;
    }

    return 0;
}
