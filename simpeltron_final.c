#include <stdio.h>
/*
proyecto simulador Simpletron_final
*/
int main()
{
	//Memoria 
	int memory[100] = {0};
	int position = 0;  //posicion  de memoria 
	int value; 
	int i;           
	
	//Acumulador 
	int accumulator = 0;
	
	//Contador de instrucciones 
	int instructionCounter = 0;
	
	
	//Registro de instrucciones 
	int instructionRegister = 0;
	
	//Codigo de operaciones 
	int operationCode = 0;
	
	
	//Operando 
	int operand = 0;
	
	//control de ejecucion 
	int running = 1;
	int salto = 0;
	
	
	//Instruccion del usuario 
	int instruction;
	
int errorFatal = 0;
	
//-------------------------------------------------------
    //Inicio//
    printf("***Bienvenidos a SIMPLETRON ***\n");
    printf("***Introduzca su programa una instruccion ***\n");
    printf("***(o palabra de datos)a la vez en la linea ***\n\n");
    printf("***de texto de entrada.Yo indicare el numero***\n");
    printf("***de posicion y una interrogacion(?).Usted ***\n");
    printf("***tecleara una palabra para esa posicion.Capture ***\n\n");
    printf("***el numero 9999 para terminar de introducir ***\n");
    printf("***su programa ***\n");
    
//---------------------------------------------------------
//Carga del programa//
while(position <100)
{
	printf("%02d ?",position);
	scanf("%d",&instruction);
	
	if(instruction ==9999)
	{
		break;
	}
	
	if(instruction <-9999 || instruction > 9998)
	{
		printf("***¡¡Valor fuera de Rango!!\n");
		
	}else{
		memory[position]=instruction;
		position++;
	}
	
}//fin del while 
    
    printf("\n***Se termino de cargar el programa***\n");
//-------------------------------------------------------
//ejecucion//
    printf("***Comienza la ejecucion del programa ***\n\n");
    while (running ==1)
    {
    	
    	if(instructionCounter <0 || instructionCounter >=100)
    	{
    		printf("***Direccion de memoria no valida***\n");
    		errorFatal=1;
    		running = 0;
    		break;
		}
    //1ra instruccion 
    instructionRegister = memory[instructionCounter];
    
    operationCode = instructionRegister /100;
    operand = instructionRegister %100;
    //reinicio del control de salto 
    salto = 0;
    
//------------------------------------
switch(operationCode)
{
	case 10:
	    printf("Teclee un entero");
	        scanf("%d",&value);
	        while(value <-9999 || value > 9998)
	        {
	        	printf("***Valor fuera de rango***\n");
	        	printf("Teclee un Entero");
	        	scanf("%d",&value);
			}
	           memory[operand] = value;
	break;
	
	case 11:
		printf("%02d => %d\n",operand,memory[operand]);
		break;
		
	case 20:
		accumulator = memory[operand];
		break;
		
	case 21:
		memory[operand]= accumulator ;
		break;
		
		
	case 30:
		accumulator = accumulator + memory[operand];
		if(accumulator > 9999 || accumulator <-9999)
		{
			printf("***Desbordamiento del acumulador***\n");
			errorFatal =1;
			running = 0;
		}
		break;
		
	case 31:
		accumulator = accumulator - memory[operand];
		if(accumulator >9999 || accumulator <-9999)
		{
			printf("***Desbordamiento del acumulado***\n");
			errorFatal = 1;
			running = 0;
		}
		break;
		
	case 32:
		if(memory[operand]==0)
		{
			printf("***No se puede dividir entre cero***\n");
			errorFatal = 1;
			running = 0;
			
		}
		else
		{
			accumulator = accumulator / memory[operand];
			
		}
		break;
		
		
	case 33:
			accumulator = accumulator * memory[operand];
			if(accumulator >9999 || accumulator <-9999 )
			{
				printf("***Desbordamiento del acumulador***\n");
				errorFatal = 1;
				running = 0;
			}
			break;
			
	
	case 40:
		instructionCounter = operand;
		salto = 1;
		break;
		
		
	case 41:
		if(accumulator <0)
		{
			instructionCounter = operand;
			salto = 1;
		}
		break;
		
		
		
	case 42:
		if(accumulator ==0)
		{
			instructionCounter = operand;
			salto = 1;
		}
			break;
	case 43:
		printf("***Termino la ejecucion de Simpletron***\n");
		running = 0;
		break;
		
	    default:
		printf("***Codigo de operacion no es valido***\n");
		errorFatal= 1;
		running = 0;
		break;
}//fin del switch
if (running == 1 && salto ==0)
{
instructionCounter++;
}
}//fin del while
    
    if(errorFatal==1)
    {
    	printf("***La ejecucion de Simpletron termino anormalmente");
	}
    
    //Vaciado de registros y memoria 
    printf("\n***REGISTROS***:\n");
    printf("accumulator:                       %+05d\n",accumulator);
    printf("instructionCounter:                %05d\n",instructionCounter);
    printf("instructionRegister:               %+05d\n",instructionRegister);
    printf("operationCode:                     %02d\n",operationCode);
    printf("operand:                           %02d\n",operand);
//Mostrar la memoria completa 
printf("\n***MEMORIA***\n\n");
//columnas 
printf(" ");
for(i=0;i<10;i++)
{
	printf("%5d",i);
}
printf("\n");

for(i=0;i<100;i++)
{
	//filaas 
	if(i % 10 == 0)
	{
		printf("%d",i/10);
	}
	printf("%+05d", memory[i]);
	
	//cambio de renglon 
	if((i+1)%10 ==0)
	{
		printf("\n");
	}
}//fin del for
    

	return 0;
}//fin del main