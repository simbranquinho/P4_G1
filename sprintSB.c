#include <stdio.h>
#include <math.h>

#define LEITURAS_MAX 5

double getTemp(double x) {
    return 260*x/1023-20;
}


int main(void) {
    double temp;
    int input;
    int input_ammount = 0;

    int input_array[LEITURAS_MAX];
    double temp_array[LEITURAS_MAX];


    printf("Sistema de Aquisição e Monitorização de Sensores\n");
    printf("Introduza um valor inteiro entre 0 e 1023, -1 para acabar a leitura\n");

    for(int i = 0; i < LEITURAS_MAX; i++) {
        printf("Valor medido pelo Sensor: \n");

        while (scanf("%d", &input) != 1 || input > 1023 || input < -1){
            while(getchar() != '\n'); //limpar input, para não ter informações em buffer da última ação

            printf("Inválido, o valor deve ser um número inteiro entre 0 e 1023 \n");
            printf("Novo Valor: \n");
        }

        if (input == -1) {
            break;
        } 
        else {
            temp = getTemp(input);

            if (temp <= -10 || temp >= 190) {
                printf("Valor fora da gama\n");
                while(getchar() != '\n');
                i--; //para não avançar no ciclo
            }
            else {
                input_array[i] = input;
                temp_array[i] = temp;
                input_ammount++;

                printf("A Temperatura é igual a %.2lf ºC\n", temp);
                while(getchar() != '\n');

            }

        }

    }

    double temp_sum = 0;
    double temp_avg = 0;
    double min_temp = temp_array[0];
    double max_temp = temp_array[0];
    double desvios = 0;
    double desvio_padrao = 0;

    printf("\nValores lidos (sensor -> temperatura)");

    for (int j = 0; j < input_ammount; j++) {

        if (temp_array[j] < min_temp) {
            min_temp = temp_array[j];
        }

        if (temp_array[j] > max_temp) {
            max_temp = temp_array[j];
        }

        temp_sum = temp_sum + temp_array[j];
        printf("\n%d -> %.2lf ºC", input_array[j], temp_array[j]);
          
    }

    temp_avg = temp_sum / input_ammount;

    printf("\n \nNúmero de valores lidos: %d \n", input_ammount);
    printf("Temperatura máxima: %.2lf ºC\n", max_temp);
    printf("Temperatura mínima: %.2lf ºC\n", min_temp);
    printf("Temperatura média: %.2lf ºC\n", temp_avg);
    
    for (int i = 0; i < input_ammount; i++) {
        desvios = desvios + pow(temp_array[i] - temp_avg, 2);
    }

    desvio_padrao = sqrt(desvios / input_ammount);

    printf("Desvio-padrão: %.2lf \n", desvio_padrao);

}