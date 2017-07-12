#include <bits/stdc++.h>

#define REPETICOES 10
#define TAMANHO 50
#define GERACOES 50
#define TAXACROSS 50
#define TAXAMUT 20
#define APTMAX 100000
#define PESOMAXIMO 0.3
#define CLASSEESTUDO 5
#define NUMGENES 34
#define TAMTREINO 239
#define TAMTESTE 119

using namespace std;

string op[] = {"=", "!=", ">=", "<"};
minstd_rand0 random_generator;

typedef struct gene{
    double peso;        // valor em [0,1] q define se isso faz parte da regra ou nao
    string operador;    // =, !=, >= e <
    int valor;          // valor de 0 a 3, excecao = idade ou historico familiar

    gene() {}
    gene(double p, string op, int v){
        this->peso = p;
        this->operador = op;
        this->valor = v;
    }
} Gene;

typedef struct individuo{
    Gene g[34];
    int classe; // classe "estudada"
    int num_genes; 
    double aptidao;

    individuo(){}
    individuo(int classe, int ngenes){
        this->classe = classe;
        this->num_genes = ngenes;
    }
} Individuo;

typedef struct base{
    int tam_base;
    int num_genes;
    int **dados;

    base(){}
    base(int tamb, int numg){
        this->tam_base = tamb;
        this->num_genes = numg;
    
        this->dados = (int**) malloc (this->tam_base * sizeof (int*));
        for(int i=0;i<this->tam_base;i++)
            this->dados[i] = (int*) malloc ((this->num_genes+1) * sizeof (int));
    }
} Base;

void lerBases(Base *treino, Base *teste);
void individuoAleatorio(Individuo *a);

bool avalia(Gene gn, int comp);                                       // Avalia a funcao gn.valor gn.operador comp
double aptidao(Individuo *a, Base *b);                                // Avalia a aptidao do individuo a de acordo com a base de dados b

void selecaoTorneio(int quant, int tam, Individuo *p, Base *b);       // Seleciona quant individuos pelo metodo de Torneio de tamanho 3 e gera os filhos da geracao atual
void selecaoRoleta(int quant, int tam, Individuo* P, Base* b);
pair<Individuo, Individuo> crossover(Individuo a, Individuo b);       // Faz crossover entre 2 individuos
void mutacao(int tam, int taxaM, int filhos, Individuo* p, Base *b);  // Faz a mutacao dos dados do individuo, com taxa de mutacao taxaM

int compara(Individuo a, Individuo b);                                // Funcao de comparacao para ordenacao
void reinsercaoOrdenada(int tam, int filhos, Individuo* p);           // Cria a proxima geracao pelo metodo da reinsercao ordenada, retorna o fitness do melhor elemento

void imprime(Individuo a){
  for(int i=0;i<a.num_genes;i++){
    if(a.g[i].peso > PESOMAXIMO)
      continue;

    cout << i << " " << a.g[i].operador << " " << a.g[i].valor << "\n";
  }
}

main(){
  Individuo *P = (Individuo*) malloc((TAMANHO + TAXACROSS) * sizeof(Individuo));
  Base treino = Base(TAMTREINO, NUMGENES), teste = Base(TAMTESTE, NUMGENES);

  lerBases(&treino, &teste);

  int ini = time(NULL);
  Individuo melhorAll;
  for(int k=0;k<REPETICOES;k++){
    random_generator = std::minstd_rand0(std::chrono::system_clock::now().time_since_epoch().count());
    for(int i=0;i<TAMANHO;i++){      
      P[i] = Individuo(CLASSEESTUDO, NUMGENES);
      individuoAleatorio(&P[i]);
      P[i].aptidao = aptidao(&P[i], &treino);
    }
    sort(P, P+TAMANHO, compara);

    Individuo melhor;
    for(int i=1;i<=GERACOES;i++){
      selecaoTorneio(TAXACROSS, TAMANHO, P, &treino);
      mutacao(TAMANHO, TAXAMUT, TAXACROSS, P, &treino);
      reinsercaoOrdenada(TAMANHO, TAXACROSS, P);

      if(i == 1 || P[0].aptidao > melhor.aptidao)
        melhor = P[0];
    }

    printf("\nFitness treinamento: %lf\n", melhor.aptidao);

    for(int i=0;i<TAMANHO;i++){
      if(i == 0 || aptidao(&P[i], &teste) > melhor.aptidao){
        melhor = P[i];
        melhor.aptidao = aptidao(&P[i], &teste);
      }
    }

    if(k == 0 || melhor.aptidao > melhorAll.aptidao)
      melhorAll = melhor;

    printf("Fitness teste: %lf\n", melhor.aptidao);
    printf("Tempo de execucao: %d\n", time(NULL) - ini);
    ini = time(NULL);
  }

  printf("treinamento %lf\nteste %lf\n", aptidao(&melhorAll, &treino), melhorAll.aptidao);
  imprime(melhorAll);
  return 0;
}

void lerBases(Base *treino, Base *teste){
  freopen("dermatology.train", "r", stdin);
  for(int i=0;i<treino->tam_base;i++)
    for(int j=0;j<treino->num_genes+1;j++)
      scanf("%d", &treino->dados[i][j]);

  freopen("dermatology.test", "r", stdin);
  for(int i=0;i<teste->tam_base;i++)
    for(int j=0;j<teste->num_genes+1;j++)
      scanf("%d", &teste->dados[i][j]);    

  return;
}

