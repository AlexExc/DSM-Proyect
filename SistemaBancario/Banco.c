#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CUENTAS 100
#define ARCHIVO_CUENTAS "cuentas.dat"  // Archivo de datos de cuentas
#define CONTRASENA_ADMIN "admin123"  // Contraseña del administrador

typedef struct {
    char nombre[50];
    char contrasena[20];
    int numeroCuenta;
    float saldo;
    int activa;
    int transferenciasHabilitadas;
} Cuenta;

typedef struct {
    char tipo[20];        // Tipo de transacción (Depósito, Retiro, Transferencia)
    float monto;          // Monto de la transacción
    char fechaHora[50];   // Fecha y hora de la transacción
} Transaccion;

void limpiarPantalla() {
    system("clear || cls");  // Limpia la pantalla
}

int generarNumeroCuenta() {
    return 100000 + rand() % 900000;  // Genera un numero de cuenta aleatorio
}

void guardarCuenta(Cuenta cuenta) {  // Guarda una cuenta en el archivo
    FILE *archivo = fopen(ARCHIVO_CUENTAS, "ab");
    if (archivo) {
        fwrite(&cuenta, sizeof(Cuenta), 1, archivo);  // Escribe la cuenta en el archivo
        fclose(archivo);  // Cierra el archivo
    }
}

int autenticarUsuario(char *nombre, char *contrasena, Cuenta *cuenta) {  // Verifica si un usuario existe
    FILE *archivo = fopen(ARCHIVO_CUENTAS, "rb");  // Abre el archivo
    if (!archivo) return 0;  // Si no se puede abrir el archivo, devuelve 0 
    
    while (fread(cuenta, sizeof(Cuenta), 1, archivo)) {  // Lee la cuenta del archivo
        if (strcmp(cuenta->nombre, nombre) == 0 && strcmp(cuenta->contrasena, contrasena) == 0) {  // Verifica si el nombre y la contraseña coinciden
            fclose(archivo);  // Cierra el archivo
            return 1;
        }
    }
    fclose(archivo);
    return 0;
}

void actualizarSaldoEnArchivo(Cuenta cuenta) {  // Actualiza el saldo de una cuenta en el archivo
    FILE *archivo = fopen(ARCHIVO_CUENTAS, "rb+");  // Abre el archivo en modo de lectura y escritura
    if (!archivo) {
        printf("Error al cargar datos fuente.\n");  // Si no se puede abrir el archivo, muestra un mensaje de error
        return;
    }

    Cuenta temp;  // Crea una variable temporal para almacenar la cuenta
    while (fread(&temp, sizeof(Cuenta), 1, archivo)) {  
        if (temp.numeroCuenta == cuenta.numeroCuenta) {  // Verifica si la cuenta coincide con la actual
            fseek(archivo, -((long)sizeof(Cuenta)), SEEK_CUR);  // Se desplaza el archivo hacia atrás
            fwrite(&cuenta, sizeof(Cuenta), 1, archivo);  // Escribe la nueva cuenta en el archivo
            fclose(archivo);
            return;
        }
    }
    fclose(archivo);
}

void registrarTransaccion(int numeroCuenta, char *tipo, float monto) {  // Registra una transacción en el archivo de historial
    char nombreArchivo[50];
    sprintf(nombreArchivo, "historial_%d.txt", numeroCuenta);  // Nombre del archivo de historial

    FILE *archivo = fopen(nombreArchivo, "a");  // Abre el archivo en modo append
    if (!archivo) {
        printf("Error al abrir el archivo de historial.\n");
        return;
    }

    // Obtener la fecha y hora actual
    time_t ahora = time(NULL);
    char fechaHora[50];
    strftime(fechaHora, sizeof(fechaHora), "%Y-%m-%d %H:%M:%S", localtime(&ahora));

    // Escribir la transacción en el archivo
    fprintf(archivo, "[%s] %s: $%.2f\n", fechaHora, tipo, monto);
    fclose(archivo);
}

void mostrarHistorial(int numeroCuenta) {  // Muestra el historial de transacciones de una cuenta
    char nombreArchivo[50];
    sprintf(nombreArchivo, "historial_%d.txt", numeroCuenta);  // Nombre del archivo de historial

    FILE *archivo = fopen(nombreArchivo, "r");  // Abre el archivo en modo lectura
    if (!archivo) {
        printf("No hay historial de transacciones para esta cuenta.\n");
        return;
    }

    printf("\n--- Historial de Transacciones (Cuenta: %d) ---\n", numeroCuenta);
    char linea[100];
    while (fgets(linea, sizeof(linea), archivo)) {  // Lee cada línea del archivo
        printf("%s", linea);
    }
    fclose(archivo);
    printf("--------------------------------------------\n");
}

