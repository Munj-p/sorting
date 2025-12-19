#include <cstdlib>
#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <fstream> 

void merge(std::vector<int>& array, int low, int mid, int high) {
    std::vector<int> temp;
    int left = low;
    int right = mid + 1;
    while (left <= mid && right <= high) {
        if (array[left] <= array[right]) { temp.push_back(array[left]); left++; }
        else { temp.push_back(array[right]); right++; }
    }
    while (left <= mid) { temp.push_back(array[left]); left++; }
    while (right <= high) { temp.push_back(array[right]); right++; }
    for (int i = 0; i < (int)temp.size(); i++) { array[low + i] = temp[i]; }
}

void divide(std::vector<int>& array, int low, int high) {
    if (low >= high) return;
    int mid = low + (high - low) / 2;
    divide(array, low, mid);
    divide(array, mid + 1, high);
    merge(array, low, mid, high);
}

std::vector<int> mergeArrays(const std::vector<int>& a, const std::vector<int>& b) {
    std::vector<int> result;
    int i = 0, j = 0;
    while (i < (int)a.size() && j < (int)b.size()) {
        if (a[i] <= b[j]) { result.push_back(a[i]); i++; }
        else { result.push_back(b[j]); j++; }
    }
    while (i < (int)a.size()) { result.push_back(a[i]); i++; }
    while (j < (int)b.size()) { result.push_back(b[j]); j++; }
    return result;
}

double runMergeSort(std::vector<int>& array, int chunkSize) {
    auto start = std::chrono::high_resolution_clock::now();
    int n = array.size();
    int numChunks = (n + chunkSize - 1) / chunkSize;
    std::vector<std::vector<int>> chunks;

    for (int i = 0; i < numChunks; i++) {
        int startIdx = i * chunkSize;
        int endIdx = std::min(startIdx + chunkSize, n);
        if (startIdx >= n) break;
        std::vector<int> chunk(array.begin() + startIdx, array.begin() + endIdx);
        divide(chunk, 0, chunk.size() - 1);
        chunks.push_back(chunk);
    }

    while (chunks.size() > 1) {
        std::vector<std::vector<int>> newChunks;
        for (size_t i = 0; i < chunks.size(); i += 2) {
            if (i + 1 < chunks.size()) {
                newChunks.push_back(mergeArrays(chunks[i], chunks[i + 1]));
            } else {
                newChunks.push_back(chunks[i]);
            }
        }
        chunks = std::move(newChunks);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return duration.count();
}

void runtest(int n, int chunkSize, std::ofstream &outFile) {
    std::vector<int> array(n);

    for (int i = 0; i < n; i++) array[i] = rand() % n + 1;
    double t1 = runMergeSort(array, chunkSize);

    std::sort(array.begin(), array.end());
    double t2 = runMergeSort(array, chunkSize);

    std::sort(array.begin(), array.end(), std::greater<int>());
    double t3 = runMergeSort(array, chunkSize);

    outFile << "ExternalMerge," << n << "," << chunkSize << ",Random," << t1 << "\n";
    outFile << "ExternalMerge," << n << "," << chunkSize << ",SortedAsc," << t2 << "\n";
    outFile << "ExternalMerge," << n << "," << chunkSize << ",SortedDes," << t3 << "\n";
}

int main() {
    std::ofstream outFile("external_merge_results.csv");

    if (outFile.is_open()) {
        outFile << "Algorithm,InputSize,ChunkSize,Case,TimeSeconds\n";

        std::vector<int> chunkSizes = {5, 100, 1000, 2000, 5000};
        std::vector<int> inputs = {10000, 50000, 100000};

        std::cout << "Running tests... please wait." << std::endl;

        for (int n : inputs) {
            for (int cs : chunkSizes) {
                runtest(n, cs, outFile);
            }
        }

        outFile.close();
        std::cout << "Data saved to external_merge_results.csv" << std::endl;
    } else {
        std::cerr << "Error: Could not open file for writing!" << std::endl;
    }

    return 0;
}
