/*
 Alexis Rivera Rodríguez 
 Matricula: 20243ds065 
 Universidad del Estado de Zapata - Desarrollo de Software 2° C
 Creado 04/02/2025   Ultima modificación 14/02/2025
 * Este es un programa en C que implementa un sistema de gestión bancaria.
 * El programa permite crear cuentas bancarias, depositar
   y retirar dinero, consultar el saldo y transferir dinero entre cuentas.
 * El programa también permite guardar y cargar los datos de las cuentas en un archivo binario.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CUENTAS 100
#define ARCHIVO_DATOS "datos_bancarios.dat"

// Estructura para representar una cuenta bancaria
struct Cuenta {
    int numero_cuenta;
    char nombre_titular[50];
    char pin[5]; // PIN de 4 dígitos + carácter nulo
    float saldo;
};

// Funciones del sistema
void crearCuenta(struct Cuenta cuentas[], int *total_cuentas);
void depositar(struct Cuenta cuentas[], int total_cuentas);
void retirar(struct Cuenta cuentas[], int total_cuentas);
void consultarSaldo(struct Cuenta cuentas[], int total_cuentas);
void transferir(struct Cuenta cuentas[], int total_cuentas);
void guardarDatos(struct Cuenta cuentas[], int total_cuentas);
void cargarDatos(struct Cuenta cuentas[], int *total_cuentas);
int buscarCuenta(struct Cuenta cuentas[], int total_cuentas, int numero_cuenta);
void mostrarMenu();

// Función principal
int main() {
    struct Cuenta cuentas[MAX_CUENTAS];
    int total_cuentas = 0;
    int opcion;

    // Cargar datos desde el archivo al iniciar
    cargarDatos(cuentas, &total_cuentas);

    do {
        mostrarMenu();
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                crearCuenta(cuentas, &total_cuentas);
                break;
            case 2:
                depositar(cuentas, total_cuentas);
                break;
            case 3:
                retirar(cuentas, total_cuentas);
                break;
            case 4:
                consultarSaldo(cuentas, total_cuentas);
                break;
            case 5:
                transferir(cuentas, total_cuentas);
                break;
            case 6:
                guardarDatos(cuentas, total_cuentas);
                printf("\nDatos guardados correctamente.\n");
                break;
            case 7:
                printf("=============================\n");
                printf(" Sistema de gestión bancaria \n");
                printf(" Terminado gracias por usar \n");
                printf(" Desarrollado por alex.exc_ \n");
                printf("=============================\n");
                break;
            default:
                printf("\nOpción inválida. Intente nuevamente.\n");
        }
    } while (opcion != 7);

    return 0;
}

// Función para crear una nueva cuenta
void crearCuenta(struct Cuenta cuentas[], int *total_cuentas) {
    if (*total_cuentas >= MAX_CUENTAS) {
        printf("\nLímite de cuentas alcanzado. No se pueden crear más cuentas.\n");
        return;
    }

    struct Cuenta nueva_cuenta;

    printf("\nIngrese el número de cuenta: ");
    scanf("%d", &nueva_cuenta.numero_cuenta);

    // Verificar si la cuenta ya existe
    if (buscarCuenta(cuentas, *total_cuentas, nueva_cuenta.numero_cuenta) != -1) {
        printf("\nError: El número de cuenta ya existe.\n");
        return;
    }

    printf("Ingrese el nombre del titular: ");
    scanf(" %[^\n]", nueva_cuenta.nombre_titular);

    printf("Ingrese un PIN de 4 dígitos: ");
    scanf("%s", nueva_cuenta.pin);

    nueva_cuenta.saldo = 0.0; // Saldo inicial es 0

    cuentas[*total_cuentas] = nueva_cuenta;
    (*total_cuentas)++;

    printf("\n¡Cuenta creada con éxito!\n");
}

// Función para depositar dinero
void depositar(struct Cuenta cuentas[], int total_cuentas) {
    int numero_cuenta;
    float monto;

    printf("\nIngrese el número de cuenta: ");
    scanf("%d", &numero_cuenta);

    int indice = buscarCuenta(cuentas, total_cuentas, numero_cuenta);
    if (indice == -1) {
        printf("\nError: Cuenta no encontrada.\n");
        return;
    }

    printf("Ingrese el monto a depositar: ");
    scanf("%f", &monto);

    if (monto > 0) {
        cuentas[indice].saldo += monto;
        printf("\nDepósito exitoso. Nuevo saldo: %.2f\n", cuentas[indice].saldo);
    } else {
        printf("\nMonto inválido. Debe ser mayor que 0.\n");
    }
}

// Función para retirar dinero
void retirar(struct Cuenta cuentas[], int total_cuentas) {
    int numero_cuenta;
    float monto;
    char pin[5];

    printf("\nIngrese el número de cuenta: ");
    scanf("%d", &numero_cuenta);

    int indice = buscarCuenta(cuentas, total_cuentas, numero_cuenta);
    if (indice == -1) {
        printf("\nError: Cuenta no encontrada.\n");
        return;
    }

    printf("Ingrese su PIN: ");
    scanf("%s", pin);

    if (strcmp(cuentas[indice].pin, pin) != 0) {
        printf("\nError: PIN incorrecto.\n");
        return;
    }

    printf("Ingrese el monto a retirar: ");
    scanf("%f", &monto);

    if (monto > 0 && monto <= cuentas[indice].saldo) {
        cuentas[indice].saldo -= monto;
        printf("\nRetiro exitoso. Nuevo saldo: %.2f\n", cuentas[indice].saldo);
    } else {
        printf("\nMonto inválido o saldo insuficiente.\n");
    }
}

// Función para consultar el saldo
void consultarSaldo(struct Cuenta cuentas[], int total_cuentas) {
    int numero_cuenta;
    char pin[5];

    printf("\nIngrese el número de cuenta: ");
    scanf("%d", &numero_cuenta);

    int indice = buscarCuenta(cuentas, total_cuentas, numero_cuenta);
    if (indice == -1) {
        printf("\nError: Cuenta no encontrada.\n");
        return;
    }

    printf("Ingrese su PIN: ");
    scanf("%s", pin);

    if (strcmp(cuentas[indice].pin, pin) != 0) {
        printf("\nError: PIN incorrecto.\n");
        return;
    }

    printf("\nNúmero de cuenta: %d", cuentas[indice].numero_cuenta);
    printf("\nTitular: %s", cuentas[indice].nombre_titular);
    printf("\nSaldo actual: %.2f\n", cuentas[indice].saldo);
}

// Función para transferir dinero entre cuentas
void transferir(struct Cuenta cuentas[], int total_cuentas) {
    int cuenta_origen, cuenta_destino;
    float monto;
    char pin[5];

    printf("\nIngrese el número de cuenta de origen: ");
    scanf("%d", &cuenta_origen);

    int indice_origen = buscarCuenta(cuentas, total_cuentas, cuenta_origen);
    if (indice_origen == -1) {
        printf("\nError: Cuenta de origen no encontrada.\n");
        return;
    }

    printf("Ingrese su PIN: ");
    scanf("%s", pin);

    if (strcmp(cuentas[indice_origen].pin, pin) != 0) {
        printf("\nError: PIN incorrecto.\n");
        return;
    }

    printf("Ingrese el número de cuenta de destino: ");
    scanf("%d", &cuenta_destino);

    int indice_destino = buscarCuenta(cuentas, total_cuentas, cuenta_destino);
    if (indice_destino == -1) {
        printf("\nError: Cuenta de destino no encontrada.\n");
        return;
    }

    printf("Ingrese el monto a transferir: ");
    scanf("%f", &monto);

    if (monto > 0 && monto <= cuentas[indice_origen].saldo) {
        cuentas[indice_origen].saldo -= monto;
        cuentas[indice_destino].saldo += monto;
        printf("===============================================\n");
        printf("\nTransferencia exitosa.\n");
        printf("Nuevo saldo de la cuenta de origen: %.2f\n", cuentas[indice_origen].saldo);
        printf("Nuevo saldo de la cuenta de destino: %.2f\n", cuentas[indice_destino].saldo);
        printf("===============================================\n");
    } else {
        printf("\nMonto no valido o su saldo insuficiente.\n");
    }
}

// Función para guardar los datos en un archivo
void guardarDatos(struct Cuenta cuentas[], int total_cuentas) {
    FILE *archivo = fopen(ARCHIVO_DATOS, "wb");
    if (archivo == NULL) {
        printf("\nError al abrir el archivo.\n");
        return;
    }

    fwrite(cuentas, sizeof(struct Cuenta), total_cuentas, archivo);
    fclose(archivo);
}

// Función para cargar los datos desde un archivo
void cargarDatos(struct Cuenta cuentas[], int *total_cuentas) {
    FILE *archivo = fopen(ARCHIVO_DATOS, "rb");
    if (archivo == NULL) {
        printf("\nNo se encontraron datos guardados anteriores.\n");
        return;
    }

    *total_cuentas = fread(cuentas, sizeof(struct Cuenta), MAX_CUENTAS, archivo);
    fclose(archivo);
}

// Función para buscar una cuenta por número de cuenta
int buscarCuenta(struct Cuenta cuentas[], int total_cuentas, int numero_cuenta) {
    for (int i = 0; i < total_cuentas; i++) {
        if (cuentas[i].numero_cuenta == numero_cuenta) {
            return i;
        }
    }
    return -1; // Cuenta no encontrada
}

// Función para mostrar el menú principal
void mostrarMenu() {
    printf("\n\t\t\t============================");
    printf("\n\t\t\tSISTEMA DE GESTIÓN BANCARIA");
    printf("\n\t\t\t============================");
    printf("\n\t1. Crear cuenta");
    printf("\n\t2. Depositar dinero");
    printf("\n\t3. Retirar dinero");
    printf("\n\t4. Consultar saldo");
    printf("\n\t5. Transferir dinero");
    printf("\n\t6. Guardar datos");
    printf("\n\t7. Salir");
    printf("\n\nSeleccione una opción: ");
}