void registrarUsuario() {  // Registra un usuario en el archivo
    Cuenta nuevaCuenta;  // Crea una variable para almacenar la nueva cuenta
    printf("Ingrese su nombre: ");  
    scanf("%s", nuevaCuenta.nombre);  // Lee el nombre del usuario
    printf("Ingrese una contrasena: ");
    scanf("%s", nuevaCuenta.contrasena);  // Lee la contraseña del usuario
    
    nuevaCuenta.numeroCuenta = generarNumeroCuenta();  // Genera un numero de cuenta aleatorio
    nuevaCuenta.saldo = 0.0;  // Inicializa el saldo a 0
    nuevaCuenta.activa = 1;  // Activa la cuenta
    nuevaCuenta.transferenciasHabilitadas = 1;  // Habilita las transferencias
    
    guardarCuenta(nuevaCuenta);  // Guarda la nueva cuenta en el archivo
    
    printf("Cuenta creada con exito! Su numero de cuenta es: %d\n", nuevaCuenta.numeroCuenta);  // Muestra un mensaje de confirmación
}

void realizarTransferencia(Cuenta *cuenta) {  // Función para realizar una transferencia
    if (!cuenta->transferenciasHabilitadas) {  // Verifica si las transferencias están habilitadas
        printf("Las transferencias estan suspendidas para esta cuenta.\n");  // Muestra un mensaje de error
        return; 
    } 

    int cuentaDestino;  // Crea una variable para almacenar el numero de cuenta destino
    float monto;  
    printf("Ingrese el numero de cuenta destino: "); 
    scanf("%d", &cuentaDestino);  

    if (cuentaDestino == cuenta->numeroCuenta) {  // Verifica si el numero de cuenta destino es el mismo que la actual
        printf("No puedes transferir dinero a tu propia cuenta.\n");  // Muestra un mensaje de error
        return;
    }

    printf("Ingrese el monto a transferir: ");  
    scanf("%f", &monto);
    
    if (monto > cuenta->saldo) {  // Verifica si el monto es mayor que el saldo
        printf("Fondos insuficientes para la transferencia.\n");
        return;
    }
    
    FILE *archivo = fopen(ARCHIVO_CUENTAS, "rb+");  // Abre el archivo en modo de lectura y escritura
    if (!archivo) {  
        printf("Error al cargar datos fuente.\n");
        return;
    }
    
    Cuenta temp;  // Crea una variable temporal para almacenar la cuenta destino
    int encontrada = 0;  // Crea una variable para almacenar si la cuenta destino ha sido encontrada
    while (fread(&temp, sizeof(Cuenta), 1, archivo)) {  
        if (temp.numeroCuenta == cuentaDestino) {  
            if (!temp.activa) {
                printf("La cuenta destino se encuentra suspendida. No se puede realizar la transferencia.\n");
                fclose(archivo);
                return;
            }
            temp.saldo += monto;  // Incrementa el saldo de la cuenta destino
            cuenta->saldo -= monto;  // Incrementa el saldo de la cuenta actual
            fseek(archivo, -((long)sizeof(Cuenta)), SEEK_CUR); 
            fwrite(&temp, sizeof(Cuenta), 1, archivo);
            encontrada = 1;
            break;
        }
    }
    fclose(archivo);

    if (encontrada) {  // Si la cuenta destino se encuentra en el archivo
        actualizarSaldoEnArchivo(*cuenta);
        registrarTransaccion(cuenta->numeroCuenta, "Transferencia (Enviado)", monto);  // Registrar transacción
        registrarTransaccion(cuentaDestino, "Transferencia (Recibido)", monto);       // Registrar transacción en la cuenta destino
        printf("Transferencia realizada con exito!\n");
    } else {
        printf("Cuenta destino no encontrada.\n");
    }
}

