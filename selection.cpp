#include <chrono>
#include <cstdlib>
#include <iostream>
#include <fstream> 
void selection(int arr[], int n) {
  for (int i = 0; i < n - 1; i++) {
    int min_idx = i;
    for (int j = i + 1; j < n; j++)
      if (arr[j] < arr[min_idx])
        min_idx = j;
    int temp = arr[i];
    arr[i] = arr[min_idx];
    arr[min_idx] = temp;
  }
}

void runTest(int n, std::ofstream &outFile) {
  int *arr = new int[n];

  for (int i = 0; i < n; i++)
    arr[i] = rand() % n + 1;

  auto start1 = std::chrono::high_resolution_clock::now();
  selection(arr, n);
  auto end1 = std::chrono::high_resolution_clock::now();
  auto duration1 = std::chrono::duration_cast<std::chrono::duration<double>>(end1 - start1);

  auto start2 = std::chrono::high_resolution_clock::now();
  selection(arr, n);
  auto end2 = std::chrono::high_resolution_clock::now();
  auto duration2 = std::chrono::duration_cast<std::chrono::duration<double>>(end2 - start2);

  for (int i = 0; i < n / 2; i++) {
    int temp = arr[i];
    arr[i] = arr[n - 1 - i];
    arr[n - 1 - i] = temp;
  }

  auto start3 = std::chrono::high_resolution_clock::now();
  selection(arr, n);
  auto end3 = std::chrono::high_resolution_clock::now();
  auto duration3 = std::chrono::duration_cast<std::chrono::duration<double>>(end3 - start3);

  outFile << "SelectionSort," << n << ",Random," << duration1.count() << "\n";
  outFile << "SelectionSort," << n << ",SortedAsc," << duration2.count() << "\n";
  outFile << "SelectionSort," << n << ",WorstCase," << duration3.count() << "\n";

  delete[] arr;
}

int main() {
  std::ofstream myFile("sorting_results.csv");

  if (!myFile.is_open()) {
    std::cerr << "Error: Could not create the file!" << std::endl;
    return 1;
  }

  myFile << "Algorithm,InputSize,Case,TimeSeconds\n";

  runTest(10000, myFile);
  runTest(50000, myFile);
  runTest(100000, myFile);

  myFile.close();

  std::cout << "Data has been successfully saved to sorting_results.csv" << std::endl;

  return 0;
}
