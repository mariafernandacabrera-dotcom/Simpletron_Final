#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
   Proyecto final Simpletron
   Compilador del lenguaje Simple a SML
*/

#define MEMORIA 100
#define MAX_LINEAS 200
#define MAX_TEXTO 256

// Variables del compilador
int memoria[MEMORIA];
int codigo = 0;
int datos = MEMORIA - 1;
int variables[26];
int constantes[200];
int direcciones[200];
int cantidadConstantes = 0;
int lineas[MAX_LINEAS];
char programas[MAX_LINEAS][MAX_TEXTO];
int cantidadLineas = 0;
int saltos[400], destinos[400], cantidadSaltos = 0;
int temporales[100];
int cantidadTemporales = 0;
int libres[100];
int cantidadLibres = 0;
int error = 0;
char mensaje[200];
const char *expresion;

//------------------------------------
// Manejo de errores y memoria
//------------------------------------
void mostrarError(const char *texto)
{
    if(!error)
    {
        error = 1;
        snprintf(mensaje, sizeof(mensaje), "%s", texto);
    }
}

void espacios(const char **p)
{
    while(isspace((unsigned char)**p)) (*p)++;
}

int reservarDato(void)
{
    if(datos < codigo)
    {
        mostrarError("La memoria del Simpletron es insuficiente.");
        return 0;
    }
    return datos--;
}
int reservarTemporal(void)
{
    int direccion;

    if (cantidadLibres > 0)
        direccion = libres[--cantidadLibres];
    else
        direccion = reservarDato();

    if (!error)
        temporales[cantidadTemporales++] = direccion;

    return direccion;
}

void liberarTemporales(void)
{
    int i;

    for (i = 0; i < cantidadTemporales; i++)
        libres[cantidadLibres++] = temporales[i];

    cantidadTemporales = 0;
}



int emitir(int operacion, int direccion)
{
    int posicion;
    if(error) return -1;
    if(codigo > datos || codigo >= MEMORIA || direccion < 0 || direccion >= MEMORIA)
    {
        mostrarError("El programa excede las 100 posiciones de memoria.");
        return -1;
    }
    posicion = codigo++;
    memoria[posicion] = operacion * 100 + direccion;
    return posicion;
}

int obtenerVariable(char letra)
{
    int indice;
    if(letra < 'a' || letra > 'z')
    {
        mostrarError("Las variables deben tener una sola letra minuscula.");
        return 0;
    }
    indice = letra - 'a';
    if(variables[indice] == -1)
        variables[indice] = reservarDato();
    return variables[indice];
}

int obtenerConstante(int valor)
{
    int i, direccion;
    if(valor < -9999 || valor > 9998)
    {
        mostrarError("Constante fuera del rango permitido.");
        return 0;
    }
    for(i = 0; i < cantidadConstantes; i++)
        if(constantes[i] == valor) return direcciones[i];
    if(cantidadConstantes >= 200)
    {
        mostrarError("Demasiadas constantes.");
        return 0;
    }
    direccion = reservarDato();
    if(!error)
    {
        constantes[cantidadConstantes] = valor;
        direcciones[cantidadConstantes++] = direccion;
        memoria[direccion] = valor;
    }
    return direccion;
}

void agregarSalto(int posicion, int linea)
{
    if(error) return;
    if(cantidadSaltos >= 400)
    {
        mostrarError("Demasiados saltos.");
        return;
    }
    saltos[cantidadSaltos] = posicion;
    destinos[cantidadSaltos++] = linea;
}

//------------------------------------
// Expresiones aritmeticas
//------------------------------------
void expresionSuma(void);

void expresionPrimaria(void)
{
    int valor = 0, direccion;
    char letra;
    espacios(&expresion);
    if(*expresion == '(')
    {
        expresion++;
        expresionSuma();
        espacios(&expresion);
        if(*expresion != ')') mostrarError("Falta cerrar parentesis.");
        else expresion++;
    }
    else if(isdigit((unsigned char)*expresion))
    {
        while(isdigit((unsigned char)*expresion))
        {
            valor = valor * 10 + (*expresion++ - '0');
            if(valor > 9998)
            {
                mostrarError("Constante demasiado grande.");
                return;
            }
        }
        direccion = obtenerConstante(valor);
        emitir(20, direccion);
    }
    else if(*expresion >= 'a' && *expresion <= 'z')
    {
        letra = *expresion++;
        if(isalpha((unsigned char)*expresion) || isdigit((unsigned char)*expresion))
        {
            mostrarError("Las variables deben tener una sola letra.");
            return;
        }
        direccion = obtenerVariable(letra);
        emitir(20, direccion);
    }
    else
        mostrarError("Se esperaba un numero, variable o parentesis.");
}

