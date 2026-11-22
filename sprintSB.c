#include <stdio.h>
#include <math.h>
#include <string.h>

#define LEITURAS_MAX 20
#define TAM_LINHA 128
#define SENSOR_MIN -1
#define SENSOR_MAX 1023
#define GAMA_MIN -10
#define GAMA_MAX 190

struct sensor {
    char *nome;
    int ganho;
    int desvio;
};

struct sensor sensores[] = {
    { "TEMP1", 260, -20 },
    { "TEMP2", 250, -15 },
    { "TEMP3", 240, -10 },
};

#define N_SENSORES ((int)(sizeof sensores / sizeof sensores[0]))

struct leitura { 
    int sensor_id;
    int instante;
    int valor;
    double temp;
};

int lerSeq(struct leitura leituras[], int n); //lê a sequência relevante ao sensor, devolve o número de resultados 
int procurarSensor(char *id); //procura o id definido em 'sensores', devolve o índice se existir, -1 se não

void cleanBuffer(void); //limpa o buffer de leitura
double getTemp(struct sensor *s, int valor); //converte valor do sensor em temperatura
int gama(double temp); //verifica se a temperatura se encontra na gama de valores seguros
int validInput(int input); //verifica se o input está no range do sensor


double media(double array[], int n); //devolve a média dos valores da array que receber
double desvioPadrao(double avg, double array[], int n); //devolve o desvio padrão de uma array, sabendo a média
double minimum(double array[], int n); //devolve o valor mínimo de uma array
double maximum(double array[], int n); //devolve o valor máximo de uma array


void printStartup(void); //imprime a mensagem de inicialização
void printStatsForSensor(struct leitura leituras[], int n); //imprime as estatísticas de cada sensor, chama 'printStats'
void printResults(struct leitura leituras[], int n); //imprime os resultados input -> temperatura
void printStats(char *nome, int n, double max, double min, double avg, double dp); //imprime as estatísticas da temperatura



int main(void) {

    struct leitura leituras[LEITURAS_MAX];  

    printStartup();

    int total = lerSeq(leituras, LEITURAS_MAX);

    printResults(leituras, total);
    printStatsForSensor(leituras, total);

}


int lerSeq(struct leitura leituras[], int n) {
    char linha[TAM_LINHA];
    char nome[TAM_LINHA];
    int total = 0;

    while (total < n) {
        if (fgets(linha, sizeof(linha), stdin) == NULL) { //lê a linha
            break; //EOF (end of file) acaba a leitura (não dá erros se acabar o ficheiro sem 'FIM')
        }
        
        if (sscanf(linha, "%s", nome) != 1) { /*verifica se foi escrito um descritor de sensor, e associa-o a 'nome' 
                                               *(efetivamente verifica se foi premido 'ENTER' sem escrever)*/
            printf("Linha Inválida\n");
            continue;
        }
        else if (strcmp(nome, "FIM") == 0) { //verifica se foi escrita a mensagem de terminação
            break;
        }

        int instante, valor, lixo = 0;
        int input = sscanf(linha, "%*s %d %d %n", &instante, &valor, &lixo); //associa o resto da linha às variáveis respetivas

        if (input != 2 || linha[lixo] != '\0') { /*deve somente ler valores para 'instante' e 'valor' (sscanf retorna 2)
                                                  *o único caratere em 'lixo' deve ser terminação de string*/
            printf("Linha inválida\n");
            continue;
        }
                //"Linha inválida" toma precedência sobre "Sensor desconhecido", daí vem ter de ser avaliada antes
        int sensor_id = procurarSensor(nome);

        if (sensor_id < 0) {    //verifica se o identificador do sensor foi encontrado
            printf("Sensor desconhecido\n");
            continue;
        }
        else if (validInput(valor) != 1) {      //verifica se o valor dado foi dentro do range do sensor
            printf("Valor medido inválido, deve ser entre 0 e 1023\n");
            continue;
        }

        double temp = getTemp(&sensores[sensor_id], valor); 
        if (gama(temp) != 1) {      //verifica se está dentro da gama
            printf("Valor fora da gama\n");
            continue;
        }
        //associação dos valores ao array das leituras definido em 'main'
        leituras[total].sensor_id = sensor_id;
        leituras[total].instante = instante;
        leituras[total].valor = valor;
        leituras[total].temp = temp;
        total++;

        printf("%.2lf\n", temp);
    }

    return total;
}

int procurarSensor(char *id) {
    for (int i = 0; i < N_SENSORES; i++) {
        if (strcmp(id, sensores[i].nome) == 0) {
            return i;
        }
    }
    return -1;
}

double getTemp(struct sensor *s, int valor) {
    return s->ganho * (double)valor / 1023 + s->desvio;
}

void cleanBuffer(void) {
    while(getchar() != '\n');
}

int gama(double temp) {
    if (temp < GAMA_MIN || temp > GAMA_MAX) {
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
    double sum = 0;
    double avg;

    for (int i = 0; i < n; i++) {
        sum += array[i];
    }
    
    avg = sum / n;

    return avg;
}

double desvioPadrao(double avg, double array[], int n) {
    double numerador = 0;
    double desvio_padrao = 0;

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
    printf("Introduza uma leitura no formato SENSOR INSTANTE VALOR, FIM para acabar a leitura\n");
}

void printStatsForSensor(struct leitura leituras[], int n) {
    for (int i = 0; i < N_SENSORES; i++) {      //itera pelos diferentes sensores
        double temps[LEITURAS_MAX];             //variáveis temporárias, reinicializadas em cada loop
        int count = 0;

        for (int j = 0; j < n; j++) {           //itera pelas leituras desse sensor
            if (leituras[j].sensor_id == i) {
                temps[count++] = leituras[j].temp;
            }
            
        }

        if (count == 0) {
            continue;
        }

        double avg = media(temps, count);
        double max = maximum(temps, count);
        double min = minimum(temps, count);
        double dp = desvioPadrao(avg, temps, count);

        printStats(sensores[i].nome, count, max, min, avg, dp);
        
    }
    
}

void printResults(struct leitura leituras[], int n) {
    printf("\nValores lidos (nome instante sensor -> temperatura)\n");

    for (int i = 0; i < n; i++) {
        printf("%s %d: %3d -> %6.2lf ºC\n", 
            sensores[leituras[i].sensor_id].nome, //variável 'nome' do array 'sensores', no índice guardado na variável 'sensor_id' de 'leituras', no índice i
            leituras[i].instante, 
            leituras[i].valor, 
            leituras[i].temp);
    }
    
}

void printStats(char *nome, int n, double max, double min, double avg, double dp) {
    printf("\nSensor %s\n", nome);
    printf("Número de valores lidos: %d \n", n);
    printf("Temperatura máxima: %.2lf ºC\n", max);
    printf("Temperatura mínima: %.2lf ºC\n", min);
    printf("Temperatura média: %.2lf ºC\n", avg);
    printf("Desvio-padrão: %.2lf \n", dp);
}