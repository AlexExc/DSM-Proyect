// Archivo de registro de logs

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Niveles de registro de logs
typedef enum {
    DEPURACION,
    INFORMACION,
    ADVERTENCIA,
    ERROR,
    CRITICO
} NivelLog;

// Estructura para registrar logs
typedef struct {
    FILE* archivoLog;
} Registrador;

// Función para crear un registrador
Registrador* crearRegistrador(const char* nombreArchivo) {
    Registrador* registrador = malloc(sizeof(Registrador));
    registrador->archivoLog = fopen(nombreArchivo, "a");
    if (!registrador->archivoLog) {
        fprintf(stderr, "Error al abrir el archivo de log.\n");
        exit(1);
    }
    return registrador;
}

// Función para destruir un registrador
void destruirRegistrador(Registrador* registrador) {
    if (registrador->archivoLog) {
        fclose(registrador->archivoLog);
    }
    free(registrador);
}

// Función para registrar un log
void registrarLog(Registrador* registrador, NivelLog nivel, const char* mensaje) {
    time_t ahora = time(NULL);
    struct tm* infoTiempo = localtime(&ahora);
    char marcaTiempo[20];
    strftime(marcaTiempo, sizeof(marcaTiempo), "%Y-%m-%d %H:%M:%S", infoTiempo);

    // Crear entrada de log
    char entradaLog[256];
    sprintf(entradaLog, "[%s] %s: %s\n", marcaTiempo, 
            (nivel == DEPURACION) ? "DEPURACION" : 
            (nivel == INFORMACION) ? "INFORMACION" : 
            (nivel == ADVERTENCIA) ? "ADVERTENCIA" : 
            (nivel == ERROR) ? "ERROR" : 
            (nivel == CRITICO) ? "CRITICO" : "DESCONOCIDO",
            mensaje);

    // Salida a consola
    printf("%s", entradaLog);

    // Salida al archivo de log
    if (registrador->archivoLog) {
        fputs(entradaLog, registrador->archivoLog);
        fflush(registrador->archivoLog); // Asegurar escritura inmediata al archivo
    }
}

int main() {
    Registrador* registrador = crearRegistrador("archivoLog.txt");
    registrarLog(registrador, INFORMACION, "Programa iniciado.");
    registrarLog(registrador, DEPURACION, "Información de depuración.");
    registrarLog(registrador, ERROR, "Ocurrió un error.");
    destruirRegistrador(registrador);
    return 0;
}
