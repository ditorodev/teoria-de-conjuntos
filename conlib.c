#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "conlib.h"

Conjunto * cn_copy(Conjunto src, Conjunto *dest) {
  int i = 0;

  if(dest) free(dest);
   
  dest = (Conjunto *) malloc(sizeof(Conjunto)); 
  dest->value = 0;
  dest->size = src.size;
  dest->p_elements = (int *) malloc(sizeof(int) * (src.size));
  for(i=0; i<src.size; i++) dest->p_elements[i] = src.p_elements[i];
  strcpy(dest->name, "Copia de ");
  strcat(dest->name, src.name);
  
  return dest;
}

void imprimirConjunto(Conjunto *c, Elemento *universo) {
  int i = 0;
  
  printf("\n%i .- %s(%i) : ", c->value, c->name, c->size);
  for (i =0; i<c->size; i++) {
    printf("%s", universo[c->p_elements[i]].name);
    if (i<(c->size-1)) printf(" - ");
  }
}
  
/*void bigandlow (Conjunto *c1, Conjunto *c2, Conjunto *big, Conjunto *low) {
  if(c1->size > c2->size) {
    big = c1;
    low = c2;  
    return;
  }
  
  big = c2;
  low = c1;
  //printf("big %s and low %s", big->name, low->name);
}*/

Conjunto *cn_union (Conjunto c1, Conjunto c2, Conjunto *p_cRes){
  Conjunto *bigC, *lowC;
  int i, j, flag = 0, cont = 0;

  // limpiamos la variable donde alojaremos el resultado
  if(p_cRes) {
    free(p_cRes);
  }
  p_cRes = (Conjunto *) malloc(sizeof(Conjunto));
  // determinamos el mayor de ambos
  if(c1.size > c2.size) {
    bigC = &c1;
    lowC = &c2;  
  }
 else { 
  bigC = &c2;
  lowC = &c1;
  }
  // si alguno es nulo devolvemos a el que no sea nulo
  if(!c1.p_elements){
    cn_copy(c2, p_cRes);
    return p_cRes; 
  }
  if(!c2.p_elements){
    cn_copy(c1, p_cRes);
    return p_cRes; 
  }
 
  for(i=0; i<lowC->size; i++) {
    for(j=0; j<bigC->size; j++) {
      if(lowC->p_elements[i] == bigC->p_elements[j]) {
        cont++;
        break;
      }
    }
  }
  p_cRes->size = bigC->size + lowC->size - cont;
  cont = 0;
  p_cRes->p_elements = (int *) malloc(sizeof(int) * (p_cRes->size));
  // anadimos todos los elementos del conjunto mas grande
  for (i = 0; i<bigC->size; i++) p_cRes->p_elements[cont++] = bigC->p_elements[i];
  // ahora anadimos los del conjunto menor que no se encuentre en el conjunto mas grande
  for(i=0; i<lowC->size; i++) {
   flag = 0;
   j = 0;
   // verificamos que el elemento de este conjunto no este presente en el otro
    while(j<bigC->size && !flag) {
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

Conjunto *cn_interseccion(Conjunto c1, Conjunto c2, Conjunto *p_cRes) {
  Conjunto *bigC, *lowC;
  int i,j, cont = 0;

  // limpiamos la variable donde estara nuestro conjunto respuesta
  if(p_cRes) {
    free(p_cRes);
  }
  p_cRes = (Conjunto *) malloc(sizeof(Conjunto));
  
   // determinamos el mayor de ambos
  if(c1.size > c2.size) {
    bigC = &c1;
    lowC = &c2;  
  }
  else { 
    bigC = &c2;
    lowC = &c1;
  }
  

  // si alguno es nulo devolvemos a el que no sea nulo
  if(!c1.p_elements){
    cn_copy(c2, p_cRes);
    return p_cRes; 
  }
  if(!c2.p_elements){
    cn_copy(c1, p_cRes);
    return p_cRes; 
  }

  for(i=0; i<lowC->size; i++) {
    for(j=0; j<bigC->size; j++) {
      if(lowC->p_elements[i] == bigC->p_elements[j]){ 
      cont++;
      break;
      }
    }
  }
  
  p_cRes->size = cont;
  cont = 0;

   for(i=0; i<lowC->size; i++) {
    for(j=0; j<bigC->size; j++) {
      if(lowC->p_elements[i] == bigC->p_elements[j]){ 
      p_cRes->p_elements[cont++] = lowC->p_elements[i];
      break;
      }
    }
  }
  strcpy(p_cRes->name, "Interseccion de ");
  strcat(p_cRes->name, c1.name);
  strcat(p_cRes->name, " y ");
  strcat(p_cRes->name, c2.name);

  return p_cRes;
}

Conjunto *cn_comp(Conjunto c, Elemento *universo, int uni_sz, Conjunto *p_cRes){
  int i, flag = 0, j, cont = 0;
 
  // limpiamos la variable donde estara nuestro conjunto respuesta
  if(p_cRes) {
    free(p_cRes);
  }
  p_cRes = (Conjunto *) malloc(sizeof(Conjunto));
  
  if(!c.p_elements) {
    p_cRes->size = uni_sz;
  } else {
    p_cRes->size = uni_sz - c.size;
  }
  p_cRes->p_elements = (int *)malloc(sizeof(int) * p_cRes->size);

  for(i=0; i<uni_sz; i++){
    j = 0;
    flag = 0;
    if(!c.p_elements){
      p_cRes->p_elements[i] = i;
      continue;
    }
    while(j<p_cRes->size && !flag) {
      if(i == c.p_elements[j++]) {
        flag = 1;
      }
    }
    if(!flag) p_cRes->p_elements[cont++] = i;
  }
  return p_cRes;
}
