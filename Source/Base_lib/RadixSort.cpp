#include "sortlist.hpp"
#include <malloc.h>

static dword temp_counters[256*4];

//----------------------------

#define MAX_STACK_ALLOC 0x8000

//----------------------------

#pragma warning(disable:4731)

//----------------------------
void __cdecl RadixSort32(S_sort_entry *data, int count1){

   dword count = count1;
   if(!count)
      return;
   dword index[256];

   S_sort_entry* temp;
   if (count1 * sizeof(S_sort_entry) <= MAX_STACK_ALLOC)
      temp = (S_sort_entry*)alloca(count1 * sizeof(S_sort_entry));
   else
      temp = new S_sort_entry[count];

   S_sort_entry* src = data;
   S_sort_entry* dst = temp;

   dword byte_index, i, byte_max = 8 * sizeof(dword);

   for (byte_index = 0; byte_index < byte_max; byte_index += 8){
      dword offsets[256] = { 0 };
      dword total = 0;
      // 1st pass - count how many of each key
      for (i = 0; i < count; i++){
         dword sort_value = src[i].sort_value;
         dword radix = (sort_value >> byte_index) & 0xFF;
         offsets[radix]++;
      }
      // 2nd pass - sum up the offsets
      for (i = 0; i < sizeof(offsets) / sizeof(offsets[0]); i++) {
         dword cnt = offsets[i];
         offsets[i] = total;
         total += cnt;
      }
      // 3rd pass - copy the data
      for (i = 0; i < count; i++){
         dword sort_value = src[i].sort_value;
         dword radix = (sort_value >> byte_index) & 0xFF;
         dst[offsets[radix]++] = src[i];
      }
      // swap src and dst
      {
         S_sort_entry* tmp = src;
         src = dst;
         dst = tmp;
      }
   }
                              //make sure that data end up back in user's memory
   if(src!=data)
      memcpy(data, temp, sizeof(S_sort_entry)*count);
   if(count1*sizeof(S_sort_entry) > MAX_STACK_ALLOC)
   delete[] temp;
}

//--------------------------------