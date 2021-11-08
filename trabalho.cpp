#include <iostream>
#include <cstring>
#include <fstream>


using namespace std;


// dados armazenados no arquivo
struct pacote {
    unsigned indice;
    float tempo;
    char origem[40];
    char destino[40];
    char protocolo[18];
    unsigned tamanho;
    char informacao[100]; 
};


void le_arquivo_bin(ifstream& arquivo) {
    pacote umPacote;
    while(arquivo.read((char*) &umPacote, sizeof(umPacote))) {
        cout << umPacote.indice << "; ";
        cout << umPacote.tempo << "; ";
        cout << umPacote.origem << "; ";
        cout << umPacote.destino << "; ";
        cout << umPacote.protocolo << "; ";
        cout << umPacote.tamanho << "; ";
        cout << umPacote.informacao << endl;
    }
}


int main(){
    ifstream arquivo_bin_read("captura_pacotes.bin",ios::binary);

    le_arquivo_bin(arquivo_bin_read);

    return 0;
}
