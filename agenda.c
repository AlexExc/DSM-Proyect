#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TAREAS 100
#define MAX_LONGITUD 100

typedef struct {
    char descripcion[MAX_LONGITUD];
    int completada; // 0: No completada, 1: Completada
} Tarea;

void cargarTareas(Tarea tareas[], int *contador) {
    FILE *archivo = fopen("agenda.dat", "rb");
    if (archivo) {
        fread(contador, sizeof(int), 1, archivo);
        fread(tareas, sizeof(Tarea), *contador, archivo);
        fclose(archivo);
    }
}

void guardarTareas(Tarea tareas[], int contador) {
    FILE *archivo = fopen("agenda.dat", "wb");
    if (archivo) {
        fwrite(&contador, sizeof(int), 1, archivo);
        fwrite(tareas, sizeof(Tarea), contador, archivo);
        fclose(archivo);
    } else {
        printf("Error al guardar las tareas.\n");
    }
}

void agregarTarea(Tarea tareas[], int *contador) {
    if (*contador >= MAX_TAREAS) {
        printf("No se pueden agregar más tareas.\n");
        return;
    }

    printf("Ingrese la descripcion de la tarea: ");
    getchar(); // Limpiar el buffer
    fgets(tareas[*contador].descripcion, MAX_LONGITUD, stdin);
    tareas[*contador].descripcion[strcspn(tareas[*contador].descripcion, "\n")] = 0; // Eliminar salto de línea
    tareas[*contador].completada = 0;
    (*contador)++;
    printf("Tarea agregada con exito.\n");
}

void mostrarTareas(Tarea tareas[], int contador) {
    if (contador == 0) {
        printf("No hay tareas en la agenda.\n");
        return;
    }

    printf("\nTareas:\n");
    for (int i = 0; i < contador; i++) {
        printf("%d. %s [%s]\n", i + 1, tareas[i].descripcion, 
               tareas[i].completada ? "Completada" : "Pendiente");
    }
}

void marcarTarea(Tarea tareas[], int contador) {
    if (contador == 0) {
        printf("No hay tareas para marcar.\n");
        return;
    }

    int indice;
    printf("Ingrese el numero de la tarea que desea marcar como completada: ");
    scanf("%d", &indice);
    if (indice < 1 || indice > contador) {
        printf("Numero de tarea invalido.\n");
    } else {
        tareas[indice - 1].completada = 1;
        printf("Tarea marcada como completada.\n");
    }
}

int main() {
    Tarea tareas[MAX_TAREAS];
    int contador = 0;
    int opcion;

    cargarTareas(tareas, &contador);

    do {
        printf("\n--- Agenda ---\n");
        printf("1. Agregar tarea\n");
        printf("2. Mostrar tareas\n");
        printf("3. Marcar tarea como completada\n");
        printf("4. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                agregarTarea(tareas, &contador);
                break;
            case 2:
                mostrarTareas(tareas, contador);
                break;
            case 3:
                marcarTarea(tareas, contador);
                break;
            case 4:
                guardarTareas(tareas, contador);
                printf("Saliendo del programa. Tareas guardadas.\n");
                break;
            default:
                printf("Opcion invalida. Intente de nuevo.\n");
        }
    } while (opcion != 4);

    return 0;
}