void expresionUnaria(void)
{
    int cero, temporal;
    espacios(&expresion);
    if(*expresion == '+')
    {
        expresion++;
        expresionUnaria();
    }
    else if(*expresion == '-')
    {
        expresion++;
        expresionUnaria();
        if(error) return;
        temporal = reservarTemporal();
        emitir(21, temporal);
        cero = obtenerConstante(0);
        emitir(20, cero);
        emitir(31, temporal);
    }
    else expresionPrimaria();
}

void expresionPotencia(void)
{
    int izquierdo, derecho;
    espacios(&expresion);
    expresionUnaria();
    espacios(&expresion);
    if(*expresion == '^' && !error)
    {
        expresion++;
        izquierdo = reservarTemporal();
        emitir(21, izquierdo);
        expresionPotencia();
      derecho = reservarTemporal();
        emitir(21, derecho);
        emitir(20, izquierdo);
        emitir(35, derecho); // extension POWER del simulador
    }
}

void expresionProducto(void)
{
    char operador;
    int izquierdo, derecho;
    expresionPotencia();
    espacios(&expresion);
    while((*expresion == '*' || *expresion == '/') && !error)
    {
        operador = *expresion++;
        izquierdo = reservarTemporal();
        emitir(21, izquierdo);
        expresionPotencia();
        derecho = reservarTemporal();
        emitir(21, derecho);
        emitir(20, izquierdo);
        emitir(operador == '*' ? 33 : 32, derecho);
        espacios(&expresion);
    }
}

void expresionSuma(void)
{
    char operador;
    int izquierdo, derecho;
    expresionProducto();
    espacios(&expresion);
    while((*expresion == '+' || *expresion == '-') && !error)
    {
        operador = *expresion++;
        izquierdo = reservarTemporal();
        emitir(21, izquierdo);
        expresionProducto();
        derecho = reservarTemporal();
        emitir(21, derecho);
        emitir(20, izquierdo);
        emitir(operador == '+' ? 30 : 31, derecho);
        espacios(&expresion);
    }
}

void traducirExpresion(const char *texto)
{
    expresion = texto;
    expresionSuma();
    espacios(&expresion);
    if(!error && *expresion != '\0')
        mostrarError("Hay simbolos no reconocidos en la expresion.");
}

//------------------------------------
// Comandos del lenguaje Simple
//------------------------------------
int leerNumero(const char **p, int *numero)
{
    int valor = 0;
    espacios(p);
    if(!isdigit((unsigned char)**p)) return 0;
    while(isdigit((unsigned char)**p))
    {
        valor = valor * 10 + (*(*p)++ - '0');
        if(valor > 99999) return 0;
    }
    *numero = valor;
    return 1;
}

int palabra(const char **p, const char *buscada)
{
    size_t n = strlen(buscada);
    espacios(p);
    if(strncmp(*p, buscada, n) != 0) return 0;
    if(isalnum((unsigned char)(*p)[n]) || (*p)[n] == '_') return 0;
    *p += n;
    return 1;
}

void sinTextoExtra(const char *p)
{
    espacios(&p);
    if(*p != '\0') mostrarError("Texto adicional al final del comando.");
}