void listarCuentas() {  // Función para listar todas las cuentas
    FILE *archivo = fopen(ARCHIVO_CUENTAS, "rb");
    if (!archivo) {
        printf("No hay cuentas registradas.\n");
        return;
    }

    Cuenta cuenta;  // Crea una variable para almacenar la cuenta
    printf("\n--- Listado de Cuentas ---\n");  
    while (fread(&cuenta, sizeof(Cuenta), 1, archivo)) {  // Lee cada cuenta del archivo
        printf("Nombre: %s\n", cuenta.nombre);  
        printf("Numero de Cuenta: %d\n", cuenta.numeroCuenta);
        printf("Saldo: $%.2f\n", cuenta.saldo);
        printf("Estado: %s\n", cuenta.activa ? "Activa" : "Suspendida");
        printf("Transferencias: %s\n", cuenta.transferenciasHabilitadas ? "Habilitadas" : "Deshabilitadas");
        printf("-------------------------\n");
    }
    fclose(archivo);
}

void cambiarEstadoCuenta(int numeroCuenta, int estado) {  // Función para cambiar el estado de una cuenta
    FILE *archivo = fopen(ARCHIVO_CUENTAS, "rb+");
    if (!archivo) {
        printf("Error al acceder a la base de datos.\n");
        return;
    }

    Cuenta cuenta;
    int encontrada = 0;
    while (fread(&cuenta, sizeof(Cuenta), 1, archivo)) {  
        if (cuenta.numeroCuenta == numeroCuenta) {
            cuenta.activa = estado;
            fseek(archivo, -((long)sizeof(Cuenta)), SEEK_CUR);
            fwrite(&cuenta, sizeof(Cuenta), 1, archivo);
            encontrada = 1;
            break;
        }
    }
    fclose(archivo);

    if (encontrada) {  
        printf("Estado de la cuenta %d actualizado a %s.\n", numeroCuenta, estado ? "Activa" : "Suspendida");  // Muestra un mensaje de estado
    } else {
        printf("Cuenta no encontrada.\n");
    }
}

void cambiarEstadoTransferencias(int numeroCuenta, int estado) {  // Función para cambiar el estado de las transferencias de una cuenta
    FILE *archivo = fopen(ARCHIVO_CUENTAS, "rb+");
    if (!archivo) {
        printf("Error al acceder a la base de datos.\n");
        return;
    }

    Cuenta cuenta;
    int encontrada = 0;
    while (fread(&cuenta, sizeof(Cuenta), 1, archivo)) {
        if (cuenta.numeroCuenta == numeroCuenta) {
            cuenta.transferenciasHabilitadas = estado;
            fseek(archivo, -((long)sizeof(Cuenta)), SEEK_CUR);
            fwrite(&cuenta, sizeof(Cuenta), 1, archivo);
            encontrada = 1;
            break;
        }
    }
    fclose(archivo);

    if (encontrada) {
        printf("Transferencias de la cuenta %d %s.\n", numeroCuenta, estado ? "habilitadas" : "deshabilitadas");
    } else {
        printf("Cuenta no encontrada.\n");
    }
}

