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

void le_arquivo_bin(ifstream &arquivo)
{
    pacote umPacote;
    int i = 0;
    while (arquivo.read((char *)&umPacote, sizeof(umPacote)))
    {
        i++;
        cout << umPacote.indice << "; ";
        cout << umPacote.tempo << "; ";
        cout << umPacote.origem << "; ";
        cout << umPacote.destino << "; ";
        cout << umPacote.protocolo << "; ";
        cout << umPacote.tamanho << "; ";
        cout << umPacote.informacao << endl;
    }
    cout << "Total de pacotes lidos: " << i << endl;
}

void criarArquivoSeparado(ifstream &arquivo)
{
    pacote umPacote;
    ofstream MyFile("teste1.txt");
    ofstream MyFile2("teste2.txt");
    int count = 0;
    int contadorDeBlocos = 0;

    cout << sizeof(umPacote) << endl;
    while (arquivo.read((char *)&umPacote, sizeof(umPacote)))
    {
        contadorDeBlocos++;
        if(contadorDeBlocos == 4) {
            contadorDeBlocos=0;
            count++;
        }
        if(count%2 == 0) {
            MyFile << umPacote.indice << umPacote.tempo << umPacote.origem << umPacote.destino << umPacote.protocolo << umPacote.tamanho << umPacote.informacao << endl;
        } else {
            MyFile2 << umPacote.indice << umPacote.tempo << umPacote.origem << umPacote.destino << umPacote.protocolo << umPacote.tamanho << umPacote.informacao << endl;
        }

    }

    MyFile.close();
    MyFile2.close();
}


void mergeSortInterativo(){

    
}

int main()
{
    ifstream arquivo_bin_read("captura_pacotes.bin", ios::binary);
    ifstream arquivo_Txt("filename.txt", ios::binary);

    le_arquivo_bin(arquivo_Txt);
    criarArquivoSeparado(arquivo_bin_read);
    return 0;
}