int compara(Individuo a, Individuo b){
  return a.aptidao > b.aptidao;
}

void individuoAleatorio(Individuo *a){
  for(int i=0;i< a->num_genes; i++){
    a->g[i].peso = (random_generator() % 1001) / 1000.0;

    if(i == 10){ // historico familiar
      a->g[i].operador = op[random_generator() % 2];
      a->g[i].valor = random_generator() % 2;
    }
    else if(i == 33){ // idade
      a->g[i].operador = op[random_generator() % 2];
      a->g[i].valor = random_generator() % 80;
    }
    else{
      a->g[i].operador = op[random_generator() % 4];
      a->g[i].valor = random_generator() % 4;
    }
  }
}

void selecaoRoleta(int quant, int tam, Individuo* P, Base* b){
  Individuo selecionados[2];

  int somatot = 1;
  for(int i=0;i<tam;i++)
    somatot += P[i].aptidao * 1000;

  for(int i=0;i<quant;i++){
    int valor = random_generator() % somatot;
    int soma = 0;
    for(int j=0;j<tam;j++){
      soma += P[j].aptidao * 10000;
      if(soma >= valor){
        selecionados[i%2] = P[j];
        break;
      }
    }

    if(i % 2){
      pair<Individuo, Individuo> par = crossover(selecionados[0], selecionados[1]);
      par.first.aptidao = aptidao(&par.first, b);
      P[tam+i-1] = par.first;

      par.second.aptidao = aptidao(&par.second, b);
      P[tam+i] = par.second;
    }
  }

  return;
}

void selecaoTorneio(int quant, int tam, Individuo* P, Base* b){
  Individuo selecionados[2];

  int somatot = 1;
  for(int i=0;i<tam;i++)
    somatot += P[i].aptidao * 1000;

  for(int i=0;i<quant;i++){
    int melhor = -1;

    for(int j=0;j<3;j++){
      int valor = random_generator() % somatot;
      int soma = 0;
      for(int k=0;k<tam;k++){
        soma += P[k].aptidao * 10000;
        if(soma >= valor){
          if(melhor == -1 || P[k].aptidao > P[melhor].aptidao)
            melhor = k;
          break;
        }
      }
    }
    selecionados[i%2] = P[melhor];

    if(i % 2){
      pair<Individuo, Individuo> par = crossover(selecionados[0], selecionados[1]);
      par.first.aptidao = aptidao(&par.first, b);
      P[tam+i-1] = par.first;

      par.second.aptidao = aptidao(&par.second, b);
      P[tam+i] = par.second;
    }
  }

  return;
}

pair<Individuo, Individuo> crossover(Individuo a, Individuo b){ // crossover de 2 pts
    int p1 = random_generator() % a.num_genes;
    int p2 = random_generator() % b.num_genes;

    if(p1 > p2)
        swap(p1, p2);

    for(int i=p1;i<=p2;i++)
        swap(a.g[i], b.g[i]);

    return make_pair(a, b);
}

void reinsercaoOrdenada(int tam, int filhos, Individuo* P){
  sort(P, P+tam+filhos, compara);
}


bool avalia(Gene gn, int comp){
    if(gn.operador == "=")
        return comp == gn.valor;
    else if(gn.operador == "!=")
        return comp != gn.valor;
    else if(gn.operador == ">=")
        return comp >= gn.valor;
    else if(gn.operador == "<")
        return comp < gn.valor;
    return false;
}

double aptidao(Individuo *a, Base *b){
    int Tp = 0, Tn = 0, Fp = 0, Fn = 0;

    for(int i=0;i<b->tam_base;i++){
        bool A = true;
        for(int j=0;j<a->num_genes && A;j++){
            if(a->g[j].peso > PESOMAXIMO)
                continue;
            A &= avalia(a->g[j], b->dados[i][j]);
        }

        bool C = (a->classe == b->dados[i][b->num_genes]);

        Tp += (A & C);
        Fp += (A & !C);
        Tn += (!A & !C);
        Fn += (!A & C);
    }

    double sensi = (Tp + 1.0) / (Tp + Fn + 1.0);
    double espe = (Tn + 1.0) / (Tn + Fp + 1.0);

    return sensi*espe;
}

void mutacao(int tam, int taxaM, int filhos, Individuo *P, Base *b){
  for(int i=tam;i<tam+filhos;i++){
    for(int j=0;j<P[i].num_genes;j++){
      if(random_generator() % 101 > TAXAMUT)
        continue;

      double r = (random_generator()%201 -100) /100.0;
      P[i].g[j].peso += r;

      if(j == 10){
        P[i].g[j].operador = (P[i].g[j].operador == "=") ? "!=" : "=";
        P[i].g[j].valor = !P[i].g[j].valor;
      }
      else if(j == 33){
        P[i].g[j].operador = (P[i].g[j].operador == "=") ? "!=" : "=";
        P[i].g[j].valor = random_generator() % 80;
      }
      else{
        P[i].g[j].operador = op[random_generator() % 4];
        P[i].g[j].operador = random_generator() % 4;
      }
      aptidao(&P[i], b);
    }
  }
}