void menuUsuario(Cuenta cuenta) {  // Función para mostrar el menú del usuario
    if (!cuenta.activa) {
        printf("Cuenta suspendida. Favor de consultar a su banco.\n");  // Mensaje en caso de cuenta suspendida
        printf("Presione Enter para continuar...");
        getchar(); getchar();
        return;
    }

    int opcion;
    do {  // Bucle principal del menú del usuario
        limpiarPantalla();   // Limpia la pantalla
        printf("Bienvenido, %s (Cuenta: %d)\n", cuenta.nombre, cuenta.numeroCuenta);
        printf("1. Consultar saldo\n");
        printf("2. Depositar dinero\n");
        printf("3. Retirar dinero\n");
        printf("4. Transferir dinero\n");
        printf("5. Ver historial de transacciones\n");
        printf("6. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        
        switch (opcion) {
            case 1:
                printf("Saldo actual: $%.2f\n", cuenta.saldo);  // Muestra el saldo actual
                break;
            case 2: {
                float monto;
                printf("Ingrese monto a depositar: ");
                scanf("%f", &monto);
                cuenta.saldo += monto;  // Incrementa el saldo
                actualizarSaldoEnArchivo(cuenta);  // Actualiza el saldo en el archivo
                registrarTransaccion(cuenta.numeroCuenta, "Deposito", monto);  // Registrar transacción
                printf("Deposito exitoso. Nuevo saldo: $%.2f\n", cuenta.saldo);
                break;
            }
            case 3: {
                float monto;
                printf("Ingrese monto a retirar: ");
                scanf("%f", &monto);
                if (monto <= cuenta.saldo) {
                    cuenta.saldo -= monto;  
                    actualizarSaldoEnArchivo(cuenta);
                    registrarTransaccion(cuenta.numeroCuenta, "Retiro", monto);  // Registrar transacción
                    printf("Retiro exitoso. Nuevo saldo: $%.2f\n", cuenta.saldo);
                } else {
                    printf("Fondos insuficientes.\n");
                }
                break;
            }
            case 4:
                realizarTransferencia(&cuenta);  // Realiza una transferencia
                break;
            case 5:
                mostrarHistorial(cuenta.numeroCuenta);  // Muestra el historial de transacciones
                break;
            case 6:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
        printf("Presione Enter para continuar...");
        getchar(); getchar();
    } while (opcion != 6);
}

void menuAdministrador() {  // Función para mostrar el menú del administrador
    char contrasena[20];  // Crea una variable para almacenar la contraseña del administrador
    printf("Ingrese la contrasena de administrador: ");
    scanf("%s", contrasena);

    if (strcmp(contrasena, CONTRASENA_ADMIN) != 0) {  // Verifica si la contraseña es correcta
        printf("Contrasena incorrecta. Volviendo al menu principal...\n");
        return;
    }

    int opcion;  
    do {  // Bucle principal del menú del administrador
        limpiarPantalla();
        printf("--- Menu Administrador ---\n");
        printf("1. Listar todas las cuentas\n");
        printf("2. Suspender una cuenta\n");
        printf("3. Reactivar una cuenta\n");
        printf("4. Deshabilitar transferencias de una cuenta\n");
        printf("5. Habilitar transferencias de una cuenta\n");
        printf("6. Volver al menu principal\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        int numeroCuenta;
        switch (opcion) {
            case 1:
                listarCuentas();  // Lista todas las cuentas
                break;
            case 2:   // Suspender una cuenta
                printf("Ingrese el numero de cuenta a suspender: ");
                scanf("%d", &numeroCuenta);
                cambiarEstadoCuenta(numeroCuenta, 0);
                break;
            case 3:   // Reactivar una cuenta
                printf("Ingrese el numero de cuenta a reactivar: ");
                scanf("%d", &numeroCuenta);
                cambiarEstadoCuenta(numeroCuenta, 1);
                break;
            case 4:   // Deshabilitar transferencias de una cuenta
                printf("Ingrese el numero de cuenta para deshabilitar transferencias: ");
                scanf("%d", &numeroCuenta);
                cambiarEstadoTransferencias(numeroCuenta, 0);
                break;
            case 5:   // Habilitar transferencias de una cuenta
                printf("Ingrese el numero de cuenta para habilitar transferencias: ");
                scanf("%d", &numeroCuenta);
                cambiarEstadoTransferencias(numeroCuenta, 1);
                break;
            case 6:   // Volver al menu principal
                printf("Volviendo al menu principal...\n");
                break;
            default:  // Opción no valida
                printf("Opcion invalida.\n");
        }
        printf("Presione Enter para continuar...");
        getchar(); getchar();
    } while (opcion != 6);
}

void menuPrincipal() {  // Función para mostrar el menú principal
    int opcion;
    do {
        limpiarPantalla();
        printf("Sistema de Gestion Bancaria\n");
        printf("1. Usuario nuevo\n");
        printf("2. Usuario existente\n");
        printf("3. Administrador\n");
        printf("4. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        
        switch (opcion) {
            case 1:   // Registrar un usuario nuevo
                registrarUsuario();
                break;
            case 2: {  // Usuario existente
                Cuenta cuenta;
                char nombre[50], contrasena[20];
                printf("Ingrese nombre de usuario: ");
                scanf("%s", nombre);
                printf("Ingrese contrasena: ");
                scanf("%s", contrasena);
                
                if (autenticarUsuario(nombre, contrasena, &cuenta)) {  // Verifica si la credenciales son correctas
                    menuUsuario(cuenta);
                } else {
                    printf("Credenciales incorrectas.\n");
                }
                break;
            }
            case 3:   // Administrador
                menuAdministrador();
                break;
            case 4:  // Mensaje de despedida
                printf("\t\t\t==============================\n");
                printf("\t\t\t Sistema de gestion bancaria \n");
                printf("\t\t\t Terminado gracias por usar \n");
                printf("\t\t\t Desarrollado por alex.exc_ \n");
                printf("\t\t\t Git Hub: https://github.com/AlexExc\n");
                printf("\t\t\t==============================\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
        printf("Presione Enter para continuar...");
        getchar(); getchar();
    } while (opcion != 4);
}

int main() {  // Función principal
    srand(time(NULL));  // Inicializa el generador de números aleatorios
    menuPrincipal();  // Muestra el menú principal
    return 0;       
}