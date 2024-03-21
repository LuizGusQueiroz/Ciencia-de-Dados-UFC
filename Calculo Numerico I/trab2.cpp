/*
Segundo trabalho do meu curso de Métodos Numéricos 1
Objetivos:
->Implementar algoritmo para escalonar matrizes pelo método de Gauss normal
->Implementar algoritmo para escalonar matrizes pelo método de Gauss Jordan
->Implementar algoritmo para calcular determinante de matrizes pela regra de Cramer

Luiz Gustavo Queiroz de Aguiar Vasconcelos - 537314

*/
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <locale>

//dimensão da matriz que será utilizada
const int linhas = 3; 
const int colunas = 3;

using namespace std;

//funções referentes ao método de Gauss

void recebeMatriz(float matriz[linhas][colunas], float vetSol[linhas]);
void preencheMatriz(float matriz[linhas][colunas], float vetSol[linhas]);
void copiaMatriz(float matriz[linhas][colunas], float matrizBase[linhas][colunas], float vetSol[linhas], float vetSolBase[linhas]);
void imprimeMatriz(float matriz[linhas][colunas], float vetSol[linhas], bool simples);
void gauss(float matriz[linhas][colunas], float vetSol[linhas], bool jordan);
void trocaLinha(float matriz[linhas][colunas], float vetSol[linhas], int lx, int ly);
void zeraAbaixo(float matriz[linhas][colunas], float vetSol[linhas], int linha, int coluna);
void zeraColuna(float matriz[linhas][colunas], float vetSol[linhas], int linha, int coluna, bool jordan);
void reduzPivo(float matriz[linhas][colunas], float vetSol[linhas], int linha, int coluna);

//funções referentes à regra de Cramer

int determinante(float matriz[linhas][colunas]);
int determinante(float matriz[linhas][colunas], int dimensao, int linha, int coluna);
void cramer(float matriz[linhas][colunas], float vetSol[linhas], float valorDasVar[linhas]);
float cramer(float matriz[linhas][colunas], float vetSol[linhas], int coluna);
void trocaColuna(float matriz[linhas][colunas], float vetSol[linhas], int coluna);
void imprimeVetor(float vetor[linhas]);

//funcão responsável pela análise final do problema

void analise(float valorDasVar[linhas], float a, float amplMax);


int main() {
  setlocale(LC_ALL, "portuguese");
  //declaração das variáveis que serão usadas
  bool padrao, simples = 0;
  float matriz[linhas][colunas], matrizBase[linhas][colunas];
  float vetSol[linhas], vetSolBase[linhas], valorDasVar[linhas];
  float a, amplMax = 3;
  int det;

  
  cout << "Digite :\n0 para colocar os valores manualmente\n1 para usar os valores padrão da questão\n";
  cin >> padrao;

    
  if(padrao)
      a = 1;
  else{
      cout << "Digite o valor da parâmetro a : ";
      cin >> a;
      recebeMatriz(matrizBase, vetSolBase);
  }

    
  if(padrao)
      //se padrão, preenche a matriz com os valores padrão da questão
      preencheMatriz(matriz, vetSol);
  else
      //caso contrário, preenche a matriz com os valores informados pelo usuário
      copiaMatriz(matriz, matrizBase, vetSol, vetSolBase);

  //impressão da matriz inicial
  cout << "\nMatriz inicial : ";
  imprimeMatriz(matriz, vetSol, simples);

  //matriz resolvida pelo método de Gauss
  gauss(matriz, vetSol, 0);
  cout << "\nMatriz escalonada pelo método de Gauss normal : ";
  imprimeMatriz(matriz, vetSol, simples);

  //redefine a matriz
  if(padrao)
      preencheMatriz(matriz, vetSol);
  else
      copiaMatriz(matriz, matrizBase, vetSol, vetSolBase);

  //matriz resolvida pelo método de Gauss Jordan
  gauss(matriz, vetSol, 1);
  cout << "\nMatriz escalonada pelo método de Gauss Jordan : ";
  imprimeMatriz(matriz, vetSol, simples);

  //redefine a matriz
  if(padrao)
      preencheMatriz(matriz, vetSol);
  else
      copiaMatriz(matriz, matrizBase, vetSol, vetSolBase);

  //cálculo do determinante da matriz
  det = determinante(matriz);
  printf("\nDeterminante da matriz : %i\n", det);
  //cálculo do vetor solução pelo método de Cramer
  cramer(matriz, vetSol, valorDasVar);

  //análise dos resultados obtidos
  analise(valorDasVar, a, amplMax);

  return 0;
}

    
void recebeMatriz(float matriz[linhas][colunas], float vetSol[linhas]){
  float valor;
  for(int i = 0; i < linhas; i++){   //percorre as linhas da matriz
      for(int j = 0; j < colunas; j++){   //percorre as colunas da linha e recebe os seus valores
          printf("Digite a posição [%i][%i] da matriz : ", i+1, j+1);
          cin >> valor;
          matriz[i][j] = valor;
      }
  }
  for(int i = 0; i < linhas; i++){   //percorre as posições do vetor
      printf("Digite a posição [%i] do vetor solução : ", i+1);
      cin >> valor;
      vetSol[i] = valor;
  }
}

