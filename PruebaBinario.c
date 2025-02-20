/*
 Alexis Rivera Rodríguez 
 Matricula: 20243ds065 
 Universidad Tecnologica Emiliano Zapata - Desarrollo de Software 2° C
 Creado 13/02/2025   Ultima modificación 17/02/2025
 Programa para administrar estudiantes y captura los datos en un archivo binario
 los cuales se pueden leer y escribir en la estructura Estudiante
 De manera que podamos guardar y recuperar los datos previamente guardados
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char nombre[50];
    int matricula;
    float promedio;
} Estudiante;

void guardarEstudiante() {
    FILE *archivo = fopen("estudiantes.bin", "ab"); // Modo "ab" para agregar sin borrar datos
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    Estudiante est;
    printf("Ingrese el nombre del estudiante: ");
    scanf(" %49[^\n]", est.nombre); // Leer con espacios
    printf("Ingrese la matricula: ");
    scanf("%d", &est.matricula);
    printf("Ingrese el promedio: ");
    scanf("%f", &est.promedio);

    fwrite(&est, sizeof(Estudiante), 1, archivo);  // Escribir en el archivo
    fclose(archivo);                                // Cerrar el archivo
    printf(" Estudiante guardado con exito.\n\n");  
}

void mostrarEstudiantes() {
    FILE *archivo = fopen("estudiantes.bin", "rb");  // Modo "rb" para leer
    if (archivo == NULL) {                           // Comprobar si se pudo abrir el archivo
        printf("Error al abrir el archivo o aun no hay estudiantes guardados.\n");
        return;
    }

    Estudiante est;
    printf("\n Lista de estudiantes:\n");
    while (fread(&est, sizeof(Estudiante), 1, archivo) == 1) {  // Leer y escribir en la estructura
        printf("Nombre: %s | Matricula: %d | Promedio: %.2f\n", est.nombre, est.matricula, est.promedio);
    }

    fclose(archivo);  // Cerrar el archivo
}

int main() {
    int opcion;

    do {
        printf("\n Menu de administracion de estudiantes:\n");
        printf("\n1. Agregar estudiante");
        printf("\n2. Mostrar estudiantes");
        printf("\n3. Salir");
        printf("\nSeleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                guardarEstudiante();
                break;
            case 2:
                mostrarEstudiantes();
                break;
            case 3:
                printf("Saliendo...\n");
                break;
            default:
                printf(" Opcion no valida, intenta de nuevo.\n");
        }
    } while (opcion != 3);

    return 0;
}
