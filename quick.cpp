#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <fstream> 
void quick(int arr[], int low, int high) {
  if (low >= high)
    return;
  int mid = low + (high - low) / 2;
  int p = arr[mid];

  int i = low;
  int j = high;

  while (i <= j) {
    while (arr[i] < p)
      i++;
    while (arr[j] > p)
      j--;

    if (i <= j) {
      std::swap(arr[i], arr[j]);
      i++;
      j--;
    }
  }

  if (low < j)
    quick(arr, low, j);
  if (i < high)
    quick(arr, i, high);
}

void runTest(int n, std::ofstream &outFile) {
  int *arr = new int[n];

  for (int i = 0; i < n; i++)
    arr[i] = rand() % n + 1;

  auto start1 = std::chrono::high_resolution_clock::now();
  quick(arr, 0, n - 1);
  auto end1 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration1 = end1 - start1;

  auto start2 = std::chrono::high_resolution_clock::now();
  quick(arr, 0, n - 1);
  auto end2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration2 = end2 - start2;

  for (int i = 0; i < n / 2; i++)
    std::swap(arr[i], arr[n - 1 - i]);

  auto start3 = std::chrono::high_resolution_clock::now();
  quick(arr, 0, n - 1);
  auto end3 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration3 = end3 - start3;

  outFile << "Quick," << n << ",Random," << duration1.count() << "\n";
  outFile << "Quick," << n << ",SortedAsc," << duration2.count() << "\n";
  outFile << "Quick," << n << ",WorstCase," << duration3.count() << "\n";

  delete[] arr;
}

int main() {
  const std::string fileName = "sorting_results.csv";

  std::ifstream inFile(fileName);
  bool isFileEmpty = inFile.peek() == std::ifstream::traits_type::eof();
  inFile.close();

  std::ofstream outFile(fileName, std::ios::app);

  if (outFile.is_open()) {
    if (isFileEmpty) {
      outFile << "Algorithm,InputSize,Case,TimeSeconds\n";
    }

    std::cout << "Appending Quick Sort results to " << fileName << "..." << std::endl;

    runTest(10000, outFile);
    runTest(50000, outFile);
    runTest(100000, outFile);

    outFile.close();
    std::cout << "Completed successfully." << std::endl;
  } else {
    std::cerr << "Error: Unable to open file!" << std::endl;
  }

  return 0;
}
