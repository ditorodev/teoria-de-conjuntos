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
  int i, j, flag = 0, cont = 0;

  // limpiamos la variable donde alojaremos el resultado
  if(p_cRes) {
    free(p_cRes);
  }
  p_cRes = (Conjunto *) malloc(sizeof(Conjunto));
  
  // si alguno es nulo devolvemos a el que no sea nulo
  if(!c1.p_elements){
    cn_copy(c2, p_cRes);
    return p_cRes; 
  }
  if(!c2.p_elements){
    cn_copy(c1, p_cRes);
    return p_cRes; 
  }
 
  bigandlow(c1, c2, bigC, lowC);
  for(i=0; i<lowC->size; i++) {
    for(j=0; i<bigC->size; j++) {
      if(lowC.p_element[i] == bigC.p_elements[j]) {
        cont++;
        break;
      }
    }
  }
  p_cRes->size = bigC->size + lowC->size - cont;
  cont = 0;
  p_cRes->p_elements = (int *) malloc(sizeof(int) * p_cRes->size);
  // anadimos todos los elementos del conjunto mas grande
  for (i = 0; i<bigC->size; i++) p_cRes->p_elements[cont++] = bigC->p_elements[i];
  // ahora anadimos los del conjunto menor que no se encuentre en el conjunto mas grande
  for(i=0; i<lowC->size; i++) {
   flag = 0;
   j = 0;
   // verificamos que el elemento de este conjunto no este presente en el otro
    while(!flag &&  j<bigC->size) {
      if(lowC->p_elements[i] == bigC->p_elements[j++]) flag = 1;
    }
  // si este elemento no se encuentra en el otro conjunto procedemos a anadirlo al conjunto resultante
  if(!flag) p_cRes->p_elements[cont++] = lowC->p_elements[i];
  }
  
  strcpy(p_cRes->name, "Union de ");
  strcat(p_cRes->name, c1.name);
  strcat(p_cRes->name, " y ");
  strcat(p_cRes->name, c2.name);

  return p_cRes;
}
