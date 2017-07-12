#include <bits/stdc++.h>

#define REPETICOES 1000
#define TAMANHO 100
#define GERACOES 200
#define TAXACROSS 80
#define TAXAMUT1 30
#define TAXAMUT2 20
#define TAXAELIT 20
#define APTMAX 100000

using namespace std;

const bool m1 = true; // mut1 ou mut2
const bool s1 = true; // roleta ou torneio
const bool r1 = true; // reinsercao elit ou ordenada

// 0 1 2 3 4 5 6 7
// s e n d m o r y _ _
typedef struct individuo{
  int v[10];
  int fitness;
} Individuo;

Individuo P[TAMANHO + TAXACROSS + 5]; // Vetor global pq nao sou obrigada ;)

void print(Individuo a);                                              // Imprime os dados do individuo
int fitness(Individuo a);                                             // Funcao de avaliacao: |(SEND+MORE)-MONEY|
int compara(Individuo a, Individuo b);                                // Funcao de comparacao para ordenacao
pair<Individuo, Individuo> crossover(pair<Individuo, Individuo> par); // Faz o crossover de 2 individuos
void mutacao(int tam, int taxaM, int filhos);                         // Faz a troca de 2 posicoes no vetor do individuo, dentre as 10 possíveis, com taxa de "taxa" % pra cada individuo
void selecaoRoleta(int quant, int tam);                               // Seleciona quant individuos pelo metodo da Roleta e e gera os filhos da geracao atual
void selecaoTorneio(int quant, int tam);                              // Seleciona quant individuos pelo metodo de Torneio de tamanho 3 e gera os filhos da geracao atual
int reinsercaoPuraXElitismo(int tam, int taxa, int filhos);           // Cria a proxima geracao pelo metodo da reinsercao pura com elitismo de "taxa" %, retorna o fitness do melhor elemento
int reinsercaoOrdenada(int tam, int filhos);                          // Cria a proxima geracao pelo metodo da reinsercao ordenada, retorna o fitness do melhor elemento

main(){
  int sucesso = 0;
  int ini = time(NULL);

  srand(time(NULL));
  // Executar AG "REPETICOES" vezes:
  for(int k=0;k<REPETICOES;k++){

    // Gera populacao de "TAMANHO" individuos aleatoriamente:
    for(int i=0;i<TAMANHO;i++){
      set<int> s;
      Individuo a;

      for(int j=0;j<10;j++){
        int valor = rand() % 10;

        while(s.count(valor))
          valor = rand() % 10;

        a.v[j] = valor;
        s.insert(valor);
      }

      a.fitness = fitness(a);
      P[i] = a;
    }

    // "GERACOES" geracoes:
    for(int i=1;i<=GERACOES;i++){
      if(s1) selecaoRoleta(TAXACROSS, TAMANHO);
      else selecaoTorneio(TAXACROSS, TAMANHO);

      if(m1) mutacao(TAMANHO, TAXAMUT1, TAXACROSS);
      else mutacao(TAMANHO, TAXAMUT2, TAXACROSS);
      
      double melhor;
      if(r1)
        melhor = reinsercaoPuraXElitismo(TAMANHO, TAXAELIT, TAXACROSS);
      else
        melhor = reinsercaoOrdenada(TAMANHO, TAXACROSS);

      if(melhor == 0){
        sucesso++;
        break;
      }
    }
  }
  printf("Taxa de Sucesso: %.2lf%c\n", (sucesso*100.0)/REPETICOES, '%');
  printf("Tempo de Execucao: %ds\n", time(NULL) - ini);
  return 0;
}

// 0 1 2 3 4 5 6 7
// s e n d m o r y _ _
void print(Individuo a){
  for(int i=0;i<10;i++)
    printf("%d ", a.v[i]);
  printf("\n");

  printf(" %d%d%d%d\n", a.v[0], a.v[1], a.v[2], a.v[3]);
  printf("+%d%d%d%d\n", a.v[4], a.v[5], a.v[6], a.v[1]);
  printf("%d%d%d%d%d\n", a.v[4], a.v[5], a.v[2], a.v[1], a.v[7]);
}

