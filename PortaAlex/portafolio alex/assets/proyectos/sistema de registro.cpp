#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// Niveles de registro de logs
enum NivelLog {
    DEPURACION,
    INFORMACION,
    ADVERTENCIA,
    ERROR,
    CRITICO
};

// Clase para registrar logs
class Registrador {
public:
    // Constructor que abre el archivo de log
    Registrador(const string& nombreArchivo) {
        archivoLog.open(nombreArchivo, ios::app);
        if (!archivoLog.is_open()) {
            cerr << "Error al abrir el archivo de log." << endl;
        }
    }

    // Destructor que cierra el archivo de log
    ~Registrador() {
        if (archivoLog.is_open()) {
            archivoLog.close();
        }
    }

    // Método para registrar un log
    void registrar(NivelLog nivel, const string& mensaje) {
        time_t ahora = time(0);
        tm* infoTiempo = localtime(&ahora);
        char marcaTiempo[20];
        strftime(marcaTiempo, sizeof(marcaTiempo), "%Y-%m-%d %H:%M:%S", infoTiempo);

        // Crear entrada de log
        ostringstream entradaLog;
        entradaLog << "[" << marcaTiempo << "] " << nivelACadena(nivel) << ": " << mensaje << endl;

        // Salida a consola
        cout << entradaLog.str();

        // Salida al archivo de log
        if (archivoLog.is_open()) {
            archivoLog << entradaLog.str();
            archivoLog.flush(); // Asegurar escritura inmediata al archivo
        }
    }

private:
    ofstream archivoLog; // Flujo de archivo para el archivo de log

    // Convierte nivel de log a cadena para salida
    string nivelACadena(NivelLog nivel) {
        switch (nivel) {
            case DEPURACION:
                return "DEPURACION";
            case INFORMACION:
                return "INFORMACION";
            case ADVERTENCIA:
                return "ADVERTENCIA";
            case ERROR:
                return "ERROR";
            case CRITICO:
                return "CRITICO";
            default:
                return "DESCONOCIDO";
        }
    }
};

int main() {
    Registrador registrador("archivoLog.txt");
    registrador.registrar(INFORMACION, "Programa iniciado.");
    registrador.registrar(DEPURACION, "Información de depuración.");
    registrador.registrar(ERROR, "Ocurrió un error.");
    return 0;
}