void traducirCondicion(const char *p)
{
    int izquierdo, derecho, linea, pos, omitir;
    char relacion[3] = "";
    const char *inicio = p, *operador = NULL;
    int nivel = 0;

    // Localizar la comparacion fuera de parentesis
    while(*p)
    {
        if(*p == '(') nivel++;
        else if(*p == ')') nivel--;
        if(nivel == 0 && (*p == '<' || *p == '>' || *p == '=' || *p == '!'))
        {
            operador = p;
            break;
        }
        p++;
    }
    if(!operador || operador == inicio || operador - inicio >= MAX_TEXTO)
    {
        mostrarError("Condicion sin operador relacional.");
        return;
    }
    {
        char primera[MAX_TEXTO], segunda[MAX_TEXTO];
        size_t n = (size_t)(operador - inicio);
        memcpy(primera, inicio, n);
        primera[n] = '\0';
        p = operador;
        relacion[0] = *p++;
        if(*p == '=') relacion[1] = *p++;
        if(strcmp(relacion,"<") && strcmp(relacion,"<=") &&
           strcmp(relacion,">") && strcmp(relacion,">=") &&
           strcmp(relacion,"==") && strcmp(relacion,"!="))
        {
            mostrarError("Operador de comparacion incorrecto.");
            return;
        }
        inicio = p;
        nivel = 0;
        while(*p)
        {
            if(*p == '(') nivel++;
            else if(*p == ')') nivel--;
            if(nivel == 0 && strncmp(p,"goto",4) == 0 &&
               (p == inicio || isspace((unsigned char)p[-1])) &&
               !isalnum((unsigned char)p[4])) break;
            p++;
        }
        if(!*p || p - inicio >= MAX_TEXTO)
        {
            mostrarError("Falta goto en la condicion.");
            return;
        }
        n = (size_t)(p - inicio);
        memcpy(segunda,inicio,n);
        segunda[n] = '\0';
        p += 4;
        if(!leerNumero(&p,&linea))
        {
            mostrarError("Numero de linea incorrecto en goto.");
            return;
        }
        sinTextoExtra(p);
        if(error) return;

        traducirExpresion(primera);
        izquierdo = reservarTemporal();
        emitir(21,izquierdo);
        traducirExpresion(segunda);
        derecho = reservarTemporal();
        emitir(21,derecho);
        emitir(20,izquierdo);
        emitir(31,derecho); // Comparacion mediante resta
        if(error) return;

        if(!strcmp(relacion,"<") || !strcmp(relacion,"<="))
        {
            pos = emitir(41,0);
            agregarSalto(pos,linea);
            if(!strcmp(relacion,"<="))
            {
                pos = emitir(42,0);
                agregarSalto(pos,linea);
            }
        }
        else if(!strcmp(relacion,"=="))
        {
            pos = emitir(42,0);
            agregarSalto(pos,linea);
        }
        else if(!strcmp(relacion,"!="))
        {
            omitir = emitir(42,0);
            pos = emitir(40,0);
            agregarSalto(pos,linea);
            if(!error) memoria[omitir] = 4200 + codigo;
        }
        else if(!strcmp(relacion,">"))
        {
            int negativo = emitir(41,0);
            int igual = emitir(42,0);
            pos = emitir(40,0);
            agregarSalto(pos,linea);
            if(!error)
            {
                memoria[negativo] = 4100 + codigo;
                memoria[igual] = 4200 + codigo;
            }
        }
        else // >=
        {
            omitir = emitir(41,0);
            pos = emitir(40,0);
            agregarSalto(pos,linea);
            if(!error) memoria[omitir] = 4100 + codigo;
        }
    }
}

void traducirComando(const char *p)
{
    int direccion, linea, pos;
    char letra;
    espacios(&p);
    if(palabra(&p,"rem")) return;
    if(palabra(&p,"input"))
    {
        espacios(&p);
        letra = *p++;
        sinTextoExtra(p);
        if(!error) { direccion = obtenerVariable(letra); emitir(10,direccion); }
    }
    else if(palabra(&p,"print"))
    {
        espacios(&p);
        letra = *p++;
        sinTextoExtra(p);
        if(!error) { direccion = obtenerVariable(letra); emitir(11,direccion); }
    }
    else if(palabra(&p,"let"))
    {
        espacios(&p);
        letra = *p++;
        espacios(&p);
        if(*p++ != '=')
        {
            mostrarError("Falta = en la asignacion.");
            return;
        }
        direccion = obtenerVariable(letra);
        traducirExpresion(p);
        emitir(21,direccion);
    }
    else if(palabra(&p,"goto"))
    {
        if(!leerNumero(&p,&linea)) mostrarError("Numero incorrecto en goto.");
        sinTextoExtra(p);
        if(!error)
        {
            pos = emitir(40,0);
            agregarSalto(pos,linea);
        }
    }
    else if(palabra(&p,"if")) traducirCondicion(p);
    else if(palabra(&p,"end"))
    {
        sinTextoExtra(p);
        emitir(43,0);
    }
    else mostrarError("Comando Simple desconocido.");
}

