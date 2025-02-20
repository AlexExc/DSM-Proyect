
#include<stdio.h>
#include<windows.h>
#include<time.h>

void main(){
    int pin=9876, opcion, pin_ingresado, intentos=0, monto=1;
    float saldo=8765;
    int continuar_transaccion=1;
    time_t ahora;
    time(&ahora);

    printf("\n");
    printf("\t\t\t\t\t %s",ctime(&ahora));
    printf("\n\t\t\t******************BIENVENIDO AL CAJERO AUTOMÁTICO DE CODE WITH CURIOUS*******************");

    while(pin != pin_ingresado){
        printf("\nPor favor, ingrese su PIN: ");
        scanf("%d",&pin_ingresado);
        if(pin_ingresado != pin){
            Beep(500,450);
            printf("PIN inválido!!!");
        }
        intentos++;
        if(intentos==3 && pin != pin_ingresado){
            exit(0);
        }
    }

    while(continuar_transaccion != 0){
        printf("\n\t\t\t*************Transacciones disponibles************");
        printf("\n\n\t\t1.Retiro");
        printf("\n\t\t2.Depósito");
        printf("\n\t\t3.Consultar saldo");
        printf("\n\n\t4.Seleccione la opción: ");
        scanf("%d",&opcion);

        switch(opcion){
            case 1:
                printf("\n\t\tIngrese el monto: ");
                scanf("%d",&monto);
                if(saldo < monto){
                    printf("\n\t Lo sentimos, saldo insuficiente");
                    monto = 1;
                    break;
                } else {
                    saldo -= monto;
                    printf("\n\t\tHa retirado Rs.%d. Su nuevo saldo es %.2f",monto,saldo);
                    monto = 1;
                    break;
                }
            case 2:
                printf("\n\t\tIngrese el monto a depositar: ");
                scanf("%d",&monto);
                saldo += monto;
                printf("\n\t\tHa depositado Rs.%d. Su nuevo saldo es %.2f",monto,saldo);
                printf("\n\t\t****************** Gracias por bancar con Code with Curious******************");
                monto = 1;
                break;
            case 3:
                printf("\n\t\tSu saldo es Rs.%.2f",saldo);
                break;
            default:
                Beep(500,450);
                printf("\n\t\tOpción inválida!!!");
        }

        printf("\n\t\t¿Desea realizar otra transacción? Presione 1[Si], 0[No]");
        scanf("%d",&continuar_transaccion);
    }
}

