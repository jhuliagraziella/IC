#include <bits/stdc++.h>

#define MAXN 4000        // quantidade maxima de vertices
#define MAXITERACOES 100 // quantidade maxima de iteracoes
#define MAXESTAGNADO 50  // quantdade de iteracoes "repetidas" suficientes para se considerar que estagnou na solucao

#define alpha 1          // taxa de importancia da trilha de feromonio
#define beta 5           // taxa de importancia da heuristica local
#define ro 0.5           // taxa de evaporacao
#define tau 1.0          // quantidade de feromonio inicial em cd aresta
#define Q 1              // quantidade de feromonio depositado por cd formiga a cd iteracao

#define verbose 0        // define se vao ser impressas msgs auxiliares ou nao

#define x first
#define y second
#define inf 0x3f3f3f3f

using namespace std;

int N;
bool mudou;

bool vis[MAXN][MAXN];               // vetor de visitados: se a formiga i visitou a cidade j
int posicao[MAXN];                  // onde a formiga i esta
double probabilidade[MAXN][MAXN];   // probabilidade da formiga i ir pra cidade j
double feromonio[MAXN][MAXN];       // quantidade de feromonio na estrada (i, j)
double dferomonio[MAXN][MAXN];      // delta feromonio -> variacao do feromonio na estrada (i, j)
double distancia[MAXN][MAXN];       // distancia entre as cidades (i, j)
bool pertence[MAXN][MAXN];          // marca se a aresta (i, j) pertence ao melhor percurso (pra ver onde deposita feromonio)

vector<int> percurso[MAXN];         // percurso de cada formiga

double LMelhorPercurso;             // comprimento do melhor percurso
vector<int> melhorPercurso;         // melhor percurso encontrado ate agora

pair<double, double> v[MAXN];       // posicao de cada vertice, caso a entrada seja feita por coordenadas

int aux;                            // variavel auxiliar na aplicacao da roleta
int ordem[MAXN];                    // vetor auxiliar na aplicacao da roleta

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
    if(verbose){
        printf(" -------------------------------------------------------------- \n");
        printf("| Opcoes:                                                      |\n");
        printf("| 1) Fornecer as coordenadas (x, y) das cidades                |\n");
        printf("| 2) Fornecer o valor das distancias entre cada par de cidades |\n");
        printf(" -------------------------------------------------------------- \n");
        printf("\nEscolha uma opcao: ");
    }

    int tipo;
    scanf("%d", &tipo);

    if(tipo == 1){
        if(verbose)
            printf("Digite a quantidade de vertices: ");
        scanf("%d", &N);

        for(int i=0;i<N;i++){
            if(verbose) 
                printf("Digite as coordenadas (x,y) da cidade %d, separadas por espaco: ", i);

            // int lixo;
            // scanf("%d", &lixo);

            scanf("%lf %lf", &v[i].x, &v[i].y);
            for(int j=0;j<i;j++)
                distancia[i][j] = distancia[j][i] = dist(v[i], v[j]);
        }
        return 1;
    }
    else if(tipo == 2){
        if(verbose)
            printf("Digite a quantidade de vertices: ");
        scanf("%d", &N);

        for(int i=0;i<N;i++){
            for(int j=i+1;j<N;j++){
                if(verbose)
                    printf("Digite a distancia entre as cidades %d e %d\n", i, j);
                scanf("%lf", &distancia[i][j]);
                distancia[j][i] = distancia[i][j];
            }
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

        // obs: somando 1 pra impedir denominador = 0
        double denominador = 0; 
        for(int l=0;l<N;l++)
            if(i != l)
                denominador += vis[k][l] ? 0 : (1 + pow(feromonio[i][l], alpha) / pow(distancia[i][l]+1, beta));
    
        for(int j=0;j<N;j++)
            if(i != j)
                probabilidade[k][j] = vis[k][j] ? 0 : ((1 + pow(feromonio[i][j], alpha) / pow(distancia[i][j]+1, beta)) / denominador);
        probabilidade[k][i] = 0;
    }
    return;
}

