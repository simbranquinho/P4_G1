#include <stdio.h>
#include <math.h>

#define LEITURAS_MAX 5
#define SENSOR_MIN -1
#define SENSOR_MAX 1023
#define GAMA_MIN -10
#define GAMA_MAX 190


int lerSeq(int input_a[], double temp_a[], int n); //lê a sequência relevante ao sensor, devolve o número de resultados 


void cleanBuffer(void); //limpa o buffer de leitura
double getTemp(double input); //converte valor do sensor em temperatura
int gama(double temp); //verifica se a temperatura se encontra na gama de valores seguros
int validInput(int input); //verifica se o input está no range do sensor


double media(double array[], int n); //devolve a média dos valores da array que receber
double desvioPadrao(double avg, double array[], int n); //devolve o desvio padrão de uma array, sabendo a média
double minimum(double array[], int n); //devolve o valor mínimo de uma array
double maximum(double array[], int n); //devolve o valor máximo de uma array


void printStartup(void); //imprime a mensagem de inicialização
void printResults(int array1[], double array2[], int n); //imprime os resultados input -> temperatura
void printStats(int n, double max, double min, double avg, double dp); //imprime as estatísticas da temperatura



int main(void) {
    int input_array[LEITURAS_MAX];
    double temp_array[LEITURAS_MAX];

    printStartup();

    int input_ammount = lerSeq(input_array, temp_array, LEITURAS_MAX);

    double avg_temp = media(temp_array, input_ammount);
    double max_temp = maximum(temp_array, input_ammount);
    double min_temp = minimum(temp_array, input_ammount);
    double desvio_padrao = desvioPadrao(avg_temp, temp_array, input_ammount);

    printResults(input_array, temp_array, input_ammount);
    printStats(input_ammount, max_temp, min_temp, avg_temp, desvio_padrao);

}


int lerSeq(int input_a[], double temp_a[], int n) {
    double temp;
    int input;
    int input_ammount = 0;

    for(int i = 0; i < n; i++) {
        printf("Valor medido pelo Sensor: \n");

        while (scanf("%d", &input) != 1 || validInput(input) != 1) {
            cleanBuffer();

            printf("Inválido, o valor deve ser um número inteiro entre 0 e 1023 \nValor medido pelo Sensor: \n");
        }

        if (input == -1) {
            break;
        } 
        else {
            temp = getTemp(input);

            if (gama(temp) != 1) {
                printf("Valor fora da gama\n");
                cleanBuffer();
                i--; //para não avançar no ciclo
            }
            else {
                input_a[i] = input;
                temp_a[i] = temp;
                input_ammount++;

                printf("A Temperatura é igual a %.2lf ºC\n", temp);
                cleanBuffer();

            }

        }

    }

    return input_ammount;
}

double getTemp(double input) {
    return 260 * input / 1023 - 20;
}

void cleanBuffer(void) {
    while(getchar() != '\n');
}

int gama(double temp) {
    if (temp <= GAMA_MIN || temp >= GAMA_MAX) {
        return 0;
    }
    else {
        return 1;
    }
    
}

int validInput(int input) {
    if (input < SENSOR_MIN || input > SENSOR_MAX) {
        return 0;
    }
    else {
        return 1;
    }
}

double media(double array[], int n) {
    float sum = 0;
    float avg;

    for (int i = 0; i < n; i++) {
        sum += array[i];
    }
    
    avg = sum / n;

    return avg;
}

double desvioPadrao(double avg, double array[], int n) {
    float numerador = 0;
    float desvio_padrao = 0;

    for (int i = 0; i < n; i++) {
        numerador += pow(array[i] - avg, 2);
    }
    
    desvio_padrao = sqrt(numerador / n);
    return desvio_padrao;

}

double minimum(double array[], int n) {
    double min = array[0];

    for (int i = 0; i < n; i++) {
        if (array[i] < min) {
            min = array[i];
        }
    }
    
    return min;
}

double maximum(double array[], int n) {
    double max = array[0];

    for (int i = 0; i < n; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    
    return max;
}

void printStartup(void) {
    printf("Sistema de Aquisição e Monitorização de Sensores\n");
    printf("Introduza um valor inteiro entre %d e %d, -1 para acabar a leitura\n", SENSOR_MIN + 1, SENSOR_MAX);
}

void printResults(int array1[], double array2[], int n) {
    printf("\nValores lidos (sensor -> temperatura)");

    for (int i = 0; i < n; i++) {
        printf("\n%d -> %.2lf ºC", array1[i], array2[i]);
    }
    
}

void printStats(int n, double max, double min, double avg, double dp) {
    printf("\n \nNúmero de valores lidos: %d \n", n);
    printf("Temperatura máxima: %.2lf ºC\n", max);
    printf("Temperatura mínima: %.2lf ºC\n", min);
    printf("Temperatura média: %.2lf ºC\n", avg);
    printf("Desvio-padrão: %.2lf \n", dp);
}