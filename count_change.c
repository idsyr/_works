#include <stdio.h>

const int coins[] = {1, 5, 10, 25, 50};
#define coins_count 5

int without(const int dptable[coins_count][202], const int coin, const int i) {
   return dptable[coin - 1][i];
}

int subtracted(const int dptable[coins_count][202], const int coin, const int i) {
   return dptable[coin][i - coins[coin]];
}

int count_change(int n) {
   int dptable[coins_count][2 * (100 + 1)] = {0};
   for(int c = 0; c < coins_count; ++c) { dptable[c][101] = 1;   }
   for(int i = 102; i < 202; ++i)       { dptable[0][i] = 1;     }

   for(int c = 1; c < coins_count; ++c) {
      for(int i = 102; i < 202; ++i) {
         dptable[c][i] = without(dptable, c, i) + subtracted(dptable, c, i);  
      }
   }

   return dptable[coins_count - 1][n + 101];
}

int main () {
   printf("count_change: %d", count_change(100));
}
