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
        if (vetor[i].indice >= vetor[j].indice)
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

    /*
        Salva um vetor em um arquivo
    */
    pacote umPacote;
    for(int i = 0; i <= tamanho; i++){
        umPacote = vetor[i];
        file.write((const char *) &umPacote, sizeof(pacote));
    }
}

int lerArquivo(string fileName){
    /*
        Realiza a contagem de elementos nos arquivos
    */

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
    /*
        Realiza a leitura do arquivo base e divide em 2 arquivo com os dados parcialmente ordenados
    */

    pacote umPacote;
    ofstream MyFile("f1.bin", ios::binary);
    ofstream MyFile2("f2.bin", ios::binary);
    int count = 0;
    int contadorDeBlocos = 0;
    int tamanho = 1000;
    pacote vetor[tamanho];
    int i = 0;

    while (arquivo.read((char *) &umPacote, sizeof(pacote))){
        if (contadorDeBlocos >= 999)
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

    int contador = 0;
    int contadorArquivo = 0;
    int i = 0;

    /*
        Enquanto for possível ler dados dos arquivo de entrada
        Carrega-se os dados na memória (1000 pacotes por vez)
        E depois realiza a intercalação para o arquivo de saída correspondente
    */

    entrada1.seekg(ios::end);
    int tamanhoEntrada1 = entrada1.tellg();
    entrada1.seekg(ios::beg);

    entrada2.seekg(ios::end);
    int tamanhoEntrada2 = entrada2.tellg();
    entrada2.seekg(ios::beg);

    entrada1.read((char *) &aux1, sizeof(pacote));
    entrada2.read((char *) &aux2, sizeof(pacote));

    int cont1 = 0, cont2 = 0;
    int contadorGeral = 0;

    while(entrada1.tellg() == tamanhoEntrada1 or entrada2.tellg() == tamanhoEntrada2){

        if(cont1 == sizeof(pacote) * tamanhoInicialBloco){
            if(contadorGeral % 2 == 0){
                saida1.write((char *) &aux2, sizeof(pacote));
            } else {
                saida2.write((char *) &aux2, sizeof(pacote));
            }

            entrada2.read((char *) &aux2, sizeof(pacote));
            cont2 += sizeof(pacote);
        } else if (cont2 == sizeof(pacote) * tamanhoInicialBloco) {
            if(contadorGeral % 2 == 0){
                saida1.write((char *) &aux1, sizeof(pacote));
            } else {
                saida2.write((char *) &aux1, sizeof(pacote));
            }

            entrada1.read((char *) &aux1, sizeof(pacote));
            cont1 += sizeof(pacote);
        } else {
            if(aux1.indice > aux2.indice){
                saida1.write((char *) &aux1, sizeof(pacote));
                entrada1.read((char *) &aux1, sizeof(pacote));
                cont1 += sizeof(pacote);
            } else {
                saida1.write((char *) &aux2, sizeof(pacote));
                entrada2.read((char *) &aux2, sizeof(pacote));
                cont2 += sizeof(pacote);
            }
        }

        contadorGeral++;
    }
}

void mergeExterno(){
    string entrada1 = "f1.bin";
    string entrada2 = "f2.bin";
    string saida1 = "s1.bin";
    string saida2 = "s2.bin";

    int tamanhoInicial = 1000;

    bool fim = false;


    // Chama a intercalação
    while(!fim){
        if((lerArquivo(entrada1) == 0) and lerArquivo(entrada2) == 0){
            intercalaArquivos(saida1, saida2, entrada1, entrada2, tamanhoInicial);
        } else {
            intercalaArquivos(entrada1, entrada2, saida1, saida2, tamanhoInicial);
        }

        tamanhoInicial = tamanhoInicial * 2;


        // Verifica se um dos arquivos de entrada estão cheio enquanto o outro está vazio indicando o fim do processo
        if(lerArquivo(entrada1) == 0 and lerArquivo(entrada2) != 0){
            fim = true;
        } else if (lerArquivo(entrada2) == 0 and lerArquivo(entrada1) != 0){
            fim = true;
        } else if (lerArquivo(saida1) == 0 and lerArquivo(saida2) != 0) {
            fim = true;
        } else if (lerArquivo(saida2) == 0 and lerArquivo(saida1) != 0) {
            fim = true;
        }
    }
};

int main(){
    ifstream arquivo_bin_read("captura_pacotes.bin", ios::binary);

    criarArquivoSeparado(arquivo_bin_read);

    mergeExterno();

    return 0;
}