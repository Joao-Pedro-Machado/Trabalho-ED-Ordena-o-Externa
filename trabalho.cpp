#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

/*
    Lógica do mergeSort:
        [x] Separar o arquivo original em f1 e f2  ordenando os pacotes
        [] Gerar dois arquivos de saída
        [] Iniciar a intercalação
            [] s1 e s2 tem que possuir blocos de tamanho dobrado em relação as entradas
            [] Os arquivos vazios viram saidas e os cheios viram entradas
            [] repetir até formar 1 bloco >= 100.000 ou todos vazios exceto 1

    Obs: O numero de pacotes só pode ser diferente em 1, em relação aos outros
*/



// dados armazenados no arquivo
struct pacote
{
    unsigned indice;
    float tempo;
    char origem[40];
    char destino[40];
    char protocolo[18];
    unsigned tamanho;
    char informacao[100];
};

void merge(pacote *vetor, int inicio, int meio, int fim)
{
    int i, j, k;
    int tam = fim - inicio + 1;
    pacote *aux = new pacote[tam];
    i = inicio;
    j = meio + 1;
    k = 0;
    while (i <= meio && j <= fim)
    {
        if (vetor[i].indice <= vetor[j].indice)
        {
            aux[k] = vetor[i];
            i++;
        }
        else
        {
            aux[k] = vetor[j];
            j++;
        }
        k++;
    }
    while (i <= meio)
    {
        aux[k] = vetor[i];
        i++;
        k++;
    }
    while (j <= fim)
    {
        aux[k] = vetor[j];
        j++;
        k++;
    }
    for (i = 0; i < tam; i++)
    {
        vetor[inicio + i] = aux[i];
    }
    delete[] aux;
}

void mergeSort(pacote *vetor, int inicio, int fim)
{

    if (inicio < fim)
    {
        int meio = (inicio + fim) / 2;
        mergeSort(vetor, inicio, meio);
        mergeSort(vetor, meio + 1, fim);
        merge(vetor, inicio, meio, fim);
    }
}

void salvaVetor(pacote *vetor, ofstream &file, int tamanho){
    pacote umPacote;
    for(int i = 0; i < tamanho; i++){
        umPacote = vetor[i];
        file.write((const char *) &umPacote, sizeof(pacote));
    }
}

int lerArquivo(string fileName){
    ifstream arquivo(fileName, ios::binary);
    pacote umPacote;
    int count = 0;
    while(arquivo.read((char *) &umPacote, sizeof(umPacote))){
        count++;
    }

    arquivo.close();
    return count;
};

void criarArquivoSeparado(ifstream &arquivo){
    pacote umPacote;
    ofstream MyFile("f1.bin", ios::binary);
    ofstream MyFile2("f2.bin", ios::binary);
    int count = 0;
    int contadorDeBlocos = 1;
    int tamanho = 1000;
    pacote vetor[tamanho];
    int i = 0;


    while (arquivo.read((char *) &umPacote, sizeof(pacote))){
        if (contadorDeBlocos == 1000)
        {
            contadorDeBlocos = 0;
            mergeSort(vetor, 0, tamanho);
            if (count % 2 == 0){
                salvaVetor(vetor, MyFile, tamanho);
            } else {
                salvaVetor(vetor, MyFile2, tamanho);
            }
            count++;
            i = 0;
        } else {
            vetor[i] = umPacote;
            i++;
            contadorDeBlocos++;
        }
    }

    cout << "arquivo f1: " << lerArquivo("f1.bin") << endl;
    cout << "arquivo f2: " << lerArquivo("f2.bin") << endl;

    MyFile.close();
    MyFile2.close();
}

void intercalaArquivos(string entrada1Name, string entrada2Name, string saida1Name, string saida2Name, int tamanhoInicialBloco){
    int tamanhoAtualBloco = tamanhoInicialBloco * 2;
    ifstream entrada1(entrada1Name, ios::binary);
    ifstream entrada2(entrada2Name, ios::binary);
    ofstream saida1(saida1Name, ios::binary);
    ofstream saida2(saida2Name, ios::binary);

    pacote aux1;
    pacote aux2;

    pacote vetor1[tamanhoInicialBloco];
    pacote vetor2[tamanhoInicialBloco];
    pacote vetorFinal[tamanhoAtualBloco];

    int contador = 0;
    int contadorArquivo = 0;
    int i = 0;

    while(entrada1.read((char*) &aux1, sizeof(pacote)) and entrada2.read((char*) &aux2, sizeof(pacote))){

        if(contador <= tamanhoInicialBloco){
            vetor1[i] = aux1;
            vetor2[i] = aux2;
            i++;
            contador++;
        } else {
            int contadorGeral = 0;
            int contador1 = 0;
            int contador2 = 0;
            bool vetor1Finalizado = false;
            bool vetor2Finalizado = false;

            while((contadorGeral <= tamanhoAtualBloco) or (vetor1Finalizado) or (vetor2Finalizado)){
                if(vetor1[contador1].indice < vetor2[contador2].indice){
                    vetorFinal[contadorGeral] = vetor1[contador1];
                    contador1++;
                } else {
                    vetorFinal[contadorGeral] = vetor2[contador2];
                    contador2++;
                }

                if(contador1 >= tamanhoInicialBloco){
                    vetor1Finalizado = true;
                } else if (contador2 >= tamanhoInicialBloco){
                    vetor2Finalizado = true;
                }

                contadorGeral++;
            }

            if (vetor1Finalizado) {
                while(contador2 <= tamanhoInicialBloco){
                    vetorFinal[contadorGeral] = vetor2[contador2];
                    contador2++;
                    contadorGeral++;
                }
            } else if (vetor2Finalizado) {
                while(contador1 <= tamanhoInicialBloco){
                    vetorFinal[contadorGeral] = vetor1[contador1];
                    contador2++;
                    contadorGeral++;
                }
            }

            if(contadorArquivo % 2 == 0) {
                salvaVetor(vetorFinal, saida1, tamanhoAtualBloco);
            } else {
                salvaVetor(vetorFinal, saida2, tamanhoAtualBloco);
            }

            contadorArquivo++;
            contador = 0;
            i = 0;
        }
    }
}

int main(){
    ifstream arquivo_bin_read("captura_pacotes.bin", ios::binary);

    criarArquivoSeparado(arquivo_bin_read);

    intercalaArquivos("f1.bin", "f2.bin", "s1.bin", "s2.bin", 1000);

    return 0;
}