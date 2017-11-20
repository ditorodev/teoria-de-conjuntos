#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conlib.h"
#include "pila.h"

//---------------------------------------------------------------------DEFINE
#define separador "-"
#define true 1
#define false 0

int toInt(char ch){
  const char nums[] = "0123456789";
  int i = 0;

  while(ch != nums[i++]);
  return i-1;    
}

char toChar(int num){
  const char nums[] = "0123456789";
  int i = 0;

  while(num != i++);
  return nums[i-1];    
}

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
  char op;
  char buffer[200],*p;
  strcpy(buffer, str);
  i = j = k = 0;
  stack = &pila; 
  anula(stack);
  p = strtok(buffer, s);
  while(p) {
    j=0;
    
    if(whosThis(p, conj, cn_sz) == -1) {
      printf("ERROR UNO DE LOS CONJUNTOS ES INVALIDO O NO EXISTE");
      return NULL;
    }

    if(str[lastOp] == '(' || str[lastOp] == 'x' || str[lastOp] == '+' || str[lastOp] == '-' || str[lastOp] == ')') {
      // BASICAMENTE SI LEO UN + Y TENIA UNA x SACO TODAS LAS x HASTA QUE HAYA OTRO +, TAMBIEN SI LEO UN - Y HABIA UN + O UN x SACO TODOS LOS OPERADORESHASTA QUE HAYA UN - O NO HAYA NADA PS
      if(vacia(stack)) { 
          push(stack, str[lastOp]);
          top(stack, &op);
        }
        else {
          // si el operador en el tope de la pila es una x y lo que lei es un + saco todas las equis hasta llegar a un -, una x o que la pila se vacie
          top(stack, &op);
          if ( top(stack, &op) && op == 'x'  && str[lastOp] == '+') {  
            while(top(stack, &op) && (op == 'x' || op == '-') ) {// mientrasse pueda  y en el tope hayan elementos de mayor precedencia los iremos sacando ejemplo cuando haya x-- y leamos + sacaremos --x y dejaremos +
              pop(stack, &op);
              if(op == 'x') strcat(dest, "x");
              else strcat(dest, "-");
              strcat(dest, "|");
            }
            // ya que sacamos todos los de mayor precedencia, metemos el de menor precedencia
            push(stack, str[lastOp]); 
          } // si el operador en el tope de la pila es u - y el que leemos es + o x, debemos sacar todos los -
            else if(op == '-' && (str[lastOp] == '+' || str[lastOp] == 'x')) {
           while( top(stack, &op) && op == '-') {
              pop(stack, &op);
              strcat(dest, "-");
              strcat(dest, "|");
            }
          push(stack, str[lastOp]); 
          }

          // si leo un parentesis que cierra debo vaciar todo hasta encontrar el parentesis que abre en la pila
          else if(str[lastOp] == ')') {
            while(top(stack, &op) && op != '(') {
              pop(stack, &op);
              if(op == 'x') strcat(dest, "x");
              else if(op == '+') strcat(dest, "+");
              else if (op == '-')strcat(dest, "-");
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
        if(str[i] == 'x' || str[i] == '+' || str[i] == '-' || str[i] == '(' || str[i] == ')' ) { 
        break;  
      }
        i++;
      }
      lastOp = i;
    } 
  }

  while(!vacia(stack)){
    pop(stack, &op);
    if(op == 'x') strcat(dest, "x");
    else if(op == '+') strcat(dest, "+");
    else if (op == '-') strcat(dest, "-");
    strcat(dest, "|");

    }
  
  printf("\n%s \nen notacion polaca\n %s\n", str, dest);
  return dest;
}

Conjunto *cn_parser(char *str, Conjunto *c, int cn_sz, Elemento *universo, int uni_sz, Conjunto *p_cRes){
  int i = 0, j = 0;
  int c1, c2;
  char aux;
  char buffer[500], ch, *p;
  Conjunto *auxC, *cnComp, *cnUnion, *cnInter;
  pila_t *stack, pila;
  stack = &pila;
  strcpy(buffer, str);
  // nos aseguramos que la variable donde alojaremos la respuesta este vacia
  if(p_cRes) free(p_cRes);
  p_cRes = (Conjunto *)malloc(sizeof(Conjunto));
  auxC = (Conjunto *) malloc(sizeof(Conjunto));
  cnComp = (Conjunto *)malloc(sizeof(Conjunto));
  cnUnion = (Conjunto *)malloc(sizeof(Conjunto));
  cnInter = (Conjunto *)malloc(sizeof(Conjunto));
  //PASO EL LO PRIMERO QUE LEA QUE SIEMPRE SERA UN CONJUNTO
  p = strtok(buffer, "|");
  while(p){
    if( !vacia(stack) && (p[0] == '+' || p[0] == 'x' || p[0] == '-')){
    ch = p[0];
    c1 = c2 = -1;
      pop(stack, &aux);
      switch (ch) {
        case 'x':
        case '+':
          if(!(aux == 'B') && !(aux == 'U')){ 
              c1 = toInt(aux);
              auxC = cn_copy(c[c1], auxC);
            }
          pop(stack, &aux);
          if(!(aux=='B') && !(aux=='U')){ 
            c2 = toInt(aux);
            if (c1 > -1 && auxC) p_cRes = cn_copy(c[c2], p_cRes); // si nunguno de los dos de la pila es un resultado, es decir, son conjuntos en memoria, se puede decir que uno de ellos es el conjunto respuesta
            else auxC = cn_copy(c[c2], auxC);
          }//QUEDASTE AQUI SAPO, TIENES QUE ASEGURARTE QUE CUANDO SEA UN NUMEL RESULTADO SE VAYA ACUMULE EN p_cRes. TAMBIEN TE FALTA HACER QUE CUANDO SEA U USES LA VARIABLE auxC
          if(aux == 'U') auxC = cn_copy(*cnComp, auxC); // si el elemento en la pila es un complemento, decimos que el auxiliar se unira con el elemento respuesta
          if(ch == 'x'){
            p_cRes = cn_interseccion(*auxC, *p_cRes, p_cRes);// almacenamos en los cojuntos resultantes
            //printf("\n ejecute interseccion \n");
            //imprimirConjunto(p_cRes, universo);
          } else if(ch == '+') {
            p_cRes = cn_union(*auxC, *p_cRes, p_cRes);
          }
          push(stack, 'B');      
        break;
        case '-':
          pop(stack, &aux);
          if(!(aux == 'B') && !(aux == 'U')) {
              c1 = toInt(aux);
              auxC = cn_copy(c[c1], auxC);
          }
          cnComp = cn_comp(*auxC, universo, uni_sz, cnComp); 
          printf("\n ejecute complemento \n");
          imprimirConjunto(p_cRes, universo);
        break;
      }
    } else {
      j = whosThis(p, c, cn_sz);
      c1 = toChar(j);
      push(stack, c1);  
    }
    printf("\n %s", p);
    p = strtok(NULL, "|");
  }
  strcpy(p_cRes->name, "RESULTADO");
  return p_cRes;  
}

int main(){

//-------------------------------------------------------VARIABLES
    FILE *fd;
    int i=0, k = 0, cn_size = 0, sz_uni = 0, cont=0, j=0, m=0;
    char aux[10000], archivo[100];   
    char *p, *buffer, auxStr[600];
    char input[] = "conjuntoC+conjuntoBxconjuntoA";
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
  //auxC = cn_copy(conj[0], auxC);
  //imprimirConjunto(auxC, universo);
  //auxC = cn_union(conj[1], conj[0], auxC);
  //imprimirConjunto(auxC, universo);
  //auxC = cn_interseccion(*auxC, conj[1], auxC);
  //imprimirConjunto(auxC, universo);
  //auxC = cn_comp(conj[1], universo, sz_uni, auxC);
  //imprimirConjunto(auxC, universo);

  cn_polaca(input, conj, cn_size, auxStr);
  printf("\n\n %s \n\n", auxStr);
  auxC = cn_parser(auxStr, conj, cn_size, universo, sz_uni, auxC);
  imprimirConjunto(auxC, universo);
  return 0;
}
