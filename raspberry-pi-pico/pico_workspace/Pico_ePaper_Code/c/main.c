#include <stdio.h>
#include "pico/stdlib.h"
#include "EPD_Test.h"


//int main() {
  //  stdio_init_all();
 //   while (true) {
  //      printf("Hello, world!\n");
 //       sleep_ms(1000);
 //   }
//    return 0;
///}

int main(void)
{
    stdio_init_all();
    sleep_ms(1000);
    printf("Starting up!\n");
	sleep_ms(1000); 
    myexmp();
   return 0;
}