int fitness(Individuo a){
  int send = a.v[0]*1000 + a.v[1]*100 + a.v[2]*10 + a.v[3];
  int more = a.v[4]*1000 + a.v[5]*100 + a.v[6]*10 + a.v[1];
  int money = a.v[4]*10000 + a.v[5]*1000 + a.v[2]*100 + a.v[1]*10 + a.v[7];

  return abs(send+more-money);
}

int compara(Individuo a, Individuo b){
  return a.fitness < b.fitness;
}

void mutacao(int tam, int taxa, int filhos){
  for(int i=tam;i<tam+filhos;i++){
    int valor = rand() % 101;

    if(valor <= taxa){
      int pos1 = rand() % 10, pos2;
      do{
        pos2 = rand() % 10;        
      } while(pos2 == pos1);

      swap(P[i].v[pos1], P[i].v[pos2]);
    }
  }

  return;
}

pair<Individuo, Individuo> crossover(pair<Individuo, Individuo> par){
  Individuo a = par.first;
  Individuo b = par.second;

  int c[]={1,1,1,1,1,1,1,1,1,1}; int cont = 10;
 
  int pos = rand() % 10;
  do{
    c[a.v[pos]]--;
    if(c[a.v[pos]] == 0)
      cont--;

    c[b.v[pos]]++;
    if(c[b.v[pos]] == 1)
      cont++;
    
    swap(a.v[pos], b.v[pos]);
    for(int i=0;i<10;i++){
      if(i != pos && a.v[pos] == a.v[i]){
        pos = i;
        break;
      }
    }
  } while(cont != 10);

  return make_pair(a, b);
}

void selecaoRoleta(int quant, int tam){
  vector<Individuo> selecionados;
    
  sort(P, P+tam, compara);

  int soma = 0;
  for(int i=0;i<tam;i++)
    soma += APTMAX - P[i].fitness;

  for(int i=0;i<quant;i++){
    int valor = (rand()) % soma;
    valor = (valor * valor) % soma;
    int cont = 0;

    int j=0;
    while(cont < valor){
      cont += APTMAX - P[j].fitness;
      j = (j + 1) % tam;
    }

    selecionados.push_back(P[j]);
  }
  for(int i=0;i<selecionados.size()-1;i+=2){
    pair<Individuo, Individuo> par = crossover(make_pair(selecionados[i], selecionados[i+1]));
    par.first.fitness = fitness(par.first);
    P[tam+i] = par.first;
    par.second.fitness = fitness(par.second);
    P[tam+i+1]= par.second;
  }

  return;
}

void selecaoTorneio(int quant, int tam){
  set<int> s;
  vector<Individuo> selecionados;

  for(int i=0;i<quant;i++){
    s.clear();

    while(s.size() < 3){
      int valor = rand() % tam;
      while(s.count(valor))
        valor = rand() % tam;
    
      s.insert(valor);
    }

    int melhor;
    for(set<int>::iterator it = s.begin(); it != s.end();it++)
      if(it == s.begin() || P[*it].fitness < P[melhor].fitness)
        melhor = *it;
  
    selecionados.push_back(P[melhor]);
  }

  for(int i=0;i<selecionados.size()-1;i+=2){
    pair<Individuo, Individuo> par = crossover(make_pair(selecionados[i], selecionados[i+1]));
    par.first.fitness = fitness(par.first);
    P[tam+i] = par.first;
    par.second.fitness = fitness(par.second);
    P[tam+i+1]= par.second;
  }

  return;
}

int reinsercaoPuraXElitismo(int tam, int taxa, int filhos){
  sort(P, P+tam, compara);

  int melhor = P[0].fitness;
  // Insere os filhos
  for(int i=taxa;i<taxa+filhos;i++){
    P[i] = P[tam+i-taxa];
    melhor = min(melhor, P[i].fitness);
  }

  return melhor;
}

int reinsercaoOrdenada(int tam, int filhos){
  sort(P, P+tam+filhos, compara);
  return P[0].fitness;
}