#ifndef CNLIB_H
#define CNLIB_H
  typedef struct {
    char name[20];
    int index;
  } Elemento;

  typedef struct {
    char name[100];
    int value;
    int size;
    int *p_elements;
  } Conjunto;

  Conjunto *cn_copy(Conjunto src, Conjunto *dest);
  void imprimirConjunto (Conjunto *c, Elemento *universo);
  
  Conjunto *cn_union (Conjunto c1, Conjunto c2, Conjunto *p_cRes);
  Conjunto *cn_interseccion (Conjunto c1, Conjunto c2, Conjunto *p_cRes);
  Conjunto *cn_comp (Conjunto c, Elemento *universo, int uni_sz, Conjunto *p_cRes);

#endif