void preencheMatriz(float matriz[linhas][colunas], float vetSol[linhas]){
  //preenche a matriz e o vetor solução com os valores padrão da questão
  for(int i = 0; i < linhas; i++)
      for(int j = 0; j < colunas; j++)
          matriz[i][j] = (i==j)?10:1;
  for(int i = 0; i < linhas; i++)
      vetSol[i] = 12;
}

void copiaMatriz(float matriz[linhas][colunas], float matrizBase[linhas][colunas], float vetSol[linhas], float vetSolBase[linhas]){
    for(int i = 0; i < linhas; i++){   //uma matriz e um vetor solução se tornam uma cópia
        //de uma matriz e de um vetor solução base
        for(int j = 0; j < colunas; j++)
            matriz[i][j] = matrizBase[i][j];
        vetSol[i] = vetSolBase[i];
    }
}

void imprimeMatriz(float matriz[linhas][colunas], float vetSol[linhas], bool simples){
  //imprime os elementos da matriz e do vetor solução
  cout << endl;
  if(simples)
      for(int i = 0; i < linhas; i++){
          cout << "[";
          for(int j = 0; j < colunas; j++)
              cout << matriz[i][j] << " ";
          cout << " = " << vetSol[i] << "]\n";
      }
  else
      for(int i = 0; i < linhas; i++){
        cout << "[";
        for(int j = 0; j < colunas; j++)
            printf("% 9.7f  ", matriz[i][j]);
        printf("=% 6.5f]\n", vetSol[i]);
      }
  cout << endl;
}

void gauss(float matriz[linhas][colunas], float vetSol[linhas], bool jordan){
  int colNulas = 0;
  //variável para caso hajam colunas nulas, o pivo não
  //sej mais em posições em que i = j
  for(int i = 0; i < linhas; i++)
      //percorre as linhas da matriz
      for(int j = 0; j < colunas; j++)
      //percorre as colunas da matriz
          if(i == j+colNulas){//verifica se é uma posição pivô
              bool colNula = false;
              //assume que a coluna que parte da posição pivo para baixo não é nula
              if(matriz[i][j+colNulas] == 0){//verifica se a posição pivô armazena um valor nulo
                  colNula = true;
                  //se o elemento da posição pivô for nulo assume que a coluna é nula
                  for(int ii = i+1; ii < linhas; ii++)
                      //percorre as linhas abaixo da linha que contém o elemento observado
                      if(matriz[ii][j+colNulas] != 0){//testa elemento por elemento se é não nulo
                          trocaLinha(matriz, vetSol, i, ii);
                          //se encontrar um não nulo, troca a linha do elemento nulo
                          //pela linha do elemento não nulo encontrado
                          colNula = false;
                          //a coluna volta a ser considerada não nula
                          break;
                          //encerra a busca por um elemento não nulo
                          //encerrando o laço de repetição
                      }//fim da busca por elemento não nulos
              }//fim da verificação do elemento na posição pivô
              if(colNula)
                  //se a coluna for nula, a variável colNulas aumenta
                  //para corrigir a posição dos pivôs seguintes
                  colNulas++;
              else
                  //se a coluna não for nula, zera todos os elementos da coluna (exceto o
                  //elemento pivô) por meio de operações elementares entre matrizes
                  zeraColuna(matriz, vetSol, i, j, jordan);
          }
}

void trocaLinha(float matriz[linhas][colunas], float vetSol[linhas], int lx, int ly){
  float varAux; //variável auxiliar para ajudar na troca dos elementos
  for(int j = 0; j < colunas; j++){//percorre todos as colunas da matriz
      varAux = matriz[lx][j]; //guarda o valor da posição [X][J] na variável auxiliar
      matriz[lx][j] = matriz[ly][j]; //guarda o valor da posição [Y][J] na posicao [X][J]
      matriz[ly][j] = varAux; //guarda o valor da variável auxiliar na posição [Y][J]
  } 
  //troca as posições equivalentes no veto solução
  varAux = vetSol[lx];
  vetSol[lx] = vetSol[ly];
  vetSol[ly] = varAux;
}

