// ----------------------------------------
// input format: N, a0, a1, ... an
// ----------------------------------------
// gcc sorts.c -o sorts.x -DINSERTION_SORT
// gcc sorts.c -o sorts.x -DSELECTION_SORT
// ----------------------------------------

#include <stdlib.h>
#include <stdio.h>

int  defined_op (int * arr, int arr_sz);
void print_arr  (const int * arr, const int arr_sz);
void read_input (int ** arr, int * arr_sz_ptr);


int main ()
{  int * arr;
   int arr_sz;
   read_input(&arr, &arr_sz);
   defined_op(arr, arr_sz);
   print_arr(arr, arr_sz);
   free(arr);
}


void print_arr (const int * arr, const int arr_sz)
{  int i = 0;
   for (; i < arr_sz; ++i) { printf("%d", arr[i]); }
   printf("\n");
}
 

void read_input (int ** arr, int * arr_sz_ptr)
{  int scanf_n;
   int i = 0;
   int arr_sz;

   scanf_n = scanf("%d", arr_sz_ptr);
   if (scanf_n != 1 || *arr_sz_ptr < 0)
   {  printf("ERROR: invalid input\n _N must be int, _N must be >0");
      abort();
   }

   arr_sz = *arr_sz_ptr;
   *arr = calloc(arr_sz, sizeof(int));

   for (; i<arr_sz; ++i)
   {  scanf_n = scanf("%d", (*arr + i));
      if (scanf_n != 1)
      {  printf("ERROR: invalid input\n _Am must be int");
         free(arr);
         abort();
      }
   }
}



#ifdef INSERTION_SORT

static int shift (int * arr, int a_pos, int b_pos)
{  for (int i = b_pos; i > a_pos; --i)
   { arr[i] = arr[i-1]; }
}


static int insert_in_sorted (int * arr, int arr_sz, int el_pos)
{  int el = arr[el_pos];
   for (int i = 0; i<arr_sz; ++i)
   {  if (el < arr[i])
      {  shift(arr, i, el_pos);
         arr[i] = el;
         return 0;
      }
  }
}


int defined_op (int * arr, int arr_sz)
{  --arr_sz;
   for(int i = 0; i<arr_sz; ++i)
   {  if(arr[i] > arr[i+1])
      { insert_in_sorted(arr, arr_sz, i+1); }
   }
   return 1;
}

#endif



#ifdef SELECTION_SORT

static int swap_in_arr (int * arr, int pos_a, int pos_b)
{  int tmp = arr[pos_a];
   arr[pos_a] = arr[pos_b];
   arr[pos_b] = tmp;
   return 1;
}


static int pos_min (const int * arr, int start, int end)
{  int pos_min = start;
   int val_min = arr[start];
   for(int i = start; i<end; ++i)
   {  if(arr[i] < val_min)
      {  pos_min = i;
         val_min = arr[i];
      }
   }
   return pos_min;
}


int defined_op (int * arr, int arr_sz)
{  for(int i = 0; i<arr_sz; ++i)
   { swap_in_arr(arr, i, pos_min(arr, i, arr_sz)); }
   return 1;
}

#endif
