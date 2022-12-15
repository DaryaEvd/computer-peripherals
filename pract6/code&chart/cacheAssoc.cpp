#include <climits>
#include <iostream>
#include <x86intrin.h>

void GoThroughArray(size_t *array, size_t offset,
                    size_t amountFrag) {
  for (size_t idx = 0; idx < offset; idx++) {
    for (size_t frag = 0; frag < amountFrag - 1; frag++) {
        array[frag * offset + idx] = (frag + 1) * offset + idx;
    }
    array[(amountFrag - 1) * offset + idx] = (idx + 1) % offset;
  }
}

size_t CountTimeInTics(size_t *array, size_t offset) {
  const size_t amountBypass = 100;
  size_t elem = 0;
  
  size_t start = __rdtsc();
  for (size_t bypass = 0; bypass < amountBypass * offset; bypass++) {
    elem = array[elem];
  }
  size_t end = __rdtsc();
  if (elem == -1) {
    std::cout << "WoW! I am out of bounds!";
  }  

  size_t tmpTime = end - start;
  size_t minTime = INT32_MAX;

  minTime = minTime > tmpTime ? tmpTime : minTime;

  return minTime / (amountBypass * offset);
}

int main() {    
  const size_t maxNumOfFrags = 32;

  const int L1_CACHE_SIZE = 32768;
  const int coeffL1 = 8;  //offset should be divisible by bank size
                        //coeff should be pow of 2
  const size_t offsetL1 = L1_CACHE_SIZE / coeffL1; 
  size_t *arrayL1 = new size_t[maxNumOfFrags * offsetL1]();
  for (size_t fragm = 1; fragm <= maxNumOfFrags; fragm++) {
    std::cout << "L1 ";
    std::cout << "fragment " << fragm << " ";

    GoThroughArray(arrayL1, offsetL1, fragm);
    std::cout << CountTimeInTics(arrayL1, offsetL1 * fragm) << " tics"
              << std::endl;
  }
  delete [] arrayL1;

  std::cout << "=============================" <<std::endl << std::endl;
  
  const size_t L2_BANK_SIZE = 262144;
  const int coeffL2 = 16;
  const size_t offsetL2 = L2_BANK_SIZE / coeffL2;
  size_t *arrayL2 = new size_t[maxNumOfFrags * offsetL2]();
  for (size_t fragm = 1; fragm <= maxNumOfFrags; fragm++) {
    std::cout << "L2 ";
    std::cout << "fragment " << fragm << " ";
 
    GoThroughArray(arrayL2, offsetL2, fragm);
    std::cout << CountTimeInTics(arrayL2, offsetL2  * fragm) << " tics"
              << std::endl;
  }
  delete [] arrayL2;

  std::cout << "=============================" <<std::endl << std::endl;
  
  //CODE BELOW DOESN'T COUNT L3 ASSOCIATIVITY
  /*
  const size_t L3_BANK_SIZE = 3145728;
  const size_t offsetL3 = L3_BANK_SIZE / 1024 / 1024 / 8;
  size_t *arrayL3 = new size_t[maxNumOfFrags * L3_BANK_SIZE]();
  for (size_t fragm = 1; fragm <= maxNumOfFrags; fragm++) {
     std::cout << "L3 ";
    std::cout << "fragment " << fragm << " ";
    GoThroughArray(arrayL3, offsetL3, fragm);

    std::cout << CountTimeInTics(arrayL3, offsetL3 * fragm) << " tics"
              << std::endl;
  }
  delete [] arrayL3;
  */
}
