#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define MAX 1200

//definindo o nó da tabela Hash
typedef struct 
{
    char frase[251];
    int tempo;
}No;

//definindo a estrutura tabela Hash
typedef struct{
    int tamanho;
    No **TabHash;
}tabela_hash;

//iniciando tabela hash
tabela_hash *inicializando(tabela_hash *t, int tamanho){
    //printf("O tamanho eh %d\n", tamanho);
    //alocando tabela hash
    if(t == NULL){
        tabela_hash *thash = malloc(sizeof(tabela_hash));
        //alocando o vetor de nós
        thash->TabHash = malloc(sizeof(No) * tamanho);
        //inicializando a tabela hash
        for(int i = 0; i < tamanho; i++)
            thash->TabHash[i] = NULL;
        return thash;
    }
    else{
        free(t->TabHash);
        //nao tenho ctz se a linha debaixo está correta
        free(t);
        tabela_hash *thash = malloc(sizeof(tabela_hash));
        //alocando o vetor de nós
        thash->TabHash = malloc(sizeof(No) * tamanho);
        //inicializando a tabela hash
        for(int i = 0; i < tamanho; i++)
            thash->TabHash[i] = NULL;
        return thash;
    }
}

int checando_primo(int num)
{
    int check;
    for (int i = 2; i <= (num / 2); i++) {
	    if ((num % i) == 0){
	        check = 1;
            return check;
            break;
        }
	    else
	        check = 0;
    }
    return check;
}

int encontrando_primo(int num)
{
    //vendo se o proprio numero eh primo
    int num2 = 1.25 * num;
    int confirmacao = checando_primo(num2);
    while(confirmacao != 0){
        num2++;
        confirmacao = checando_primo(num2);
    }

    return num2;
}

//funcao que converte string em chave
unsigned long djb2(char *str) {
  unsigned long hash = 5381;
  int c;
  
  while ((c = *str++))
    hash = ((hash << 5) + hash) ^ c; // hash * 33 XOR c
  
  return hash;
}

//funcao que calcula a chave hash de uma palavra/frase
int hashFunc(char *string, int colisoes, int maximo){
    int h1 = djb2(string) % maximo;
    int h2 = 1 + (djb2(string) % (maximo-1));

    int hash_final = (h1 + colisoes*h2) % maximo;

    return hash_final; 
}

//funcao para inserir um novo elemento
int inserir_elemento(tabela_hash *t, char *string_elemento, int maximo_tab, int tempo_elemento){
    //printf("tamanho da string: %d\n", strlen(string_elemento));

    int colidiu = 0;
    int hash_elemento = hashFunc(string_elemento, colidiu, maximo_tab);
    //printf("Valor de Hash: %d\n", hash_elemento);

    No *elemento = t->TabHash[hash_elemento];

    while(elemento != NULL){
        if(strcmp(elemento->frase, string_elemento) != 0){
            colidiu++;
            hash_elemento = hashFunc(string_elemento, colidiu, maximo_tab);
            elemento = t->TabHash[hash_elemento];
        }
        else{
            elemento->tempo = tempo_elemento;
            //printf("Status do 2 elemento: %p\n", elemento);
            return 0;
        }
    }

    if(elemento == NULL){
        No *novo_elemento = malloc(sizeof(No));
        strcpy(novo_elemento->frase, string_elemento);
        novo_elemento->tempo = tempo_elemento;
        t->TabHash[hash_elemento] = novo_elemento;
        return 0;
    }
    
}

//funcao que busca um elemento na tabela hash
int buscar_elemento(tabela_hash *t, char *str, int maximum){
    int colidiu = 0;
    int hash_elemento = hashFunc(str, colidiu, maximum);
    //printf("Valor de Hash: %d\n", hash_elemento);

    No *busca = t->TabHash[hash_elemento];

    while(busca){
        if(strcmp(busca->frase, str) != 0){
            colidiu++;
            hash_elemento = hashFunc(str, colidiu, maximum);
            busca = t->TabHash[hash_elemento];
        }
        else{
            printf("TS de [%s]: %d\n", busca->frase, busca->tempo);
            return 0;
        }
    }

    if(busca == NULL){
        printf("TS de [%s]: -1\n", str);
        return 0;
    }
}

//funcao que remove um elemento da tabela hash
int remover_elemento(tabela_hash *t, char *str, int maximum){
    int colidiu = 0;
    int hash_elemento = hashFunc(str, colidiu, maximum);
    //printf("Valor de Hash: %d\n", hash_elemento);

    No *remove = t->TabHash[hash_elemento];

    while(remove){
        if(strcmp(remove->frase, str) != 0){
            colidiu++;
            hash_elemento = hashFunc(str, colidiu, maximum);
            remove = t->TabHash[hash_elemento];
        }
        else{
            t->TabHash[hash_elemento] = NULL;
            free(remove);
            return 0;
        }
    }

    if(remove == NULL){
        return 0;
    }
}

//funcao para destruir a tabela hash
void destruir_tabela(tabela_hash *t, int maximo){
    for(int i = 0; i < maximo; i++){
        free(t->TabHash[i]);
    }
}

int main(void){
    int tamanho_main;
    tabela_hash *TabelaHash = NULL;
    char comando;
    int tempo = 0;

    while(scanf("%c", &comando) != 'f'){
        int ignore;
        char aleat;

        if(comando == 'c'){
            tempo = 0;
            scanf("%c", &aleat);
            scanf("%d", &tamanho_main);
            tamanho_main = encontrando_primo(tamanho_main);
            TabelaHash = inicializando(TabelaHash, tamanho_main);
        }

        if(comando == 'i'){
            char str_input [251];
            scanf("%c", &aleat);
            scanf("%[^\n]", str_input);
            ignore = inserir_elemento(TabelaHash, str_input, tamanho_main, tempo);
            tempo++;
        }

        if(comando == 'b'){
            char str_busca [251];
            scanf("%c", &aleat);
            scanf("%[^\n]", str_busca);
            ignore = buscar_elemento(TabelaHash, str_busca, tamanho_main);
        }

        if(comando == 'r'){
            char str_rem [251];
            scanf("%c", &aleat);
            scanf("%[^\n]", str_rem);
            ignore = remover_elemento(TabelaHash, str_rem, tamanho_main);
        }

        if(comando == 'f'){
            destruir_tabela(TabelaHash, tamanho_main);
            break;
        }
        
    }
    
}