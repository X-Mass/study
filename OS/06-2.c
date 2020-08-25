#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int a = 0;

void *mythread(void *dummy)

/*
 *   Parameter void *dummy is used only for type compatibility, and will not be used
 *   at the function now. At the same reason the return value of function has type 
 *   void *, but it wiil be not used at this program.
 */

{

   pthread_t mythid;
   
   mythid = pthread_self();
   if (*((int*)dummy) == 0) {
      a = a + 1;
   }
   if (*((int*)dummy) == 1) {
      a = a - 1;
   }
   if (*((int*)dummy) == 2) {
      a = a * 2;
   }
   

   printf("Thread %lu, Calculation result = %d\n", mythid, a);

   return NULL;
}

int main()
{
   pthread_t thid[3], mythid; 
   int       result[3];
   int o[3] = {0, 1, 2};
   for (int i = 0; i < 3; i++) {
      result[i] = pthread_create( &thid[i], (pthread_attr_t *)NULL, mythread, &o[i]);
      if(result[i] != 0){
         printf ("Error on thread create, return value = %d\n", result);
         exit(-1);
      }   
      printf("Thread created, thid = %lu\n", thid);
   }

   

   mythid = pthread_self();
   
   a = a+1;
   
   printf("Thread %u, Calculation result = %d\n", mythid, a);
   for (int i = 0; i < 3; i++) {
      pthread_join(thid[i], (void **)NULL);
   }
   pthread_join(thid, (void **)NULL);

   return 0;
}
