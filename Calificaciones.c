/*Programa que imprime los nombres de los alumnos y su calificación
Alexis Tomas Rivera Rodriguez Creado 06/Febrero/2025
*/
#include <stdio.h>
#include <string.h>


int main() {
 char nombre[10][50];
 char calificacion[10];
 int contador = 0;

 while (contador < 5) {
  printf("Ingrese el nombre del alumno: ");  //leer nombre del alumno
  fgets(nombre[contador], 50, stdin);
  nombre[contador][strcspn(nombre[contador], "\n")] = 0;  
  printf("Ingrese la calificacion del alumno (A-F): ");
  scanf(" %c", &calificacion[contador]);
  getchar();  //para que el usuario pueda ingresar otro alumno

  printf("%s: %c - ", nombre[contador], calificacion[contador]);  //imprimir nombre y calificacion
  if (calificacion[contador] == 'A') {
   printf("Excelente\n");
  } else if (calificacion[contador] == 'B') {
   printf("Notable\n");
  } else if (calificacion[contador] == 'C') {
   printf("Aprobado\n");
  } else {
   printf("Reprobado\n");
  }

  contador++;  //incrementar contador de alumnos
 }

 printf("\nLista de calificaciones:\n");  //imprimir lista de calificaciones
 for (int i = 0; i < contador; i++) {
  printf("Alumno %d: %s - %c ", i+1, nombre[i], calificacion[i]);
  if (calificacion[i] == 'A') {  //imprimir calificacion
   printf("- Excelente\n");
  } else if (calificacion[i] == 'B') {
   printf("- Notable\n");
  } else if (calificacion[i] == 'C') {
   printf("- Aprobado\n");
  } else {
   printf("- Reprobado\n");
  }
 }

 return 0;
}