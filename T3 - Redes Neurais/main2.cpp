#include <bits/stdc++.h>

#define NUMOBJ 2
#define NUMNEU 2
#define NUMPAD 2
#define TAM 30
#define LINHAS 6
#define COLUNAS 5
#define MODIFICADOS 10

using namespace std;

double p[][30]= {{0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0},
                 {0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1},
                 {0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1},
                 {0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1},
                 {1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
                 {0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1}};

double mod[][10][30] = {{{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0},
                         {0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0},
                         {0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0},
                         {0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0},
                         {0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0},
                         {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0},
                         {0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},
                         {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},
                         {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
                         {0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0}},

                        {{0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0},
                         {0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0},
                         {0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0},
                         {0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0},
                         {0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1},
                         {0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0},
                         {0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1},
                         {0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1},
                         {1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1},
                         {1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1}}};

main(){
    double neuronio[6][TAM];
    bool random = 1;

    // printf("Deseja que a inicializacao seja feita de forma aleatoria? Digite 1 se sim, 0 caso contrario: ");
    // scanf("%d", &random);

    srand(time(NULL));
    for(int i=0;i<NUMNEU;i++)
        for(int j=0;j<TAM;j++)
            neuronio[i][j] = random ? (-1.0 + (rand() % 4) / 2.0) : 0.0;

    for(int i=0;i<NUMNEU;i++){
        printf("Neuronio %d -> [%.2lf", i, neuronio[i][0]);
        for(int j=1;j<TAM;j++)
            printf(", %.2lf", neuronio[i][j]);
        printf("]\n");
    }

    int erros;
    int epoca = 0;
    do{
        erros = 0;
        epoca++;
        for(int obj=0;obj<NUMNEU;obj++){
            // to considerando o neuronio de id obj, ele tem q dar 1 comparado com obj e 0 comparado com tds os outros
            for(int i=0;i<NUMPAD;i++){
                double soma = 0;
                for(int j=0;j<TAM;j++)
                    soma += neuronio[obj][j] * p[i][j];
    
                soma = (soma > 0);
                if(soma == (i == obj))  // o resultado ta ok
                    continue;

                erros++;
                for(int j=0;j<TAM;j++)
                    neuronio[obj][j] += (soma < (i==obj) ? 1 : -1) * p[i][j]; 
            }
        }
    } while(erros > 0);

    printf("Numero de neuronios: %d\n", NUMNEU);
    printf("Numero de padroes: %d\n", NUMPAD);
    printf("Numero de epocas: %d\n", epoca);
    printf("Vetores de pesos:\n");


    for(int i=0;i<NUMNEU;i++){
        printf("Neuronio %d -> [%.2lf", i, neuronio[i][0]);
        for(int j=1;j<TAM;j++)
            printf(", %.2lf", neuronio[i][j]);
        printf("]\n");
    }

    for(int i=0;i<NUMOBJ;i++){ // qtdd de objetos modificados q vou testar
        for(int j=0;j<MODIFICADOS;j++){ // qtdd de modificacoes de cd obj -> to vendo a modificacao j do obj i
            // transformar o soma num vetor e mudar o bagulhinho q verifica
            erros = 0;
            for(int k=0;k<NUMPAD;k++){
                double soma = 0;
                for(int ii=0;ii<LINHAS;ii++){
                    for(int jj=0;jj<COLUNAS;jj++){
                        if(k == 0)
                            printf("%c", mod[i][j][ii*COLUNAS+jj] == 1 ? 'x' : 'o');
                        soma += mod[i][j][ii*COLUNAS+jj] * neuronio[k][ii*COLUNAS+jj];
                    }
                    if(k == 0)
                        printf("\n");
                }
                soma = (soma > 0);
                if(soma == (i == k))  // o resultado ta ok
                    continue;

                erros++;
            }
            printf("O padrao %d %s reconhecido.\n\n", i, (!erros) ? "foi" : "nao foi"); 
        }
    }


    for(int outros=2;outros<6;outros++){
        int ans = -1;
        int cont = 0;
        for(int k=0;k<NUMPAD;k++){
            double soma = 0;
            for(int i=0;i<TAM;i++)
                soma += p[outros][i] * neuronio[k][i];

            soma = (soma > 0);
            
            if(soma == k){
                ans = k;
                cont++;
            }
            // printf("%d %d %lf\n", outros, k, soma);
        }
        // printf("serio? %d %d\n", cont, ans);

        printf("O padrao %d foi reconhecido como %d.\n\n", outros, cont == 1 ? ans : -1); 
    }
}