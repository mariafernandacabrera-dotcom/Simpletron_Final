# Pruebas de funcionamiento - Simpletron

En esta carpeta se organizan las pruebas realizadas durante el desarrollo del proyecto Simpletron.

## 1. Simulador Simpletron

Se realizaron pruebas para comprobar las siguientes operaciones:

- Suma: 4 + 7 = 11.
- Resta: 15 - 8 = 7.
- Multiplicación: 6 × 4 = 24.
- División: 24 / 6 = 4.
- Módulo: 7 % 3 = 1.
- Potencia: 7³ = 343.

También se verificó la detección de errores, incluyendo divisiones entre cero, desbordamientos del acumulador e instrucciones no válidas.

## 2. Evaluador de expresiones

Se probaron expresiones con diferentes operadores y paréntesis:

| Expresión | Resultado esperado |
|---|---|
| `(12 + 3) * 4` | 60 |
| `2 ^ 3 ^ 2` | 512 |

También se realizaron pruebas con paréntesis incorrectos y divisiones entre cero.

## 3. Compilador Simple a SML

Se realizaron pruebas de traducción y ejecución de programas con:

- Asignaciones y operaciones aritméticas.
- Condiciones y comparaciones.
- Ciclos mediante instrucciones de salto.
- Detección de errores de sintaxis.
- Validación de saltos a líneas inexistentes.

## Resultados

En las pruebas realizadas se obtuvieron los resultados esperados para los casos mencionados. 
Estas pruebas permiten comprobar las funciones principales del simulador, el evaluador y el compilador.
