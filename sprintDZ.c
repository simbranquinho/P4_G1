#include <stdio.h>
#include <math.h>
int main (){
int c; // valor para limpiar buffer
int v; // valor de entrada //
int contador = 0;
float soma = 0;
float soma2 = 0;
float média;
float Dp;
int n; // VLA

printf("Este programa lê valores do sensor e calcula a temperatura (ºC)\n ");
printf("Insira o numero de amostras a serem lidas: \n");
scanf("%d", &n);
int leituras [n];
float temps [n];
int max = n;
printf("Insira o valor lido pelo sensor: (0 até 1023)\n");

while(v != -1 && contador < max){ // bucle infinito y controlado por las condicionales dentro
if (scanf("%d", &v)!= 1){ // si scanf lee un caracter
	printf("Este programa não aceita caractéres, por favor insira um valor válido: \n");
	while((c = getchar()) != '\n');
	continue;
}
if ( v == -1){
	break;
	}
if( (v < 0) || (v > 1023) ) {
	printf("valor inserido não válido: por favor insira outro valor: \n");
	
	}
else {
float temp = ((260.0*v)/1023.0)-20.0;
if((temp < -10) || (temp > 190)){
	printf("Valor fora da gama \n");
	}
else {
	temps[contador] = temp;
	printf("%.2f ºC \n", temp);
	leituras[contador] = v;
	contador++;
}
}; 
} // fim do while

// lista valores
printf("valores lidos (sensor -> temperatura): \n");
for (int i = 0; i < max; i++) {
printf(" %d -> %.2f ºC \n",leituras[i], temps[i] );
}
printf("Número de valores lidos: %d \n", contador);

// calculo das estatísticas
float Tmax = temps [0];
float Tmin = temps [0];
for(int i = 0; i < contador; i++){
	if(temps[i] < Tmin){
		Tmin = temps[i];
		}
	if(temps[i] > Tmax){
		Tmax = temps[i];
		}
	soma += temps[i];
	}
	
média = soma / contador;

for(int i = 0; i < contador; i++){
	soma2 += pow((temps[i] - média), 2);
	}
Dp = sqrtf(soma2 / contador);
printf("Temperatura mínima: %.2f ºC \n", Tmin);
printf("Temperatura máxima: %.2f ºC \n", Tmax);
printf("Temperatura média: %.2f ºC \n", média);
printf("Desvío padrão de temperatura: %.2f ºC \n", Dp);
return 0;
}
