# Simpletron_Final
# Proyecto Final: Simpletron

## Descripción general

El presente proyecto consiste en el desarrollo de **Simpletron**, un simulador de computadora creado en lenguaje C que permite interpretar y ejecutar instrucciones del lenguaje de máquina SML.

El simulador utiliza una memoria de 100 posiciones para almacenar tanto las instrucciones como los datos necesarios durante la ejecución.

Como parte del proyecto, se implementaron mejoras al simulador original, un evaluador de expresiones aritméticas y un compilador encargado de traducir programas escritos en el lenguaje Simple a instrucciones SML.

## Objetivo

Desarrollar un sistema que permita comprender el funcionamiento de una computadora a nivel de instrucciones, aplicando conocimientos de programación, manejo de memoria, operaciones aritméticas y estructuras de control.

## Herramientas utilizadas

- Lenguaje de programación C.
- Embarcadero Dev-C++ 6.3.
- Git para el control de versiones.
- GitHub para almacenar y organizar el proyecto.

## Estructura del proyecto

| Archivo | Descripción |
|---|---|
| `simpeltron_final.c` | Simulador encargado de cargar y ejecutar instrucciones SML. |
| `Expresiones.c` | Programa para evaluar expresiones aritméticas. |
| `compilador.c` | Compilador que traduce programas del lenguaje Simple a SML. |
| `suma.sml` | Archivo de prueba para comprobar operaciones del simulador. |

## Funcionalidades implementadas

**1. Simulador Simpletron**

Permite introducir instrucciones manualmente o cargar programas desde archivos SML. Realiza operaciones aritméticas, saltos condicionales y muestra el contenido de los registros y la memoria al finalizar.

También cuenta con validaciones para identificar errores, como divisiones entre cero, instrucciones no válidas y desbordamientos del acumulador.

**2. Extensiones del lenguaje SML**

Se incorporaron las siguientes instrucciones:

| Código | Operación | Descripción |
|---|---|---|
| 34 | MOD | Obtiene el residuo de una división. |
| 35 | POWER | Calcula potencias con exponentes enteros no negativos. |
| 50 | NEWLINE | Imprime un salto de línea. |

**3. Evaluador de expresiones**

Permite trabajar con expresiones aritméticas que incluyen operadores matemáticos, potencias y paréntesis, respetando la prioridad de las operaciones.

**4. Compilador Simple a SML**

Traduce instrucciones del lenguaje Simple a código SML, utilizando variables, expresiones aritméticas, asignaciones y estructuras de control como condiciones y saltos.

## Compilación y ejecución

El proyecto fue desarrollado en Embarcadero Dev-C++ 6.3. Cada uno de los programas principales se compila de manera independiente.

También es posible compilarlos utilizando GCC:

```bash
gcc simpeltron_final.c -o simpletron
gcc Expresiones.c -o expresiones
gcc compilador.c -o compilador
```

**Ejecución del simulador**

Al iniciar el programa se puede seleccionar entre introducir instrucciones por teclado o cargar un archivo `.sml`.

**Ejecución del compilador**

Se solicita el nombre del programa de entrada con extensión `.simple` y el nombre del archivo `.sml` que se desea generar.

El archivo resultante puede cargarse posteriormente en Simpletron para su ejecución.

## Pruebas de funcionamiento

Se realizaron pruebas para verificar el comportamiento de los diferentes módulos, incluyendo:

- Operaciones de suma, resta, multiplicación y división.
- Validación de división entre cero y desbordamiento.
- Ejecución de las instrucciones MOD y POWER.
- Evaluación de expresiones con paréntesis y potencias.
- Traducción de programas Simple a SML.
- Ejecución de condiciones, comparaciones y ciclos.
- Identificación de errores en los programas de entrada.

## Control de versiones

Para el desarrollo y seguimiento del proyecto se utilizó Git y GitHub.

El repositorio cuenta con dos ramas principales:

- **main:** versión inicial del simulador.
- **mejoras-simpletron:** implementación de nuevas instrucciones y desarrollo de los módulos adicionales.

Los avances se registraron mediante commits para mantener un historial de los cambios realizados.

## Integrante

María Fernanda Cabrera Córdoba
