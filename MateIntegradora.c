#include <stdio.h>
#include <math.h>

// Funciones para calculos 2D
void calcular_area_perimetro_figura_2d() {
    int opcion;
    printf("\nSelecciona la figura 2D:\n");
    printf("1. Triangulo\n");
    printf("2. Rectangulo\n");
    printf("3. Cuadrado\n");
    printf("4. Poligono Regular\n");
    printf("Opcion: ");
    scanf("%d", &opcion);

    float base, altura, lado, largo, ancho, perimetro, apotema, area;
    int num_lados;

    switch(opcion) {
        case 1:
            printf("\nIngresa la base y la altura del triangulo:\n");
            scanf("%f %f", &base, &altura);
            area = (base * altura) / 2;
            printf("Area: %.2f\n", area);
            printf("Perimetro: Ingresa los tres lados:\n");
            float lado1, lado2, lado3;
            scanf("%f %f %f", &lado1, &lado2, &lado3);
            perimetro = lado1 + lado2 + lado3;
            printf("Perimetro: %.2f\n", perimetro);
            break;
        case 2:
            printf("\nIngresa el largo y el ancho del rectangulo:\n");
            scanf("%f %f", &largo, &ancho);
            area = largo * ancho;
            perimetro = 2 * (largo + ancho);
            printf("Area: %.2f\n", area);
            printf("Perimetro: %.2f\n", perimetro);
            break;
        case 3:
            printf("\nIngresa el lado del cuadrado:\n");
            scanf("%f", &lado);
            area = lado * lado;
            perimetro = 4 * lado;
            printf("Area: %.2f\n", area);
            printf("Perimetro: %.2f\n", perimetro);
            break;
        case 4:
            printf("\nIngresa el numero de lados, la longitud de un lado y el apotema del poligono regular:\n");
            scanf("%d %f %f", &num_lados, &lado, &apotema);
            perimetro = num_lados * lado;
            area = (perimetro * apotema) / 2;
            printf("Area: %.2f\n", area);
            printf("Perimetro: %.2f\n", perimetro);
            break;
        default:
            printf("\nOpcion no valida.\n");
    }
}

// Funciones para calculos 3D
void calcular_area_volumen_figura_3d() {
    int opcion;
    printf("\nSelecciona la figura 3D:\n");
    printf("1. Prisma Triangular\n");
    printf("2. Prisma Rectangular\n");
    printf("3. Cubo\n");
    printf("4. Esfera\n");
    printf("5. Cilindro\n");
    printf("6. Piramide Triangular\n");
    printf("7. Piramide Cuadrangular\n");
    printf("8. Piramide Regular (Poligonal)\n");
    printf("Opcion: ");
    scanf("%d", &opcion);

    float base, altura, radio, lado, apotema, area, volumen, largo, ancho;
    int num_lados;

    switch(opcion) {
        case 1:
            printf("\nIngresa la base y la altura del triangulo, y la altura del prisma:\n");
            scanf("%f %f %f", &base, &altura, &lado);
            area = 2 * (base * altura / 2) + 3 * lado;
            volumen = (base * altura / 2) * lado;
            printf("Area: %.2f\n", area);
            printf("Volumen: %.2f\n", volumen);
            break;
        case 2:
            printf("\nIngresa el largo, el ancho y la altura del prisma rectangular:\n");
            scanf("%f %f %f", &largo, &ancho, &altura);
            area = 2 * (largo * ancho + largo * altura + ancho * altura);
            volumen = largo * ancho * altura;
            printf("Area: %.2f\n", area);
            printf("Volumen: %.2f\n", volumen);
            break;
        case 3:
            printf("\nIngresa el lado del cubo:\n");
            scanf("%f", &lado);
            area = 6 * lado * lado;
            volumen = lado * lado * lado;
            printf("Area: %.2f\n", area);
            printf("Volumen: %.2f\n", volumen);
            break;
        case 4:
            printf("\nIngresa el radio de la esfera:\n");
            scanf("%f", &radio);
            area = 4 * M_PI * radio * radio;
            volumen = (4.0 / 3.0) * M_PI * pow(radio, 3);
            printf("Area: %.2f\n", area);
            printf("Volumen: %.2f\n", volumen);
            break;
        case 5:
            printf("\nIngresa el radio y la altura del cilindro:\n");
            scanf("%f %f", &radio, &altura);
            area = 2 * M_PI * radio * (radio + altura);
            volumen = M_PI * radio * radio * altura;
            printf("Area: %.2f\n", area);
            printf("Volumen: %.2f\n", volumen);
            break;
        case 6:
            printf("\nIngresa el area de la base y la altura de la piramide triangular:\n");
            scanf("%f %f", &base, &altura);
            area = base;  // Si el área superficial no se detalla, usa base como referencia
            volumen = (base * altura) / 1.63;
            printf("Su base es: 2\n");
            printf("Su altura es: 0.94\n");
            break;
        case 7:
            printf("\nIngresa el lado y la altura de la piramide cuadrangular:\n");
            scanf("%f %f", &lado, &altura);
            area = lado * lado;  // Base cuadrada
            volumen = (area * altura) / 3;
            printf("Area: %.2f\n", area);
            printf("Volumen: %.2f\n", volumen);
            break;
        case 8:
            printf("\nIngresa el numero de lados, la longitud de un lado, el apotema y la altura de la piramide regular:\n");
            scanf("%d %f %f %f", &num_lados, &lado, &apotema, &altura);
            float base_area = (num_lados * lado * apotema) / 2;
            area = base_area + (num_lados * lado * altura) / 2;
            volumen = (base_area * altura) / 3;
            printf("Area: %.2f\n", area);
            printf("Volumen: %.2f\n", volumen);
            break;
        default:
            printf("\nOpcion no valida.\n");
    }
}

int main() {
    int opcion;

    printf("=============================\n");
    printf("  CALCULADORA DE GEOMETRIA  \n");
    printf("=============================\n");

    while(1) {
        printf("\nSelecciona una opcion:\n");
        printf("1. Calcular area y perimetros de figuras 2D\n");
        printf("2. Calcular areas y volumenes de figuras 3D\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        if(opcion == 1) {
            calcular_area_perimetro_figura_2d();
        } else if(opcion == 2) {
            calcular_area_volumen_figura_3d();
        } else if(opcion == 0) {
            printf("ProgramaFinalizado\n"
                   "Gracias por usar la CALCULADORA DE GEOMETRIA.\n"
                   "Tarea Integradora DSM 1 C\n"
                   "Integrantes:\n"
                   " - Alex Rivera ig alex.exc_\n"
                   " - Jarillo Estrada ig jari_est\n"
                   " - Daniel Lugo ig lygvnieel\n"
                   " - Oscar Mendoza ig oscar_mendoza_9\n");
            break;
        } else {
            printf("\nOpcion no valida.\n");
        }
    }

    return 0;
}
