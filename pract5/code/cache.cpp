#include <ctime>
#include <iostream>
#include <x86intrin.h> //for __rdtsc()

/* ==================== CPU PREHEAT ==================== */
void PrintMatrix(int *matrix, size_t sizeMatr) {
  for (int i = 0; i < sizeMatr; i++) {
    for (int j = 0; j < sizeMatr; j++) {
      std::cout << matrix[i * sizeMatr + j] << " ";
      if (i % 5 == 0) {
        std::cout << "WoW I can print a matrix!" << std::endl;
      }
    }
    std::cout << std::endl;
  }
  std::cout << "==================================================="
            << std::endl;
}

void FillMatrix(int *matrix, size_t sizeMatr) {
  srand(time(NULL));
  for (size_t i = 0; i < sizeMatr * sizeMatr; i++) {
    matrix[i] = rand() % 50;
  }
}

void MultMatrixes(int *matrix1, int *matrix2, int *result,
                  size_t sizeMatr) {
  for (int i = 0; i < sizeMatr; i++) {
    for (int k = 0; k < sizeMatr; k++) {
      for (int j = 0; j < sizeMatr; j++) {
        result[i * sizeMatr + j] +=
            matrix1[i * sizeMatr + k] * matrix2[k * sizeMatr + j];
      }
    }
  }
}

void PrepareCPU() {
  size_t sizeMatr = 2500;

  int *m1 = new int[sizeMatr * sizeMatr]();
  FillMatrix(m1, sizeMatr);

  int *m2 = new int[sizeMatr * sizeMatr]();
  FillMatrix(m2, sizeMatr);

  int *resMatr = new int[sizeMatr * sizeMatr]();

  MultMatrixes(m1, m2, resMatr, sizeMatr);
  PrintMatrix(resMatr, sizeMatr / 2);
}

/* ==================== CACHE PREHEAT ==================== */
void PrepareCache(int *array, size_t lengthArr) {
  int elem = 0;
  for (size_t i = 0; i < lengthArr; i++) {
    elem = array[elem];
  }
  if (elem == 25) {
    std::cout << "Hello from 25 :) " << std::endl;
  }
}

/* ================= TYPES TO GO TROUGHT ARRAY ================= */
void GoStraight(int *array, size_t lengthArr) {
  for (size_t i = 0; i < lengthArr - 1; ++i) {
    array[i] = i + 1;
  }
  array[lengthArr - 1] = 0;
}

void GoReverse(int *array, size_t lengthArr) {
  for (size_t i = lengthArr - 1; i > 0; --i) {
    array[i] = i - 1;
  }
  array[0] = lengthArr - 1;
}

void GoRandom(int *array, size_t lengthArr) {
  srand(time(nullptr));
  bool *used = new bool[lengthArr];
  for (size_t i = 0; i < lengthArr; i++) {
    used[i] = false;
  }
  int j = 0;
  used[0] = true;
  for (size_t i = 0; i < lengthArr - 1; i++) {
    int tmp;
    while (used[tmp = rand() % lengthArr] != false)
      ;

    array[j] = tmp;
    used[tmp] = true;
    j = tmp;
  }
  array[j] = 0;

  delete[] used;
}

/* ================= CHECK CORRECTNESS OF TYPES ================= */
void PrintArray(int *array, size_t lengthArr) {
  for (size_t i = 0; i < lengthArr; i++) {
    std::cout << array[i] << " ";
  }
  std::cout << std::endl;
}

void CheckCorrectCalculating() {
  size_t size = 20;
  std::cout << "size : " << size << std::endl;
  int *arr = new int[size]();

  GoStraight(arr, size);
  std::cout << "straight: ";
  PrintArray(arr, size);

  GoReverse(arr, size);
  std::cout << "reversed: ";
  PrintArray(arr, size);

  GoRandom(arr, size);
  std::cout << "random:   ";
  PrintArray(arr, size);
}

/* =============== COUNT TIME PER ELEMENT IN TICKS =============== */
double CountTimeInTicks(int *array, size_t lengthArr) {
  PrepareCache(array, lengthArr);

  size_t numberOfBypass = 100;

  double time;

  int elem = 0;
  for (int j = 0; j < numberOfBypass; j++) {
    double start = __rdtsc();
    for (int i = 0; i < lengthArr; i++) {
      elem = array[elem];
      if (elem == (lengthArr + 10))
        std::cout << "WooW, I'm out of bounds! =)" << std::endl;
    }
    double end = __rdtsc();

    time = end - start;
  }
  return (time / lengthArr);
}

int main() {
  CheckCorrectCalculating();
  PrepareCPU();

  std::cout << " ======= Straight bypass ======= " << std::endl;
  for (size_t sizeArr = 256; sizeArr < 256 * 32 * 1024;
       sizeArr *= 1.2) {
    std::cout << "Straight: ";
    std::cout << "Number: " << sizeArr << " ";
    int *array = new int[sizeArr];

    GoStraight(array, sizeArr);
    double time = CountTimeInTicks(array, sizeArr);
    std::cout << "Ticks: " << time << std::endl;
    delete[] array;
  }
  std::cout << std::endl << std::endl;

  std::cout << " ======= Reversed bypass ======= " << std::endl;
  for (size_t sizeArr = 256; sizeArr < 256 * 32 * 1024;
       sizeArr *= 1.2) {
    std::cout << "Reverse: ";
    std::cout << "Number: " << sizeArr << " ";
    int *array = new int[sizeArr];

    GoReverse(array, sizeArr);
    double time = CountTimeInTicks(array, sizeArr);
    std::cout << "Ticks: " << time << std::endl;
    delete[] array;
  }
  std::cout << std::endl << std::endl;

  std::cout << " ======= Random bypass ======= " << std::endl;
  for (size_t sizeArr = 256; sizeArr < 256 * 32 * 1024;
       sizeArr *= 1.2) {
    std::cout << "Random: ";
    std::cout << "Number: " << sizeArr << " ";
    int *array = new int[sizeArr];

    GoRandom(array, sizeArr);
    double time = CountTimeInTicks(array, sizeArr);

    std::cout << "Ticks: " << time << std::endl;
    delete[] array;
  }
  return 0;
}
