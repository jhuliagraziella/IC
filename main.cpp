#include <bits/stdc++.h>

#define MAXN 50
#define alpha 1 // taxa de importancia da trilha de feromonio
#define beta 1  // taxa de importancia da heuristica local
#define ro 1    // taxa de evaporacao
#define Q 1     // quantidade de feromonio depositado por cd formiga a cd iteracao

int N;
bool vis[MAXN][MAXN];               // vetor de visitados: se a formiga i visitou a cidade j
int posicao[MAXN];                  // onde a formiga i esta
double probabilidade[MAXN][MAXN];   // probabilidade da formiga i ir pra cidade j
double feromonio[MAXN][MAXN];       // quantidade de feromonio na estrada (i, j)
double dferomonio[MAXN][MAXN];      // delta feromonio -> variacao do feromonio na estrada (i, j)
double distancia[MAXN][MAXN];       // distancia entre as cidades (i, j)

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

// => eh usada roleta sobre a matriz de probabilidades pra definir aonde q a porra da formiga vai

void geraCaminhos(){ // gera o caminho de cada porra de formiga

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

}   