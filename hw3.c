#include <stdio.h>
#include "input.c" // Datei einbinden. Variablien mit Strg+Leerzeichen anzeigen lassen (geany kann das)

int main()
{
  //TODO Unterschied zwischen links und rechts shifting  
  int x = 5;
  int y = 1; //--> n (gibt an um wie viele stellen geshfitet wird)
  printf("%d\n", x << y);  // nach links   * 2^n --> in binär --> 101 --> 1010 (1 NUll) --> 10
  printf("%d",   x >> y);  // nach rechts  / 2^n --> in binär --> 101 --> 10            --> 2 weil int ist ganze zahlen, ansonsten 2.5
   
   
  //TODO eigentlicher Programmstart: Erstellung einer Maske
  int mask = 0;
  int ip_address = 0;
  
  int i;
  for(i = 0; i < 32; i++)
  {
    if(i < suffix)
    {
      mask = mask | 1;          // schreibe eine 1 und shifte eines nach links
      mask = mask << 1;         // andere schreibweise: mask <<= mask;
    }
    else
    {
      mask = mask << 1;         // sonst schreibe eine 0, passiert autom. beim linksshift
    }
  }
  
  //TODO richtig verbinden!
  //printf("\n%d\n", mask);
  ip_address = ip_address_2 & mask;
  //printf("%d\n", ip_address_netzwerk);
  mask =~ mask; //NOT: mask = mask ^ mask;
  ip_address = ip_address | (ip_address_1 & mask); 
  
  
  //TODO Ausgabe.
  //1111 0010 | 0010 0010 | 0010 0010 | 0010 0010
  printf( "New IP address: %d.%d.%d.%d/%d", (ip_address>>24),
                                            (ip_address<<8)>>24,
                                            (ip_address<<16)>>24, 
                                            (ip_address<<24)>>24,
                                            suffix );
  
  return 0;
} 
