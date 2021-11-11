#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

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

    cout << " inicio " << inicio << " fim " << fim << endl;
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
    pacote batatinha[100];
    int i = 0;
    while (arquivo.read((char *)&umPacote, sizeof(umPacote)))
    {
        i++;

        batatinha[i] = umPacote;
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
            mergeSort(batatinha, 0, i);

            for (int r = 0; r < 10; r++)
            {
                cout << "strinhg " << batatinha[r].indice << endl;
            }
        }
    };
}

void criarArquivoSeparado(ifstream &arquivo)
{
    pacote umPacote;
    ofstream MyFile("teste1.txt");
    ofstream MyFile2("teste2.txt");
    int count = 0;
    int contadorDeBlocos = 0;

    while (arquivo.read((char *)&umPacote, sizeof(umPacote)))
    {
        contadorDeBlocos++;
        if (contadorDeBlocos == 4)
        {
            contadorDeBlocos = 0;
            count++;
        }
        if (count % 2 == 0)
        {
            MyFile << umPacote.indice << umPacote.tempo << umPacote.origem << umPacote.destino << umPacote.protocolo << umPacote.tamanho << umPacote.informacao << endl;
        }
        else
        {
            MyFile2 << umPacote.indice << umPacote.tempo << umPacote.origem << umPacote.destino << umPacote.protocolo << umPacote.tamanho << umPacote.informacao << endl;
        }
    }

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
    lerPrimeiroArquivo(arquivo_Txt1);
    return 0;
}