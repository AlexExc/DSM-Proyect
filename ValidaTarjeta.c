//programa con metodo de luhn
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 20

typedef struct {
    char numeroTarjeta[MAX_LENGTH];
    int esValida;
} Tarjeta;


int validarTarjeta(const char* numeroTarjeta) {  //definicion de metodo
    int longitud = strlen(numeroTarjeta);
    int suma = 0;
    int alternar = 0;

    for (int i = longitud - 1; i >= 0; i--) {  //recorrido de la cadena
        if (!isdigit(numeroTarjeta[i])) {
            return 0; 
        }

        int digito = numeroTarjeta[i] - '0';  //conversion de caracter a numero

        if (alternar) {  //si es par
            digito *= 2;
            if (digito > 9) {
                digito -= 9;
            }
        }

        suma += digito;  //suma de los digitos
        alternar = !alternar;  //alternar el valor de alternar
    }

    return (suma % 10 == 0);  //si el resto de la suma es cero, la tarjeta es valida
}

int main() {
    char numeroTarjeta[20];  

    printf("Ingrese el numero de la tarjeta de credito: ");  
    scanf("%19s", numeroTarjeta);
  
    if (validarTarjeta(numeroTarjeta)) {  //llamada a metodo
        printf("La tarjeta es valida.\n");  
    } else {
        printf("La tarjeta no es valida.\n");  
    }

    return 0;  
}