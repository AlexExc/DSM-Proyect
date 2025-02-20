#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_CALLS 30
#define MAX_MESSAGES 30
#define MAX_USUARIOS 100

// Definición de IDs para controles
#define ID_BTN_LLAMADA 101
#define ID_BTN_MENSAJE 102
#define ID_BTN_RECARGAR 103
#define ID_BTN_SALIR 104
#define ID_BTN_HISTORIAL_LLAMADAS 105
#define ID_BTN_HISTORIAL_MENSAJES 106
#define ID_BTN_NAVEGAR_INTERNET 107
#define ID_BTN_BORRAR_HISTORIAL 108
#define ID_BTN_REGISTRAR_USUARIO 109
#define ID_BTN_ELIMINAR_USUARIO 110
#define ID_BTN_CONSULTAR_USUARIO 111
#define ID_INPUT_SALDO 201
#define ID_INPUT_NUMERO 202
#define ID_INPUT_MENSAJE 203
#define ID_INPUT_USUARIO 204
#define ID_INPUT_PASSWORD 205
#define ID_STATIC_SALDO 301
#define ID_EDIT_HISTORIAL 302
#define ID_STATIC_FECHA_HORA 303

typedef struct {
    char username[50];
    char password[50];
} Usuario;

// Variables globales
float saldo = 0.0;
long long llamadas_numero[MAX_CALLS];
int llamadas_duracion[MAX_CALLS];
long long mensajes_numero[MAX_MESSAGES];
char mensajes_mensaje[MAX_MESSAGES][100];
Usuario usuarios[MAX_USUARIOS];
int cantidad_usuarios = 0;
int pos_llamadas = 0, pos_mensajes = 0;

// Prototipos de funciones
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void agregarInterfaz(HWND hwnd);
void actualizarSaldoVisual(HWND hwnd);
void realizarLlamada(HWND hwnd);
void enviarMensaje(HWND hwnd);
void recargarSaldo(HWND hwnd);
void mostrarHistorialLlamadas(HWND hwnd);
void mostrarHistorialMensajes(HWND hwnd);
void borrarHistorial(HWND hwnd);
void registrarUsuario(HWND hwnd);
void eliminarUsuario(HWND hwnd);
void consultarUsuario(HWND hwnd);
void navegarInternet(HWND hwnd);
void mostrarMensaje(HWND hwnd, const char *mensaje, const char *titulo);
void actualizarHora(HWND hwnd);  // Declaración de la función actualizarHora

// Función principal
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "SimuladorCelular";

    RegisterClass(&wc);

    HWND hwnd = CreateWindow("SimuladorCelular", "Simulador de Celular", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                             100, 100, 600, 800, NULL, NULL, hInstance, NULL);

    if (!hwnd) return 0;

    SetTimer(hwnd, 1, 1000, NULL); // Timer para actualizar la hora
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

// Función para agregar los controles de la interfaz
void agregarInterfaz(HWND hwnd) {
    HINSTANCE hInstance = GetModuleHandle(NULL);

    // Campo para mostrar el saldo
    CreateWindow("static", "Saldo: $0.00", WS_VISIBLE | WS_CHILD,
                 50, 30, 200, 20, hwnd, (HMENU)ID_STATIC_SALDO, hInstance, NULL);

    // Campo para ingresar saldo
    CreateWindow("edit", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                 50, 60, 200, 20, hwnd, (HMENU)ID_INPUT_SALDO, hInstance, NULL);
    CreateWindow("button", "Recargar Saldo", WS_VISIBLE | WS_CHILD,
                 270, 60, 120, 20, hwnd, (HMENU)ID_BTN_RECARGAR, hInstance, NULL);

    // Controles para gestión de usuarios
    CreateWindow("static", "Usuario:", WS_VISIBLE | WS_CHILD,
                 50, 100, 150, 20, hwnd, NULL, hInstance, NULL);
    CreateWindow("edit", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                 50, 130, 200, 20, hwnd, (HMENU)ID_INPUT_USUARIO, hInstance, NULL);
    CreateWindow("static", "Contraseña:", WS_VISIBLE | WS_CHILD,
                 50, 170, 150, 20, hwnd, NULL, hInstance, NULL);
    CreateWindow("edit", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
                 50, 200, 200, 20, hwnd, (HMENU)ID_INPUT_PASSWORD, hInstance, NULL);

    CreateWindow("button", "Registrar Usuario", WS_VISIBLE | WS_CHILD,
                 270, 130, 150, 20, hwnd, (HMENU)ID_BTN_REGISTRAR_USUARIO, hInstance, NULL);
    CreateWindow("button", "Eliminar Usuario", WS_VISIBLE | WS_CHILD,
                 270, 170, 150, 20, hwnd, (HMENU)ID_BTN_ELIMINAR_USUARIO, hInstance, NULL);
    CreateWindow("button", "Consultar Usuario", WS_VISIBLE | WS_CHILD,
                 270, 200, 150, 20, hwnd, (HMENU)ID_BTN_CONSULTAR_USUARIO, hInstance, NULL);

    // Botón para salir
    CreateWindow("button", "Salir", WS_VISIBLE | WS_CHILD,
                 270, 260, 120, 30, hwnd, (HMENU)ID_BTN_SALIR, hInstance, NULL);
}