void zeraColuna(float matriz[linhas][colunas], float vetSol[colunas], int linha, int coluna, bool jordan){
  reduzPivo(matriz, vetSol, linha, coluna);
  //reduz o pivô, deixando ele igual a 1
  int inicio = (jordan)?0:linha+1;
  //se jordan começa na primeira linha da matriz
  //caso contrário, começa na linha abaixo do pivô
  for(int i = inicio; i < linhas; i++) //percorre as linhas do vetor
      if(i != linha){//verifica se é a linha do elemento pivô
          float multiplicador = matriz[i][coluna] / matriz[linha][coluna];
          //encontra o multiplicador da linha analisada
          for(int j = 0; j < colunas; j++)
              //percorre as colunas da linha analisada
              //realizando a operação elementar
              matriz[i][j] -= multiplicador * matriz[linha][j];
          vetSol[i] -= multiplicador * vetSol[linha];
          //faz a operação elementar no vetor solução
      }
}

void reduzPivo(float matriz[linhas][colunas], float vetSol[colunas], int linha, int coluna){
  float multiplicador = 1/matriz[linha][coluna];
  for(int j = 0; j < colunas; j++)
      //multiplica toda a linha do pivô pelo seu inverso
      matriz[linha][j] *= multiplicador;
  vetSol[linha] = vetSol[linha] * multiplicador;
  //multiplica a posição correspondente do vetor solução pelo inverso do pivô
}

int determinante(float matriz[linhas][colunas]){
  int dimensao = linhas; //variável para saber se a matriz já é 2x2
  int linha = 0;  //linha observada
  int coluna = 0;  //coluna observada
  return determinante(matriz, dimensao, linha, coluna);
  //chama a função recursiva
}

int determinante(float matriz[linhas][colunas], int dimensao, int linha, int coluna){
  float det = 0; //cria a variável que será o resultado
  if(dimensao > 2) 
      for(int j = 0; j < colunas; j++)
          //percorre as colunas da matriz e calcula o determinante recursivamente
          det += matriz[linha][j] * pow(-1, linha+j+2) * determinante(matriz, dimensao-1, linha+1, j);
  else
      //retorna o valor de cada submatriz 2x2 possível para o problema
      if(coluna == 0)
          det = matriz[linha][1] * matriz[linha+1][2] - matriz[linha][2] * matriz[linha+1][1];
      else if(coluna == 1)
          det = matriz[linha][0] * matriz[linha+1][2] - matriz[linha][2] * matriz[linha+1][0];
      else
          det = matriz[linha][0] * matriz[linha+1][1] - matriz[linha][1] * matriz[linha+1][0];
  return det;
}

void cramer(float matriz[linhas][colunas], float vetSol[linhas], float valorDasVar[linhas]){
  for(int i = 0; i < linhas; i++)
      //percorre as linhas da matriz
      valorDasVar[i] = cramer(matriz, vetSol, i);
      //coloca o valor de cada variável em um vetor
  cout << "\nVetor solução obtido pela regra de Cramer : ";
  imprimeVetor(valorDasVar); //imprime o valor das variáveis
}

float cramer(float matriz[linhas][colunas], float vetSol[linhas], int coluna){
  trocaColuna(matriz, vetSol, coluna);
  //troca a coluna C pelo vetor solução para encontrar o subdeterminante
  float subDet = determinante(matriz);
  
  trocaColuna(matriz, vetSol, coluna);
  //destroca a coluna pelo vetor solucao, retornando a matriz original
  return subDet/determinante(matriz);
}

void trocaColuna(float matriz[linhas][colunas], float vetSol[linhas], int coluna){
  float varAux; //variável auxiliar para ajudar na troca dos elementos
  for(int i = 0; i < linhas; i++){//percorre cada linha trocando os elementos da coluna C
      //pelo elemento i do vetor solução
      varAux = matriz[i][coluna];
      matriz[i][coluna] = vetSol[i];
      vetSol[i] = varAux;
  }
}

void imprimeVetor(float vetor[linhas]){
  cout << endl;
  for(int i = 0; i < linhas; i++)
      //percorre cada posição do vetor e imprime seu valor
      cout << "[" << vetor[i] << "]\n";
}

void analise(float valorDasVar[linhas], float a, float amplMax){
  for(int i = 0; i < linhas; i++)
      if(a * valorDasVar[i] > amplMax){//percorre as posições do vetor, calcula a amplitude de deslocamento
          //e calcula se é maior que a amplitude máxima
          printf("\nA amplitude de deslocamente d%i é superior a 3cm.\nLogo, os pêndulos poderão romper.\n", i+1);
          return;
      }
  cout << ("\nNenhuma amplitude de deslocamento foi superior a 3cm.\nLogo, nenhum pêndulo corre risco de romper.\n");
        
}
