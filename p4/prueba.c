#include <stdio.h>
#include <stdlib.h>

void main(){
  int i, primero, segundo, tercero;
  int posicionCaballo[5] = {0, 3, 8, 4, 1};
  primero = 0;
  segundo = 0;
  tercero = 0;

  for(i=1;i<3;i++){
    if(posicionCaballo[i]>posicionCaballo[primero]){
      tercero = segundo;
      segundo = primero;
      primero = i;
    } else if((posicionCaballo[i]>posicionCaballo[segundo])||(segundo==primero)) {
      tercero = segundo;
      segundo = i;
    } else if((posicionCaballo[i]>posicionCaballo[tercero])||(tercero==segundo)||(tercero==primero)) {
      tercero = i;
    }
  }
  printf("%d, %d, %d", primero, segundo, tercero);
}
