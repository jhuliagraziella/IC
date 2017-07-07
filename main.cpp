#include <bits/stdc++.h>

#define MAXN 50 // quantidade maxima de vertices
#define alpha 1 // taxa de importancia da trilha de feromonio
#define beta 1  // taxa de importancia da heuristica local
#define ro 1    // taxa de evaporacao
#define tau 1   // quantidade de feromonio inicial em cd aresta
#define Q 1     // quantidade de feromonio depositado por cd formiga a cd iteracao

#define verbose 0

#define x first
#define y second
#define inf 0x3f3f3f3f

using namespace std;

int N;
bool vis[MAXN][MAXN];               // vetor de visitados: se a formiga i visitou a cidade j
int posicao[MAXN];                  // onde a formiga i esta
double probabilidade[MAXN][MAXN];   // probabilidade da formiga i ir pra cidade j
double feromonio[MAXN][MAXN];       // quantidade de feromonio na estrada (i, j)
double dferomonio[MAXN][MAXN];      // delta feromonio -> variacao do feromonio na estrada (i, j)
double distancia[MAXN][MAXN];       // distancia entre as cidades (i, j)
double avaliacao[MAXN];             // avaliacao do melhor caminho da formiga i

vector<int> percurso[MAXN];         // percurso de cada formiga

pair<double, double> v[MAXN];       // posicao de cada vertice, caso a entrada seja feita por coordenadas

// so pra garantir q a matriz de distancias eh consistente: o caminho de i ate j eh sempre o menor, independente se eh direto ou nao
void floydWarshall(){
    for (int k=0;k<N;k++)
        for (int i=0;i<N;i++) 
          for (int j=0;j<N;j++)
            if (distancia[i][k] + distancia[k][j] < distancia[i][j])
                distancia[i][j] = distancia[i][k] + distancia[k][j];
}

double dist(pair<double, double> a, pair<double, double> b){
    return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

bool leEntrada(){
    int tipo;

    if(verbose){
        printf(" -------------------------------------------------------------- \n");
        printf("| Opcoes:                                                      |\n");
        printf("| 1) Fornecer as coordenadas (x, y) das cidades                |\n");
        printf("| 2) Fornecer o valor das distancias entre cada par de cidades |\n");
        printf(" -------------------------------------------------------------- \n");
        printf("\nEscolha uma opcao: ");
    }
    scanf("%d", &tipo);

    if(verbose)
        printf("Digite a quantidade de vertices: ");

    scanf("%d", &N);

    if(tipo == 1){
        for(int i=0;i<N;i++){
            if(verbose) 
                printf("Digite as coordenadas (x,y) da cidade %d, separadas por espaco: ", i);

            scanf("%lf %lf", &v[i].x, &v[i].y);
            for(int j=0;j<i;j++)
                distancia[i][j] = distancia[j][i] = dist(v[i], v[j]);
            distancia[i][i] = 0;
        }
        return 1;
    }
    else if(tipo == 2){
        for(int i=0;i<N;i++){
            for(int j=i+1;j<N;j++){
                if(verbose)
                    printf("Digite a distancia entre as cidades %d e %d\n", i, j);
                scanf("%lf", &distancia[i][j]);
                distancia[j][i] = distancia[i][j];
            }
            distancia[i][i] = 0;
        }
        return 1;
    }
    return 0;
}

/*
    CALCULO DAS PROBABILIDADES:

    p_kij(t) = vis[k][j] ? 0 : feromonio_ij ^ alpha * atratividade_ij ^ beta / denominador
    feromonio_ij: qtdd de feromonio na estrada (i, j) = feromonio[i][j]
    atratividade_ij: atratividade da cidade j a partir da cidade i = 1/distancia[i][j]
    alpha e beta sao parametros do algoritmo
    denominador: somatorio da formula de tds as cidades
*/

void geraProbabilidades(){  // atualiza a matriz de probabilidades das movimentacoes das formigas
    for(int k=0;k<N;k++){   // k eh a formiga q estamos analisando
        int i = posicao[k];

        double denominador = 0;
        for(int l=0;l<N;l++)
            denominador += vis[k][l] ? 0 : (pow(feromonio[i][l], alpha) / pow(distancia[i][l], beta));

        for(int j=0;j<N;j++)
            probabilidade[k][j] = vis[k][j] ? 0 : ((pow(feromonio[i][j], alpha) / pow(distancia[i][j], beta)) / denominador);
    }
    return;
}

// => eh usada roleta sobre a matriz de probabilidades pra definir aonde q a da formiga vai

void geraCaminhos(){ // gera o caminho de cada formiga
    for(int i=0;i<N;i++){ // nenhuma formiga visitou nenhum lugar
        for(int j=0;j<N;j++)
            vis[i][j] = 0;

        posicao[i] = rand() % N; // posicao inicial de cada formiga eh aleatoria
        percurso[i].clear();
        percurso[i].push_back(posicao[i]);
    }

    for(int passo=1;passo<N;passo++){ // quantidade de arestas do caminho
        geraProbabilidades();

        for(int k=0;k<N;k++){
            int caminho;

            // pra cada formiga k, tem q aplicar o metodo da roleta pra descobrir qual caminho q ela vai seguir
            // preciso de uma estrutura de dado melhor do q uma matriz de probabilidade pra poder fazer isso aqui,
            // mas n eh mto dificil de alterar
        
            percurso[k].push_back(caminho);
        }
    }
}

/*
    ATUALIZACAO DO FEROMONIO:

    -> evaporacao: evita q cresca demais / ajuda a esquecer decisoes ruins
    -> deposito: qtdd de feromonio deixada por formigas

    dferomonio_ij = somatorio pra cada formiga k de: (i,j) pertence ao caminho da formiga k ? f(percurso_k) : 0;
    feromonio_ij = (1.0-ro) * feromonio_ij + dferomonio_ij;
*/

void geraFeromonio(){ // atualiza a matriz de quantidade de feromonio em cada caminho (i,j)
    for(int i=0;i<N;i++)    // zera o delta do feromonio
        for(int j=0;j<N;j++)
            dferomonio[i][j] = 0;

    for(int k=0;k<N;k++){   // calcula o delta do feromonio
        double L = 0;       // comprimento do percurso da formiga k;
        for(int i=0;i<percurso[k].size();i++)
            L += distancia[percurso[k][i]][percurso[k][(i+1) % percurso[k].size()]];

        for(int i=0;i<percurso[k].size();i++)
            dferomonio[percurso[k][i]][percurso[k][(i+1) % percurso[k].size()]] += Q / L;
    }

    for(int i=0;i<N;i++)    // calcula a matriz de feromonio (aplica a variacao)
        for(int j=0;j<N;j++)
            feromonio[i][j] = (1.0-ro) * feromonio[i][j] + dferomonio[i][j];
    return;
}

main(){
    srand(time(NULL));

    if(!leEntrada()){
        if(verbose)
            printf("Falha na leitura da entrada, o programa sera finalizado\n");
        return 0;
    }

    floydWarshall();

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            dferomonio[i][j] = 0;
            feromonio[i][j] = tau;
        }
        avaliacao[i] = inf;
    }
}