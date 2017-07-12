/* 
  Calcula o menor percurso TSP em O(n!), usado pra conferir os testes do ACO
*/

#include <bits/stdc++.h>

#define eps 1e-9
#define ft first
#define sd second
#define inf 0x3f3f3f3f
#define x first
#define y second

using namespace std;

int N;

bool vis[12][(1<<12)];
double dp[12][(1<<12)];
double distancia[50][50];
pair<double, double> v[50];

double dist(pair<double, double> a, pair<double, double> b){
    return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

bool leEntrada(){
    int tipo;
    scanf("%d", &tipo);

    if(tipo == 1){
        scanf("%d", &N);

        int menory = 0, maiory = 0;
        for(int i=0;i<N;i++){
            scanf("%lf %lf", &v[i].x, &v[i].y);
            for(int j=0;j<i;j++)
                distancia[i][j] = distancia[j][i] = dist(v[i], v[j]);
        
            if(v[i].y < v[menory].y)
              menory = i;
            if(v[i].y > v[maiory].y)
              maiory = i;
        }
        // printf("oi %.4lf %.4lf\n", v[menory].x, v[menory].y);
        // printf("oi %.4lf %.4lf\n", v[maiory].x, v[maiory].y);
        return 1;
    }
    else if(tipo == 2){
        scanf("%d", &N);

        for(int i=0;i<N;i++){
            for(int j=i+1;j<N;j++){
                scanf("%lf", &distancia[i][j]);
                distancia[j][i] = distancia[i][j];
            }
        }
        return 1;
    }
    return 0;
}


double tsp(int i, int mask)
{
  if(mask == (1<<N)-1)
    return dp[i][mask] = distancia[0][i];
  if(vis[i][mask])
    return dp[i][mask];

  double ans = inf;
  for(int j=0;j<N;j++)
    if(!(mask & (1<<j)))
      ans = min(ans, tsp(j, mask | (1<<j)) + distancia[i][j]);
    
  vis[i][mask] = true;
  return dp[i][mask] = ans;
}

void imprime(int i, int mask){
  if(mask == (1<<N)-1){
    printf(" %d]\n", 0);
    return;
  }

  double ans = inf;
  for(int j=0;j<N;j++){
    if(!(mask & (1<<j)) && dp[i][mask] >= dp[j][mask | (1<<j)] + distancia[i][j] - eps && dp[i][mask] <= dp[j][mask | (1<<j)] + distancia[i][j] + eps){
      printf(" %d ->", j);
      imprime(j, mask | (1<<j));
      break;
    }
  }
}

main()
{
  leEntrada();
  printf("Comprimento do menor percurso: %lf\n", tsp(0, 1));

  printf("[0 -> ");
  imprime(0, 1);
}