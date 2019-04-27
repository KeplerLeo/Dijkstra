#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODE 10

//Estrutura que armazena dados uteis de um No, e que sera utilizada dentro da Dijkstra

typedef struct {
    int visitado;
    int peso;
    int prev;
} Node;

//Estrutura que armazena a matriz de adjacencia
int matriz_adj[MAX_NODE][MAX_NODE];

/*Funcao que imprime a matriz de adjacencia
 */
void imprime_matriz_adjacencia(int numero_nos) {
    printf("\n---------- Matriz de Adjacencia ----------\n");
    for (int l = 0; l < numero_nos; l++) {
        for (int c = 0; c < numero_nos; c++) {
            printf("%d\t", matriz_adj[l][c]);
        }
        printf("\n");
    }
    printf("------------------------------------------\n");
}

/**Funcao que inicializa a matriz de adjacencia com 0
 */
void inicializa_matriz_adjacencia(int numero_nos) {

    if (numero_nos > MAX_NODE) {
        printf("ERRO - Numero de nos excede MAX_NODE\n");
        while (1);
    }

    for (int i = 0; i < numero_nos; i++) {
        for (int k = 0; k < numero_nos; k++) {
            matriz_adj[i][k] = 0;
        }
    }
}

/**Funcao que le um arquivo grafo.txt descrito da seguinte maneira, exemplo:
2
node:0
1,10
node:1

Onde a primeira linha eh o numero total de nos
O restante da linhas detalha o nos em "node: x" e, abaixo, quais seus sucessores e pesos, ex:
Abaixo o no 0 possui um sucessor 1 com peso de comunicacao de 10, já o no 1 nao possui sucessores.
node:0
1,10
node:1
 */
int le_grafo() {
    FILE *arq;
    int linha_count, result, node_id, viz_id, peso;
    char linha[20];
    int numero_nos;

    arq = fopen("grafo.txt", "rt");

    if (arq == NULL) {
        printf("Problemas na CRIACAO do arquivo\n");
        while (1);
    }

    linha_count = 1;
    //Percorre todas as linhas do arquivo (inclusive com o '\n')
    while (!feof(arq)) {
        fgets(linha, 100, arq); // o 'fgets' lê até 99 caracteres ou até o '\n'

        if (linha_count == 1) { //Se for a primeira linha, entao le o numero de nos e inicializa a matriz de adjacencia com zeros
            numero_nos = atoi(linha);
            printf("Numero de nodes: %d\n", numero_nos);
            inicializa_matriz_adjacencia(numero_nos);

        } else { //Senao, aplica o algoritmo pra interpretar o arquivo e montar a matriz de adjacencia conforme as comunicacoes e custos

            //Se a linha atual possui a palavra "node:" entao, o ID do nodo atual e lido
            if (strstr(linha, "node:") != NULL) {
                node_id = atoi(linha + strlen("node:"));
            } else { //Senao, le os vizinhos do nodo atual e tambem o custo de comunicacao entre eles, atualizando a matriz
                viz_id = linha[0] - '0'; //Coverte char para int
                peso = atoi(linha + strlen(",") + 1); //Converte string para int
                matriz_adj[node_id][viz_id] = peso; //Atualiza a matriz com o peso
            }
        }
        linha_count++; //Incrementa o contador de linhas
    }
    fclose(arq);

    imprime_matriz_adjacencia(numero_nos);

    return numero_nos;
}

/*Dijkstra utiliza as os seguintes dados globais:
- matriz_adj = Matriz de Adjacencia
- lista_nos = Lista dos nos
 */

