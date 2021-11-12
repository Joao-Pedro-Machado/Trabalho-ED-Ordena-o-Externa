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

// void le_arquivo_bin(ifstream &arquivo)
// {
//     pacote umPacote;
//     int i = 0;
//     while (arquivo.read((char *)&umPacote, sizeof(umPacote)))
//     {
//         i++;
//         cout << umPacote.indice << "; ";
//         cout << umPacote.tempo << "; ";
//         cout << umPacote.origem << "; ";
//         cout << umPacote.destino << "; ";
//         cout << umPacote.protocolo << "; ";
//         cout << umPacote.tamanho << "; ";
//         cout << umPacote.informacao << endl;
//     }
//     cout << "Total de pacotes lidos: " << i << endl;
// }

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

void lerPrimeiroArquivo(ifstream &arquivo)
{
    pacote umPacote;
    pacote vetor[1000];
    int i = 0;
    while (arquivo.read((char *)&umPacote, sizeof(umPacote)))
    {
        i++;

        vetor[i] = umPacote;
        // if (i = 10)
        // {
        //     // for (int j = 0; j < 10; j++)
        //     // {
        //     //     cout << batatinha[j].indice << "; ";
        //     //     cout << batatinha[j].tempo << "; ";
        //     //     cout << batatinha[j].origem << "; ";
        //     //     cout << batatinha[j].destino << "; ";
        //     //     cout << batatinha[j].protocolo << "; ";
        //     //     cout << batatinha[j].tamanho << "; ";
        //     //     cout << batatinha[j].informacao << endl;
        //     // }
        // }
        if (i == 10)
        {
            mergeSort(vetor, 0, i);

            for (int r = 0; r < 1000; r++)
            {
                cout << "strinhg " << vetor[r].indice << endl;
            }
        }
    };
}

void salvaVetor(pacote *vetor, ofstream &file, int tamanho){
    for(int i = 0; i < tamanho; i++) {
        file.write((const char *) &vetor[i], sizeof(pacote));
    }
}

void criarArquivoSeparado(ifstream &arquivo)
{
    pacote umPacote;
    ofstream MyFile("f1.bin", ios::binary);
    ofstream MyFile2("f2.bin", ios::binary);
    int count = 0;
    int contadorDeBlocos = 1;
    int tamanho = 1000;
    pacote vetor[tamanho];
    int i = 0;

    while (arquivo.read((char *)&umPacote, sizeof(umPacote)))
    {
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
        } else {
            vetor[i] = umPacote;
            i++;
            contadorDeBlocos++;
        }
    }
        /*if (count % 2 == 0)
        {
            MyFile.write((const char *) &vetor, sizeof(pacote) * tamanho);

            MyFile << umPacote.indice << umPacote.tempo << umPacote.origem << umPacote.destino << umPacote.protocolo << umPacote.tamanho << umPacote.informacao << endl;
        }
        else
        {
            MyFile2 << umPacote.indice << umPacote.tempo << umPacote.origem << umPacote.destino << umPacote.protocolo << umPacote.tamanho << umPacote.informacao << endl;
        }*/

    MyFile.close();
    MyFile2.close();
}

int main()
{
    ifstream arquivo_bin_read("captura_pacotes.bin", ios::binary);
    ifstream arquivo_Txt("filename.txt", ios::binary);
    ifstream arquivo_Txt1("teste1.txt", ios::binary);

    // le_arquivo_bin(arquivo_Txt);
    criarArquivoSeparado(arquivo_bin_read);
    //lerPrimeiroArquivo(arquivo_Txt1);
    return 0;
}