// Función para manejar eventos de la ventana
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            agregarInterfaz(hwnd);
            break;
        case WM_ERASEBKGND:
            {
                // Manejar fondo celeste
                HDC hdc = (HDC)wParam;
                HBRUSH brush = CreateSolidBrush(RGB(173, 216, 230));  // Celeste
                RECT rect;
                GetClientRect(hwnd, &rect);
                FillRect(hdc, &rect, brush);
                DeleteObject(brush);
                return 1;
            }
        case WM_TIMER:
            if (wParam == 1) { // Timer ID 1
                actualizarHora(hwnd); // Actualiza la hora cada vez que el temporizador se dispara
            }
            break;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_BTN_REGISTRAR_USUARIO:
                    registrarUsuario(hwnd);
                    break;
                case ID_BTN_ELIMINAR_USUARIO:
                    eliminarUsuario(hwnd);
                    break;
                case ID_BTN_CONSULTAR_USUARIO:
                    consultarUsuario(hwnd);
                    break;
                case ID_BTN_SALIR:
                    PostQuitMessage(0);
                    break;
                case ID_BTN_NAVEGAR_INTERNET:
                    navegarInternet(hwnd);
                    break;
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Función para mostrar mensajes
void mostrarMensaje(HWND hwnd, const char *mensaje, const char *titulo) {
    MessageBox(hwnd, mensaje, titulo, MB_OK | MB_ICONINFORMATION);
}

// Función para navegar por Internet (simulada)
void navegarInternet(HWND hwnd) {
    mostrarMensaje(hwnd, "Navegar por internet no está implementado en esta aplicación.", "Información");
}

// Función para actualizar la hora y fecha
void actualizarHora(HWND hwnd) {
    SYSTEMTIME st;
    GetLocalTime(&st);  // Obtener hora y fecha local

    // Formato: Día de la semana, Mes Día, Año, Hora:Minutos:Segundos
    char fechaHora[100];
    sprintf(fechaHora, "%02d/%02d/%04d %02d:%02d:%02d",
            st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);

    // Actualizar el texto en el control de hora y fecha
    SetWindowText(GetDlgItem(hwnd, ID_STATIC_FECHA_HORA), fechaHora);
}

// Función para verificar la validez de un número de teléfono
int esNumeroTelefonoValido(const char *telefono) {
    while (*telefono) {
        if (!isdigit(*telefono)) return 0;
        telefono++;
    }
    return 1;
}

