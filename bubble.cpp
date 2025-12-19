#include <chrono>
#include <cstdlib>
#include <iostream>
#include <fstream> 
void bubble(int arr[], int n) {
  for (int i = 0; i < n - 1; i++)
    for (int j = 0; j < n - i - 1; j++)
      if (arr[j] > arr[j + 1]) {
        int temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
}

void runTest(int n, std::ofstream &outFile) {
  int *arr = new int[n];

  for (int i = 0; i < n; i++)
    arr[i] = rand() % n + 1;

  auto start1 = std::chrono::high_resolution_clock::now();
  bubble(arr, n);
  auto end1 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration1 = end1 - start1;

  auto start2 = std::chrono::high_resolution_clock::now();
  bubble(arr, n);
  auto end2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration2 = end2 - start2;

  for (int i = 0; i < n / 2; i++) {
    int temp = arr[i];
    arr[i] = arr[n - 1 - i];
    arr[n - 1 - i] = temp;
  }

  auto start3 = std::chrono::high_resolution_clock::now();
  bubble(arr, n);
  auto end3 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration3 = end3 - start3;

  outFile << "BubbleSort," << n << ",Random," << duration1.count() << "\n";
  outFile << "BubbleSort," << n << ",SortedAsc," << duration2.count() << "\n";
  outFile << "BubbleSort," << n << ",WorstCase," << duration3.count() << "\n";

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

    std::cout << "Running Bubble Sort tests and appending to " << fileName << "...\n";
    
    runTest(10000, outFile);
    runTest(50000, outFile);
    runTest(100000, outFile);

    outFile.close();
    std::cout << "Done!" << std::endl;
  } else {
    std::cerr << "Error: Could not open file for writing." << std::endl;
  }

  return 0;
}