//------------------------------------
// Lectura y dos pasadas
//------------------------------------
int leerPrograma(const char *nombre)
{
    FILE *archivo = fopen(nombre,"r");
    char texto[MAX_TEXTO];
    const char *p;
    int numero, anterior = -1;
    if(!archivo)
    {
        mostrarError("No se pudo abrir el programa Simple.");
        return 0;
    }
    while(fgets(texto,sizeof(texto),archivo))
    {
        p = texto;
        espacios(&p);
        if(*p == '\0') continue;
        if(cantidadLineas >= MAX_LINEAS)
        {
            mostrarError("Demasiadas lineas en el archivo.");
            break;
        }
        if(!strchr(texto,'\n') && !feof(archivo))
        {
            mostrarError("Una linea del programa es demasiado larga.");
            break;
        }
        if(!leerNumero(&p,&numero) || numero <= anterior)
        {
            mostrarError("Los numeros de linea deben ser ascendentes.");
            break;
        }
        anterior = numero;
        espacios(&p);
        lineas[cantidadLineas] = numero;
        snprintf(programas[cantidadLineas],MAX_TEXTO,"%s",p);
        programas[cantidadLineas][strcspn(programas[cantidadLineas],"\r\n")] = '\0';
        cantidadLineas++;
    }
    fclose(archivo);
    if(cantidadLineas == 0) mostrarError("El programa esta vacio.");
    return !error;
}

int compilar(const char *origen, const char *salida)
{
    int i,j,destino,encontro,termina = 0;
    int direccionesLinea[MAX_LINEAS];
    FILE *archivo;
    for(i=0;i<26;i++) variables[i] = -1;
    if(!leerPrograma(origen)) return 0;

    // Primera pasada: reconocer comandos y crear codigo provisional
    for(i=0;i<cantidadLineas && !error;i++)
    {
        direccionesLinea[i] = codigo;
        if(strncmp(programas[i],"end",3)==0 &&
           (programas[i][3]=='\0' || isspace((unsigned char)programas[i][3])))
            termina = 1;
        traducirComando(programas[i]);
        liberarTemporales();
        if(error)
            printf("Error en linea Simple %d: %s\n",lineas[i],mensaje);
    }
    if(!termina && !error) mostrarError("Falta el comando end.");

    // Segunda pasada: resolver saltos hacia numeros de linea
    for(i=0;i<cantidadSaltos && !error;i++)
    {
        encontro = 0;
        for(j=0;j<cantidadLineas;j++)
        {
            if(lineas[j] == destinos[i])
            {
                destino = direccionesLinea[j];
                if(destino >= MEMORIA)
                    mostrarError("Direccion de salto fuera de memoria.");
                else memoria[saltos[i]] += destino;
                encontro = 1;
                break;
            }
        }
        if(!encontro) mostrarError("Un goto apunta a una linea inexistente.");
    }
    if(error) return 0;

    archivo = fopen(salida,"w");
    if(!archivo)
    {
        mostrarError("No se pudo crear el archivo de salida.");
        return 0;
    }
    // Escribir las 100 palabras: instrucciones, espacios y datos.
    for(i=0;i<MEMORIA;i++)
        fprintf(archivo,"%04d\n",memoria[i]);
    fprintf(archivo,"9999\n");
    fclose(archivo);
    printf("Compilacion correcta. Instrucciones: %d. Datos desde: %d.\n",codigo,datos+1);
    printf("Archivo generado: %s\n",salida);
    return 1;
}

//------------------------------------
// Programa principal
//------------------------------------
int main(void)
{
    char origen[260],salida[260];
    printf("----- COMPILADOR SIMPLE A SML -----\n\n");
    printf("Archivo de entrada (.simple): ");
    if(scanf("%259s",origen)!=1) return 1;
    printf("Archivo de salida (.sml): ");
    if(scanf("%259s",salida)!=1) return 1;
    if(!compilar(origen,salida))
    {
        printf("***Error de compilacion: %s***\n",mensaje);
        return 1;
    }
    printf("----- FIN DEL COMPILADOR -----\n");
    return 0;
}