void dijkstra(int numero_nos, int origem, int destino) {
    //Variaveis auxiliares
    int id_curr, caminho, custo_total;
    //Estrutura que armazena informações uteis de cada No
    Node lista_nos[MAX_NODE];
    //Inicializa a lista de Nos
    for (int i = 0; i < numero_nos; i++) {
        lista_nos[i].peso = 65535;
        lista_nos[i].prev = -1;
        lista_nos[i].visitado = 0;
    }
    //Inicializa o no de origem com peso zero
    lista_nos[origem].peso = 0;
    //Percorre todos os nos
    for (int n = 0; n < numero_nos; n++) {
        id_curr = -1;
        //Busca pelo menor No que ainda nao foi visitado e que tenha o peso menor entre todos os outros Nos nao visitados
        for (int i = 0; i < numero_nos; i++) {
            //printf("visitado %d\n", visitados[i]);
            if (!lista_nos[i].visitado && (id_curr == -1 || lista_nos[i].peso <= lista_nos[id_curr].peso)) {
                id_curr = i;
            }
        }
        //Nesta regiao, o No com o menor peso eh representado por id_curr
        //Testa se o id_curr eh igual ao destino, se for, entao a busca eh encerrada
        if (id_curr == destino) {
            printf("\n\nFim da busca, caminho encontrado: {");
            break;
        }
        //Senao, bloqueia o No em id_curr
        printf("\nBloqueiou no %d\n", id_curr + 1);
        lista_nos[id_curr].visitado = 1; //Atualiza sua flag de visitado
        //Para cada vizinho do no bloqueado faca
        for (int v = 0; v < numero_nos; v++) {
            caminho = matriz_adj[id_curr][v]; //Pega o caminho do vizinho da matriz de adjacencia
            custo_total = caminho + lista_nos[id_curr].peso; //Calcula o caminho total, ou seja, o custo ate o vizinho + custo do No atual
            //Se o caminho ate o vizinho for valido (> 0) e e custo total e menor que o custo atual do vizinho entao deve atualizar o custo do vizinho
            if (caminho > 0 && custo_total < lista_nos[v].peso) {
                lista_nos[v].peso = custo_total; //Atualiza o custo do vizinho
                lista_nos[v].prev = id_curr; //Atualiza o No anterior, para depois poder saber o caminho mais curto
                printf("\tAtualizou no %d, com peso %d\n", v + 1, custo_total);
            }
        }

    }
    //Backtraking (define o caminho voltado do destino ate origem)
    while (id_curr != origem) {
        printf("%d -> ", id_curr);
        id_curr = lista_nos[id_curr].prev;
    }
    printf("%d}", id_curr);
}

void floydWarshall(int numero_nos, int origem, int destino) {
    //Variaveis auxiliares
    int id_curr, caminho, custo_total;
    //Estrutura que armazena informações uteis de cada No
    Node lista_nos[MAX_NODE];
    //Inicializa a lista de Nos
    for (int i = 0; i < numero_nos; i++) {
        lista_nos[i].peso = 65535;
        lista_nos[i].prev = -1;
        lista_nos[i].visitado = 0;
    }
    //Inicializa o no de origem com peso zero
    lista_nos[origem].peso = 0;
    //Percorre todos os nos
    for (int n = 0; n < numero_nos; n++) {
        id_curr = -1;
        //Busca pelo menor No que ainda nao foi visitado e que tenha o peso menor entre todos os outros Nos nao visitados
        for (int i = 0; i < numero_nos; i++) {
            //printf("visitado %d\n", visitados[i]);
            if (!lista_nos[i].visitado && (id_curr == -1 || lista_nos[i].peso <= lista_nos[id_curr].peso)) {
                id_curr = i;
            }
        }
        //Nesta regiao, o No com o menor peso eh representado por id_curr
        //Testa se o id_curr eh igual ao destino, se for, entao a busca eh encerrada
        if ((id_curr) == (destino)) {
            printf("\n\nFim da busca, caminho encontrado: {");
            break;
        }
        //Senao, bloqueia o No em id_curr
        printf("\nBloqueiou no %d\n", id_curr + 1);
        lista_nos[id_curr].visitado = 1; //Atualiza sua flag de visitado
        //Para cada vizinho do no bloqueado faca
        for (int v = 0; v < numero_nos; v++) {
            caminho = matriz_adj[id_curr][v]; //Pega o caminho do vizinho da matriz de adjacencia
            custo_total = caminho + lista_nos[id_curr].peso; //Calcula o caminho total, ou seja, o custo ate o vizinho + custo do No atual
            //Se o caminho ate o vizinho for valido (> 0) e e custo total e menor que o custo atual do vizinho entao deve atualizar o custo do vizinho
            if (caminho > 0 && custo_total < lista_nos[v].peso) {
                lista_nos[v].peso = custo_total; //Atualiza o custo do vizinho
                lista_nos[v].prev = id_curr; //Atualiza o No anterior, para depois poder saber o caminho mais curto
                printf("\tAtualizou no %d, com peso %d\n", v + 1, custo_total);
            }
        }

    }
    //Backtraking (define o caminho voltado do destino ate origem)
    while (id_curr != origem) {

        printf("%d -> ", (id_curr));
        id_curr = lista_nos[id_curr].prev;
    }
    printf("%d}", (id_curr));
}

int main(int argc, char** argv) {
    int numero_nos;
    numero_nos = le_grafo();
    dijkstra(numero_nos, 0, 5);
    return (EXIT_SUCCESS);
}