bool compara(int a, int b){
    return probabilidade[aux][a] > probabilidade[aux][b];
}

void geraCaminhos(){ // gera o caminho de cada formiga
    for(int i=0;i<N;i++){                // a posicao inicial de cada formiga eh aleatoria
        posicao[i] = rand() % N; 
        percurso[i].clear();
        percurso[i].push_back(posicao[i]);
        ordem[i] = i;

        for(int j=0;j<N;j++)
            vis[i][j] = 0;
    }

    for(int passo=1;passo<N;passo++){    // quantidade de arestas do caminho
        geraProbabilidades();

        for(int k=0;k<N;k++){
            vis[k][posicao[k]] = true;   // marca aonde a formiga ja esteve

            aux = k;                     // qual formiga to usando de base agr
            sort(ordem, ordem+N, compara);

            int soma = 0;
            for(int i=0;i<N;i++)
                soma += probabilidade[k][ordem[i]] * 100;

            int caminho = 0, cont = 0, valor = rand() % soma;
            while(cont < valor){
                cont += int(probabilidade[k][ordem[caminho]] * 100);
                caminho++;
            }
            if(cont > valor)
                caminho--;

            posicao[k] = ordem[caminho]; // nova posicao da formiga
            percurso[k].push_back(posicao[k]);
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

        if(L < LMelhorPercurso){
            for(int i=0;i<melhorPercurso.size();i++)
                pertence[melhorPercurso[i]][melhorPercurso[(i+1) % melhorPercurso.size()]] = false;

            mudou = true;
            LMelhorPercurso = L;
            melhorPercurso = percurso[k];

            for(int i=0;i<percurso[k].size();i++)
                pertence[percurso[k][i]][percurso[k][(i+1) % percurso[k].size()]] = true;            
        }

        for(int i=0;i<percurso[k].size();i++)
            if(pertence[percurso[k][i]][percurso[k][(i+1) % percurso[k].size()]])
                dferomonio[percurso[k][i]][percurso[k][(i+1) % percurso[k].size()]] += Q / L;
    }

    for(int i=0;i<N;i++)    // calcula a matriz de feromonio (aplica a variacao)
        for(int j=0;j<N;j++)
            feromonio[i][j] = (1.0-ro) * feromonio[i][j] + dferomonio[i][j];
    return;
}

int main(){
    srand(time(NULL));

    if(!leEntrada()){
        if(verbose)
            printf("Falha na leitura da entrada, o programa sera finalizado.\n");
        return 0;
    }

    // floydWarshall();

    // for(int i=0;i<N;i++){
    //     for(int j=i+1;j<N;j++)
    //         printf("%.4lf ", distancia[i][j]);
    //     printf("\n");
    // }

    // inicializa a quantidade de feromonio de cada aresta
    LMelhorPercurso = inf;
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            feromonio[i][j] = tau / (N*N);

    int rep = 0;
    int estagnado = 0;
    do{
        rep++;

        mudou = false;
        geraCaminhos();
        geraFeromonio();

        estagnado = mudou ? 0 : (estagnado+1);

        // printf("%d) Melhor solucao encontrada: \n[", rep, melhorPercurso[0]);
        // for(int i=0;i<melhorPercurso.size();i++)
        //     printf("%d -> ", melhorPercurso[i]);
        // printf("%d]\nComprimento: %.4lf\n\n", melhorPercurso[0], LMelhorPercurso);

    } while(rep < MAXITERACOES && estagnado < MAXESTAGNADO);

    printf("Melhor solucao encontrada: \n[", melhorPercurso[0]);
    for(int i=0;i<melhorPercurso.size();i++)
        printf("%d -> ", melhorPercurso[i]);
    printf("%d]\n\nComprimento: %.4lf\n", melhorPercurso[0], LMelhorPercurso);

    return 0;
}