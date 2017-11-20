#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conlib.h"
#include "pila.h"

//---------------------------------------------------------------------DEFINE
#define separador "-"
#define true 1
#define false 0



int getSize (char *str) {
  int i=0, cont=0;
  
  while(str[i] != '\0') {
    if(str[i] == '-') cont++;
    i++;
  }
  
  return cont+1;
}
  
void strip (char *input, char ch){     
  int loop;
  char output[strlen(input)];
  char *dest = output;
 
  if (output){
    for (loop=0; loop<strlen(input); loop++)
      if (input[loop] != ch) *dest++ = input[loop];
      *dest = '\0';
     }
  
    strcpy(input, output); 
 }

int whosThis(char *str, Conjunto *conj, int cn_sz){
  int i, flag = 0;  
  
  for(i=0; i<cn_sz; i++) {
    if(!(strcmp(conj[i].name, str))) {
      flag = 1;
      break;
    }
  }
  if (!flag) return -1;
  return i;
}

char *cn_polaca(char *str, Conjunto *conj, int cn_sz, char *dest){
  int i,j,k,lastOp = 0;
  char s[] = "()+x-";
  pila_t pila, *stack;
  char output[50];
  char op;
  char buffer[20],*p;
  strcpy(buffer, str);
  i = j = k = 0;
  stack = &pila; 
  anula(stack);
  p = strtok(buffer, s);
  printf("\n %s \n", str);
  while(p) {
    j=0;
    
    printf("\n%i lastop %i\n", i, lastOp);
    if(whosThis(p, conj, cn_sz) == -1) {
      printf("ERROR UNO DE LOS CONJUNTOS ES INVALIDO O NO EXISTE");
      return NULL;
    }

    if(str[lastOp] == '(' || str[lastOp] == 'x' || str[lastOp] == '+' || str[lastOp] == '-' || str[lastOp] == ')') {
      printf("\n lei %c \n", str[lastOp]);
      // BASICAMENTE SI LEO UN + Y TENIA UNA x SACO TODAS LAS x HASTA QUE HAYA OTRO +, TAMBIEN SI LEO UN - Y HABIA UN + O UN x SACO TODOS LOS OPERADORESHASTA QUE HAYA UN - O NO HAYA NADA PS
      printf("\n dest %s\n", dest);  
      if(vacia(stack)) { 
          printf("pila vacia\n"); 
          push(stack, str[lastOp]);
          top(stack, &op);
          printf("op %c", op);
        }
        else {
          // si el operador en el tope de la pila es una x y lo que lei es un + saco todas las equis hasta llegar a un -, una x o que la pila se vacie
          top(stack, &op);
          printf("\ntope en el else %c", op);
          if ( top(stack, &op) && op == 'x'  && str[lastOp] == '+') {  
            while(top(stack, &op) && (op == 'x' || op == '-') ) {// mientrasse pueda  y en el tope hayan elementos de mayor precedencia los iremos sacando ejemplo cuando haya x-- y leamos + sacaremos --x y dejaremos +
              pop(stack, &op);
              if(op == 'x') strcat(dest, "x");
              else strcat(dest, "-");
              strcat(dest, "|");
              printf("anadi %c al string", str[lastOp]);
              //strcat(dest, "x");
            }
            // ya que sacamos todos los de mayor precedencia, metemos el de menor precedencia
            push(stack, str[lastOp]); 
          } // si el operador en el tope de la pila es u - y el que leemos es + o x, debemos sacar todos los -
            else if(op == '-' && (str[lastOp] == '+' || str[lastOp] == 'x')) {
           //top(stack, &op); 
           while( top(stack, &op) && op == '-') {
              pop(stack, &op);
              strcat(dest, "-");
              strcat(dest, "|");
            }
          push(stack, str[lastOp]); 
          }

          // si leo un parentesis que cierra debo vaciar todo hasta encontrar el parentesis que abre en la pila
          else if(str[lastOp] == ')') {
            printf("\nDEBERIAS VACIAR LA PILA HASTA ENCONTRAR (\n");
            while(top(stack, &op) && op != '(') {
              pop(stack, &op);
              //if(op == '(') break;
              if(op == 'x') strcat(dest, "x");
              else if(op == '+') strcat(dest, "+");
              else if (op == '-')strcat(dest, "-");
              printf("\n anadimos %c al string \n", str[lastOp]);
              strcat(dest, "|");
            }
          }
           else{
           push(stack, str[lastOp]);
          }    
        } 
        lastOp++;
      }
      else if((j = whosThis(p, conj, cn_sz)) > -1){
      strcat(dest, conj[j].name);
      strcat(dest , "|");
      p = strtok(NULL, s);
      i = lastOp;
      while( str[i] != '\0' ){
      //if(str[i] == '(') {
        //  printf("\n\n\nENCONTRE UN (\n\n\n") ;
      //}
        if(str[i] == 'x' || str[i] == '+' || str[i] == '-' || str[i] == '(' || str[i] == ')' ) { 
        break;  
      }
        printf("\n%c\n", str[i]);
        i++;
      }
      lastOp = i;
    } 
    printf ("\n lastop esta en %c \n", str[lastOp]);
    printf("\n---------------------------------------\n");
  }

  while(!vacia(stack)){
    pop(stack, &op);
    if(op == 'x') strcat(dest, "x");
    else if(op == '+') strcat(dest, "+");
    else if (op == '-') strcat(dest, "-");

    }
  
  printf("\n%s \nen notacion polaca\n %s\n", str, dest);
  return dest;
}
int main(){

//-------------------------------------------------------VARIABLES
    FILE *fd;
    int i=0, k = 0, cn_size = 0, sz_uni = 0, cont=0, j=0, m=0;
    char aux[10000], archivo[100];   
    char *p, *buffer, auxStr[600];
    char input[] = "conjuntoA+conjuntoBx(conjuntoC+-conjuntoA)xconjuntoB";
    Conjunto conj[1000], *auxC;
    Elemento universo[100];
    p=NULL;
       

//-------------------------------------------------------ABRIR ARCHIVO
    printf("Ingrese el nombre del archivo :");
    scanf("%s", archivo);
    fd = fopen(archivo,"r");
       
    if    (fd == NULL)
            printf("No se pudo leer el archivo \n");
    if     (fd != NULL)
        printf("Se leyo el archivo con exito \n");  
//-------------------------------------------------------VACIAR ARCHIVO EN ARRAY
    while (!feof(fd)){
        j = 0;
        if(!fgets(aux,100,fd)) break; //100 tamaño del archivo
        strip(aux, ' ');
        buffer=aux;
        conj[i].size = 0;
        if(!cont) {
          sz_uni = getSize(buffer);
          p = (char *)(strtok(buffer, ":"));
          for ( p=(char*)(strtok(NULL,separador)) ; p!=NULL ; p=(char*)(strtok(NULL,separador))) //Esta funcion va cortando la cadena usando el separado 
      {
            strip(p, '\n');
            strcpy(universo[k].name, p);
            universo[k].index = k;
            k++;
       }
          cont++;
          continue;
        }
        conj[i].size = getSize(aux);
        p=(char*)(strtok(buffer,":"));
        strip(p, '\n');
        strcpy(conj[i].name, p);
        conj[i].value = i;
        conj[i].p_elements = (int *) malloc(sizeof(int) * conj[i].size); 
        for (p=(char *)(strtok(NULL, separador)); p!=NULL ; p=(char*)(strtok(NULL,separador))) //Esta funcion va cortando la cadena usando el separado 
      {
        for(m=0; m<sz_uni; m++) {
          strip(p, '\n');
          if(!(strcmp(universo[m].name, p))){
            conj[i].p_elements[j] = universo[m].index;
            j++;
            break;
          }  
        }
      }
      if(j<conj[i].size) {
        printf("UNO DE LOS ELEMENTOS DEL CONJUNTO %s NO EXISTE EN EL UNIVERSO", conj[i].name);
        exit(1);
        break;
      }
      i++;
  }
  fclose(fd);
  cn_size = i;
  printf("\n------UNIVERSO------\n");
  for(i=0; i<sz_uni; i++) printf("|| %i .- %s", universo[i].index, universo[i].name); 
  for(i=0; i<cn_size; i++) {
    imprimirConjunto(&conj[i], universo);
  }
  auxC = cn_copy(conj[0], auxC);
  imprimirConjunto(auxC, universo);
  auxC = cn_union(conj[1], conj[0], auxC);
  imprimirConjunto(auxC, universo);
  auxC = cn_interseccion(*auxC, conj[1], auxC);
  imprimirConjunto(auxC, universo);
  auxC = cn_comp(conj[1], universo, sz_uni, auxC);
  imprimirConjunto(auxC, universo);

  cn_polaca(input, conj, cn_size, auxStr);
  return 0;
}
