#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum EstadoVoo { 
    PLANEJADO = 0, 
    EM_CURSO = 1, 
    SUCESSO = 2, 
    EXPLOSAO = 3 
};

class Astronauta {
public:
    string cpf;
    string nome;
    int idade;
    bool vivo = true;
    bool disponivel = true;
};

class Voo {
public:
    int codigo;
    vector<string> cpfsPassageiros;
    EstadoVoo estado = PLANEJADO;
};

vector<Astronauta> astronautas;
vector<Voo> voos;

int buscarAstronauta(string cpf) {
    for (int i = 0; i < astronautas.size(); i++) {
        if (astronautas[i].cpf == cpf) {
            return i;
        }
    }
    return -1;
}

int buscarVoo(int codigo) {
    for (int i = 0; i < voos.size(); i++) {
        if (voos[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

void cadastrarAstronauta(string cpf, int idade, string nome) {
    if (buscarAstronauta(cpf) != -1) {
        cout << "Erro: Astronauta com CPF " << cpf << " ja cadastrado.\n";
        return;
    }
    Astronauta a;
    a.cpf = cpf;
    a.idade = idade;
    a.nome = nome;
    astronautas.push_back(a);
    cout << "Astronauta " << nome << " cadastrado com sucesso.\n";
}

void cadastrarVoo(int codigo) {
    if (buscarVoo(codigo) != -1) {
        cout << "Erro: Voo " << codigo << " ja existe.\n";
        return;
    }
    Voo v;
    v.codigo = codigo;
    voos.push_back(v);
    cout << "Voo " << codigo << " cadastrado com sucesso (Planejado).\n";
}

void adicionarAstronauta(string cpf, int codigo) {
    int idxAstro = buscarAstronauta(cpf);
    if (idxAstro == -1) {
        cout << "Erro: Astronauta nao encontrado.\n";
        return;
    }
    int idxVoo = buscarVoo(codigo);
    if (idxVoo == -1) {
        cout << "Erro: Voo nao encontrado.\n";
        return;
    }
    
    if (voos[idxVoo].estado != PLANEJADO) {
        cout << "Erro: Voo nao esta mais na fase de planejamento.\n";
        return;
    }
    if (!astronautas[idxAstro].vivo) {
        cout << "Erro: Astronauta esta morto.\n";
        return;
    }
    
    for (int i = 0; i < voos[idxVoo].cpfsPassageiros.size(); i++) {
        if (voos[idxVoo].cpfsPassageiros[i] == cpf) {
            cout << "Erro: Astronauta ja esta neste voo.\n";
            return;
        }
    }
    
    voos[idxVoo].cpfsPassageiros.push_back(cpf);
    cout << "Astronauta adicionado ao voo " << codigo << " com sucesso.\n";
}

void removerAstronauta(string cpf, int codigo) {
    int idxAstro = buscarAstronauta(cpf);
    int idxVoo = buscarVoo(codigo);
    
    if (idxAstro == -1 || idxVoo == -1) {
        cout << "Erro: Astronauta ou Voo nao encontrado.\n";
        return;
    }
    if (voos[idxVoo].estado != PLANEJADO) {
        cout << "Erro: O voo nao esta em planejamento.\n";
        return;
    }

    bool removido = false;
    for (int i = 0; i < voos[idxVoo].cpfsPassageiros.size(); i++) {
        if (voos[idxVoo].cpfsPassageiros[i] == cpf) {
            voos[idxVoo].cpfsPassageiros.erase(voos[idxVoo].cpfsPassageiros.begin() + i);
            removido = true;
            cout << "Astronauta removido do voo " << codigo << " com sucesso.\n";
            break;
        }
    }
    
    if (!removido) {
        cout << "Erro: Astronauta nao pertence a este voo.\n";
    }
}

void lancarVoo(int codigo) {
    int idxVoo = buscarVoo(codigo);
    if (idxVoo == -1) {
        cout << "Erro: Voo nao encontrado.\n";
        return;
    }
    if (voos[idxVoo].estado != PLANEJADO) {
        cout << "Erro: Voo nao esta em planejamento.\n";
        return;
    }
    if (voos[idxVoo].cpfsPassageiros.empty()) {
        cout << "Erro: Voo nao pode ser lancado sem astronautas.\n";
        return;
    }
    
    for (int i = 0; i < voos[idxVoo].cpfsPassageiros.size(); i++) {
        int idxAstro = buscarAstronauta(voos[idxVoo].cpfsPassageiros[i]);
        if (!astronautas[idxAstro].vivo || !astronautas[idxAstro].disponivel) {
            cout << "Erro: O astronauta de CPF " << astronautas[idxAstro].cpf 
                 << " nao pode participar (morto ou indisponivel).\n";
            return;
        }
    }
    
    voos[idxVoo].estado = EM_CURSO;
    for (int i = 0; i < voos[idxVoo].cpfsPassageiros.size(); i++) {
        int idxAstro = buscarAstronauta(voos[idxVoo].cpfsPassageiros[i]);
        astronautas[idxAstro].disponivel = false;
    }
    cout << "Voo " << codigo << " lancado com sucesso! Status: Em curso.\n";
}

void explodirVoo(int codigo) {
    int idxVoo = buscarVoo(codigo);
    if (idxVoo == -1) {
        cout << "Erro: Voo nao encontrado.\n";
        return;
    }
    if (voos[idxVoo].estado != EM_CURSO) {
        cout << "Erro: Apenas voos 'Em curso' podem explodir.\n";
        return;
    }
    
    voos[idxVoo].estado = EXPLOSAO;
    for (int i = 0; i < voos[idxVoo].cpfsPassageiros.size(); i++) {
        int idxAstro = buscarAstronauta(voos[idxVoo].cpfsPassageiros[i]);
        astronautas[idxAstro].vivo = false;
        astronautas[idxAstro].disponivel = false;
    }
    cout << "Alerta: Voo " << codigo << " explodiu. Astronautas declarados mortos.\n";
}

void finalizarVoo(int codigo) {
    int idxVoo = buscarVoo(codigo);
    if (idxVoo == -1) {
        cout << "Erro: Voo nao encontrado.\n";
        return;
    }
    if (voos[idxVoo].estado != EM_CURSO) {
        cout << "Erro: Apenas voos 'Em curso' podem ser finalizados com sucesso.\n";
        return;
    }
    
    voos[idxVoo].estado = SUCESSO;
    for (int i = 0; i < voos[idxVoo].cpfsPassageiros.size(); i++) {
        int idxAstro = buscarAstronauta(voos[idxVoo].cpfsPassageiros[i]);
        if(astronautas[idxAstro].vivo) {
            astronautas[idxAstro].disponivel = true;
        }
    }
    cout << "Voo " << codigo << " finalizado com sucesso. Astronautas disponiveis novamente.\n";
}

void listarVoos() {
    string nomesEstados[] = {"Planejados", "Em curso", "Finalizados com sucesso", "Finalizados com explosao"};
    
    cout << "\n--- LISTAGEM DE VOOS ---\n";
    for (int e = 0; e <= 3; e++) {
        cout << "[" << nomesEstados[e] << "]\n";
        bool achou = false;
        for (int i = 0; i < voos.size(); i++) {
            if (voos[i].estado == e) {
                achou = true;
                cout << "  Voo " << voos[i].codigo << " | Passageiros: ";
                if (voos[i].cpfsPassageiros.empty()) cout << "Nenhum";
                
                for (int j = 0; j < voos[i].cpfsPassageiros.size(); j++) {
                    int idxAstro = buscarAstronauta(voos[i].cpfsPassageiros[j]);
                    cout << astronautas[idxAstro].nome << " (" << astronautas[idxAstro].cpf << ")";
                    if (j < voos[i].cpfsPassageiros.size() - 1) cout << ", ";
                }
                cout << "\n";
            }
        }
        if (!achou) cout << "  Nenhum voo neste estado.\n";
        cout << "\n";
    }
}

void listarMortos() {
    cout << "--- ASTRONAUTAS MORTOS ---\n";
    bool achouMorto = false;
    for (int i = 0; i < astronautas.size(); i++) {
        if (!astronautas[i].vivo) {
            achouMorto = true;
            cout << "CPF: " << astronautas[i].cpf << " | Nome: " << astronautas[i].nome << " | Voos: ";
            
            bool primeiro = true;
            for (int j = 0; j < voos.size(); j++) {
                if (voos[j].estado != PLANEJADO) {
                    for (int k = 0; k < voos[j].cpfsPassageiros.size(); k++) {
                        if (voos[j].cpfsPassageiros[k] == astronautas[i].cpf) {
                            if (!primeiro) cout << ", ";
                            cout << voos[j].codigo;
                            primeiro = false;
                            break;
                        }
                    }
                }
            }
            if (primeiro) cout << "Nenhum";
            cout << "\n";
        }
    }
    if (!achouMorto) cout << "Nenhum astronauta morto registrado.\n";
    cout << "\n";
}

int main() {
    string comando;
    while (cin >> comando) {
        if (comando == "FIM") {
            cout << "Encerrando sistema da Agencia Espacial.\n";
            break;
        }
        
        if (comando == "CADASTRAR_ASTRONAUTA") {
            string cpf, nome;
            int idade;
            cin >> cpf >> idade;
            getline(cin >> ws, nome);
            cadastrarAstronauta(cpf, idade, nome);
        }
        else if (comando == "CADASTRAR_VOO") {
            int codigo;
            cin >> codigo;
            cadastrarVoo(codigo);
        }
        else if (comando == "ADICIONAR_ASTRONAUTA") {
            string cpf;
            int codigo;
            cin >> cpf >> codigo;
            adicionarAstronauta(cpf, codigo);
        }
        else if (comando == "REMOVER_ASTRONAUTA") {
            string cpf;
            int codigo;
            cin >> cpf >> codigo;
            removerAstronauta(cpf, codigo);
        }
        else if (comando == "LANCAR_VOO") {
            int codigo;
            cin >> codigo;
            lancarVoo(codigo);
        }
        else if (comando == "EXPLODIR_VOO") {
            int codigo;
            cin >> codigo;
            explodirVoo(codigo);
        }
        else if (comando == "FINALIZAR_VOO") {
            int codigo;
            cin >> codigo;
            finalizarVoo(codigo);
        }
        else if (comando == "LISTAR_VOOS") {
            listarVoos();
        }
        else if (comando == "LISTAR_MORTOS") {
            listarMortos();
        }
        else {
            cout << "Comando nao reconhecido: " << comando << "\n";
            string descarte;
            getline(cin, descarte);
        }
    }

    return 0;
}