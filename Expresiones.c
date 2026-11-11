#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM 300

/*
 Proyecto Final Simpletron
 Evaluacion de expresiones
*/

//--------------------------------------
// Variables y funciones
//--------------------------------------

int prioridad(char operador)
{
    if (operador == '^') return 4;
    if (operador == '~') return 3; // signo negativo
    if (operador == '*' || operador == '/') return 2;
    if (operador == '+' || operador == '-') return 1;
    return 0;
}

int abre(char c)
{
    return c == '(' || c == '[' || c == '{';
}

int cierra(char c)
{
    return c == ')' || c == ']' || c == '}';
}

int pareja(char a, char b)
{
    return (a == '(' && b == ')') ||
           (a == '[' && b == ']') ||
           (a == '{' && b == '}');
}

//--------------------------------------
// Validar parentesis
//--------------------------------------

int validar(char expresion[], int *maximo)
{
    char pila[TAM];
    int tope = -1;
    int i;
    *maximo = 0;

    for (i = 0; expresion[i] != '\0'; i++)
    {
        if (abre(expresion[i]))
        {
            if (tope >= TAM - 1) return 0;
            pila[++tope] = expresion[i];
            if (tope + 1 > *maximo) *maximo = tope + 1;
        }
        else if (cierra(expresion[i]))
        {
            if (tope < 0 || !pareja(pila[tope], expresion[i]))
                return 0;
            tope--;
        }
    }
    return tope == -1;
}

//--------------------------------------
// Convertir a postfija
//--------------------------------------

int convertir(char expresion[], char postfija[])
{
    char pila[TAM];
    int tope = -1, i = 0, j = 0;
    int esperaNumero = 1;
    char c;

    while ((c = expresion[i]) != '\0')
    {
        if (isspace((unsigned char)c)) { i++; continue; }

        if (isdigit((unsigned char)c))
        {
            if (!esperaNumero) return 0;
            while (isdigit((unsigned char)expresion[i]))
            {
                if (j >= TAM - 3) return 0;
                postfija[j++] = expresion[i++];
            }
            postfija[j++] = ' ';
            esperaNumero = 0;
            continue;
        }
        if (abre(c))
        {
            if (!esperaNumero || tope >= TAM - 1) return 0;
            pila[++tope] = c;
        }
        else if (cierra(c))
        {
            if (esperaNumero) return 0;
            while (tope >= 0 && !abre(pila[tope]))
            {
                if (j >= TAM - 3) return 0;
                postfija[j++] = pila[tope--];
                postfija[j++] = ' ';
            }
            if (tope < 0 || !pareja(pila[tope], c)) return 0;
            tope--;
            esperaNumero = 0;
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^')
        {
            if (esperaNumero)
            {
                if (c == '+') { i++; continue; }
                if (c != '-' || tope >= TAM - 1) return 0;
                pila[++tope] = '~';
            }
            else
            {
                while (tope >= 0 && !abre(pila[tope]) &&
                      (prioridad(pila[tope]) > prioridad(c) ||
                      (prioridad(pila[tope]) == prioridad(c) && c != '^')))
                {
                    if (j >= TAM - 3) return 0;
                    postfija[j++] = pila[tope--];
                    postfija[j++] = ' ';
                }
                if (tope >= TAM - 1) return 0;
                pila[++tope] = c;
                esperaNumero = 1;
            }
        }
        else return 0;
        i++;
    }
    if (esperaNumero) return 0;
    while (tope >= 0)
    {
        if (abre(pila[tope]) || j >= TAM - 3) return 0;
        postfija[j++] = pila[tope--];
        postfija[j++] = ' ';
    }
    postfija[j] = '\0';
    return 1;
}

//--------------------------------------
// Calcular expresion postfija
//--------------------------------------

int calcular(char postfija[], long long *resultado)
{
    long long pila[TAM], a, b, total, potencia;
    int tope = -1, i = 0, k;
    char c;

    while ((c = postfija[i]) != '\0')
    {
        if (c == ' ') { i++; continue; }
        if (isdigit((unsigned char)c))
        {
            total = 0;
            while (isdigit((unsigned char)postfija[i]))
            {
                total = total * 10 + (postfija[i++] - '0');
                if (total > 9999) return 0;
            }
            if (tope >= TAM - 1) return 0;
            pila[++tope] = total;
            continue;
        }
        if (c == '~')
        {
            if (tope < 0) return 0;
            pila[tope] = -pila[tope];
            i++;
            continue;
        }
        if (tope < 1) return 0;
        b = pila[tope--];
        a = pila[tope--];
        total = 0;
        switch (c)
        {
            case '+': total = a + b; break;
            case '-': total = a - b; break;
            case '*': total = a * b; break;
            case '/':
                if (b == 0) return 0;
                total = a / b;
                break;
            case '^':
                if (b < 0) return 0;
                potencia = 1;
                for (k = 0; k < b; k++)
                {
                    potencia *= a;
                    if (potencia > 9999 || potencia < -9999) return 0;
                }
                total = potencia;
                break;
            default: return 0;
        }
        if (total > 9999 || total < -9999) return 0;
        pila[++tope] = total;
        i++;
    }
    if (tope != 0) return 0;
    *resultado = pila[0];
    return 1;
}

//--------------------------------------
// Programa principal
//--------------------------------------

int main(void)
{
    char expresion[TAM], postfija[TAM];
    int profundidad;
    long long resultado;

    printf("----- EVALUADOR DE EXPRESIONES -----\n\n");
    printf("Ingrese una expresion: ");
    if (fgets(expresion, TAM, stdin) == NULL) return 1;

    if (!validar(expresion, &profundidad))
    {
        printf("Error: parentesis incorrectos.\n");
        return 1;
    }
    if (!convertir(expresion, postfija))
    {
        printf("Error: expresion incorrecta.\n");
        return 1;
    }
    if (!calcular(postfija, &resultado))
    {
        printf("Error: operacion invalida o fuera de rango.\n");
        return 1;
    }
    printf("\nValidacion: Correcta\n");
    printf("Profundidad maxima: %d\n", profundidad);
    printf("Expresion postfija: %s\n", postfija);
    printf("Resultado: %lld\n", resultado);
    printf("\n----- FIN DEL PROGRAMA -----\n");
    return 0;
}
