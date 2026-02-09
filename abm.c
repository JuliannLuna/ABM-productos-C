/* 

Se desea realizar un programa que permita actualizar la lista de precios de una empresa en forma individual a cada uno de sus productos.
Los productos que están guardados en el archivo productos.dat con la siguiente estructura:
    • Código (entero)
    • Precio (float)
    • Descripción (de hasta 50 caracteres)

Para modificar un precio se ingresa por teclado el código del producto y el nuevo precio. La modificación
de precios finaliza con un código de producto igual a 0.

Al finalizar exportar a un archivo con formato de texto (.csv) para que la lista de precios pueda ser
visualizada directamente utilizando un programa de planillas de cálculo como por ejemplo el Excel.

*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

typedef struct
{
    int codigo;
    float precio;
    char descripcion[51];
} Producto;

FILE *abreArch(char*,char*);
void ingresoProductos();
float ingresoFloatMayorA(int);
int ingresoIntMayor(int);
void mostrarProductos();
void modificaProductos();

int main(void)
{
    int opcion;
    printf("Ingrese 1 si quiere dar de alta, 2 para modificar los precios, 3 para mostrar o 4 para finalizar: ");
    do
    {
        scanf("%d",&opcion);
        if(opcion!=1 && opcion!=2 && opcion!=3 && opcion!=4)
            printf("Ingreso incorrecto, intente nuevamente: ");
    }while(opcion!=1 && opcion!=2 && opcion!=3 && opcion!=4);

    while(opcion!=4)
    {
        switch(opcion)
        {
            case 1:
                ingresoProductos();
                break;
            case 2:
                modificaProductos();
                break;
            case 3:
                mostrarProductos();
                break;
        }

        printf("Ingrese 1 si quiere dar de alta, 2 para modificar los precios, 3 para mostrar o 4 para finalizar: ");
        do
        {
            scanf("%d",&opcion);
            if(opcion!=1 && opcion!=2 && opcion!=3 && opcion!=4)
                printf("Ingreso incorrecto, intente nuevamente: ");
        }while(opcion!=1 && opcion!=2 && opcion!=3 && opcion!=4);
    }
    return 0;
}

FILE *abreArch(char *nomArch,char *modo)
{
    FILE *archivo;
    archivo=fopen(nomArch,modo);
    if(archivo==NULL)
    {
        printf("\nError al abrir %s\n",nomArch);
        getch();
        exit(1);
    }
    return archivo;
}

void ingresoProductos()
{
    printf("\n\n===INGRESO PRODUCTOS===\n\n");
    FILE *archivo;
    Producto prod;
    archivo = abreArch("PRODUCTOS.dat","wb");
    int codigo;
    printf("Ingrese codigo del producto (con 0 finaliza): ");
    codigo = ingresoIntMayor(0);
    while(codigo!=0)
    {
        prod.codigo = codigo;
        printf("Ingrese precio mayor a 0 del producto: ");
        prod.precio = ingresoFloatMayorA(0);
        printf("Ingrese descripcion del producto: ");
        fflush(stdin);
        fgets(prod.descripcion,51,stdin);
        fwrite(&prod,sizeof(Producto),1,archivo);

        printf("�Ingreso correcto!\n\nIngrese codigo del producto (con 0 finaliza): ");
        codigo = ingresoIntMayor(0);
    }
    fclose(archivo);
}

int ingresoIntMayor(int inf)
{
    int num;
    do
    {
        scanf("%d",&num);
        if(num<inf)
            printf("El numero debe ser mayor o igual a %d, intente nuevamente: ",inf);
    }
    while(num<inf);
    return num;
}
float ingresoFloatMayorA(int inf)
{
    float num;
    do
    {
        scanf("%f",&num);
        if(num<=inf)
            printf("El numero debe ser mayor a %d, intente nuevamente: ",inf);
    }
    while(num<=inf);
    return num;
}

void mostrarProductos()
{
    printf("\n\n===LISTA DE PRODUCTOS===\n\n");
    FILE *archivo;
    Producto prod;
    archivo = abreArch("PRODUCTOS.dat","rb");
    fread(&prod,sizeof(Producto),1,archivo);
    while(!feof(archivo))
    {
        printf("Codigo del producto: %d\nPrecio: $%.2f\nDescripcion: %s\n\n",prod.codigo,prod.precio,prod.descripcion);
        fread(&prod,sizeof(Producto),1,archivo);
    }
    fclose(archivo);
}

void modificaProductos()
{
    FILE *archivo;
    Producto prod;
    archivo = abreArch("PRODUCTOS.dat","r+b");
    int codigo,encontrado;
    float precio;

    printf("Ingrese precio: ");
    precio = ingresoFloatMayorA(0);
    printf("Ingrese codigo del producto al cual modificarle el precio (con 0 finaliza): ");
    codigo = ingresoIntMayor(0);

    while(codigo!=0)
    {
        rewind(archivo); //llevo el puntero posicion al inicio para la nueva busqueda
        encontrado=0;
        fread(&prod,sizeof(Producto),1,archivo);
        while(!feof(archivo) && !encontrado) //Se ley� o se llego a la ultima posicion sin encontrar el codigo
        {
            if(prod.codigo==codigo)
            {
                prod.precio = precio;
                fseek(archivo,sizeof(Producto)*-1,SEEK_CUR); // se retrocede una posicion
                fwrite(&prod,sizeof(Producto),1,archivo); // sobreescribe
                fflush(archivo);
                encontrado=1;
                printf("Modificado con exito!\n");
            }else
                fread(&prod,sizeof(Producto),1,archivo);
        }
        printf("Ingrese precio: ");
        precio = ingresoFloatMayorA(0);
        printf("\n\nIngrese codigo del producto al cual modificarle el precio (con 0 finaliza): ");
        codigo = ingresoIntMayor(0);
    }
    fclose(archivo);
}
