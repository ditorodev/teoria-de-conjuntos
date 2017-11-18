#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cnlib.h"

void cn_copy(Conjunto src, Conjunto *dest) {
  int i;
  
  dest->value = 0;
  dest->size = src.size;
  dest->p_elements = (int *) malloc(sizeof(int) * dest->size);
  for(i=0; i<src.size; i++) dest->p_elements[i] = src.p_elements[i];
  strcpy(dest->name, "Copia de ");
  strcat(dest->name, src.name);
  
}

void imprimirConjunto(Conjunto c, Elemento *universo) {
  int i = 0;
  
  printf("%i .- %s(%i) : ", c.value, c.name, c.size);
  for (i =0; i<c.size; i++) {
    printf("%s", universo[c.p_elements[i]].name);
    if (i<size-1) printf(" - ");
  }
}
  
void bigandlow (Conjunto c1, Conjunto c2, Conjunto *big, Conjunto *low) {
  int aux;

  if(c1.size > c2.size) {
    big = &c1;
    low = &c2;  
    return;
  }
  
  big = &c2;
  low = &c1;
}

Conjunto *cn_union (Conjunto c1, Conjunto c2, Conjunto *p_cRes){
  Conjunto auxC, *bigC, *lowC;
  
  if(!c1.p_elements){
    cn_copy(c2, p_cRes);
    return p_cRes; 
  }
  if(!c2.p_elements){
    cn_copy(c1, p_cRes);
    return p_cRes; 
  }
  
  bigandlow(c1, c2, bigC, lowC);
  return p_cRes;
}