// Función para realizar una llamada
void realizarLlamada(HWND hwnd) {
    char numero[20];
    GetWindowText(GetDlgItem(hwnd, ID_INPUT_NUMERO), numero, 20);

    if (!esNumeroTelefonoValido(numero)) {
        MessageBox(hwnd, "Número de teléfono inválido.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    // Guardar número de la llamada
    if (pos_llamadas < MAX_CALLS) {
        llamadas_numero[pos_llamadas] = atoll(numero);
        llamadas_duracion[pos_llamadas] = rand() % 60 + 1;  // Duración aleatoria entre 1 y 60 minutos
        pos_llamadas++;
    }

    MessageBox(hwnd, "Llamada realizada con éxito.", "Información", MB_OK | MB_ICONINFORMATION);
}


// Función para enviar un mensaje
void enviarMensaje(HWND hwnd) {
    char numero[20], mensaje[100];
    GetWindowText(GetDlgItem(hwnd, ID_INPUT_NUMERO), numero, 20);
    GetWindowText(GetDlgItem(hwnd, ID_INPUT_MENSAJE), mensaje, 100);

    if (!esNumeroTelefonoValido(numero)) {
        MessageBox(hwnd, "Número de teléfono inválido.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    if (saldo < 1.0) {
        MessageBox(hwnd, "Saldo insuficiente para enviar el mensaje.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    saldo -= 1.0; // Deduct 1 unit per message
    pos_mensajes++;
    mensajes_numero[pos_mensajes - 1] = atoll(numero);
    strcpy(mensajes_mensaje[pos_mensajes - 1], mensaje);

    actualizarSaldoVisual(hwnd);
    MessageBox(hwnd, "Mensaje enviado correctamente.", "Éxito", MB_OK | MB_ICONINFORMATION);
}

// Función para recargar el saldo
void recargarSaldo(HWND hwnd) {
    char saldoTexto[50];
    GetWindowText(GetDlgItem(hwnd, ID_INPUT_SALDO), saldoTexto, 50);

    float cantidad = atof(saldoTexto);
    if (cantidad <= 0) {
        MessageBox(hwnd, "Monto de recarga inválido.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    saldo += cantidad;
    actualizarSaldoVisual(hwnd);
    MessageBox(hwnd, "Saldo recargado exitosamente.", "Éxito", MB_OK | MB_ICONINFORMATION);
}

// Función para mostrar el historial de llamadas
void mostrarHistorialLlamadas(HWND hwnd) {
    // Mostrar historial de llamadas en el campo de texto
    char historial[500] = "Historial de Llamadas:\n";
    for (int i = 0; i < pos_llamadas; i++) {
        char temp[200];
        sprintf(temp, "Llamada %d: Teléfono: %lld, Duración: %d minutos\n", i + 1, llamadas_numero[i], llamadas_duracion[i]);
        strcat(historial, temp);
    }
    SetWindowText(GetDlgItem(hwnd, ID_EDIT_HISTORIAL), historial);
}

// Función para mostrar el historial de mensajes
void mostrarHistorialMensajes(HWND hwnd) {
    // Mostrar historial de mensajes en el campo de texto
    char historial[500] = "Historial de Mensajes:\n";
    for (int i = 0; i < pos_mensajes; i++) {
        char temp[200];
        sprintf(temp, "Mensaje %d: Teléfono: %lld, Mensaje: %s\n", i + 1, mensajes_numero[i], mensajes_mensaje[i]);
        strcat(historial, temp);
    }
    SetWindowText(GetDlgItem(hwnd, ID_EDIT_HISTORIAL), historial);
}

// Función para borrar el historial
void borrarHistorial(HWND hwnd) {
    pos_llamadas = 0;
    pos_mensajes = 0;
    SetWindowText(GetDlgItem(hwnd, ID_EDIT_HISTORIAL), "");
    MessageBox(hwnd, "Historial borrado correctamente.", "Éxito", MB_OK | MB_ICONINFORMATION);
}



// Función principal
int main() {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "MiVentana";
    
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "No se pudo registrar la clase de la ventana.", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    HWND hwnd = CreateWindowEx(0, "MiVentana", "Aplicación de Teléfono Móvil", 
                               WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 600, 700,
                               NULL, NULL, wc.hInstance, NULL);
    
    if (!hwnd) {
        MessageBox(NULL, "No se pudo crear la ventana.", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    SetTimer(hwnd, 1, 1000, NULL); // Temporizador para actualizar la hora cada segundo

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}