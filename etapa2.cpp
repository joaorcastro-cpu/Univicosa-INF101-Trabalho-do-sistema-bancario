#include <iostream>
#include <string>
#include <iomanip>
using namespace std;
const int MAX_CONTAS = 100;
const int MAX_MOVIMENTACOES = 500;
int numeroConta[MAX_CONTAS];
string nomeCliente[MAX_CONTAS];
string cpf[MAX_CONTAS];
int tipoConta[MAX_CONTAS];
double saldo[MAX_CONTAS];
bool contaAtiva[MAX_CONTAS];
int quantidadeContas = 0;
int contaMovimentacao[MAX_MOVIMENTACOES];
char tipoMovimentacao[MAX_MOVIMENTACOES];
double valorMovimentacao[MAX_MOVIMENTACOES];
int quantidadeMovimentacoes = 0;
bool lerInteiro(const string& mensagem, int& valor) {
    cout << mensagem;
    if (cin >> valor) return true;
    cout << "Entrada invalida. Digite um numero inteiro.\n";
    return false;
}
bool lerReal(const string& mensagem, double& valor) {
    cout << mensagem;
    if (cin >> valor) return true;
    cout << "Entrada invalida. Digite um numero.\n";
    return false;
}
bool lerTexto(const string& mensagem, string& valor) {
    cout << mensagem;
    cin >> ws;
    if (getline(cin, valor)) return true;
    cout << "Entrada invalida.\n";
    return false;
}
int localizarConta(int numero) {
    for (int i = 0; i < quantidadeContas; i++) {
        if (numeroConta[i] == numero) {
            return i;
        }
    }
    return -1;
}
bool lerContaExistente(int& numero, int& indice) {
    if (!lerInteiro("Numero da conta: ", numero)) {
        return false;
    }
    indice = localizarConta(numero);
    if (indice == -1) {
        cout << "Conta nao encontrada.\n";
        return false;
    }
    return true;
}
void registrarMovimentacao(int numero, char tipo, double valor) {
    contaMovimentacao[quantidadeMovimentacoes] = numero;
    tipoMovimentacao[quantidadeMovimentacoes] = tipo;
    valorMovimentacao[quantidadeMovimentacoes] = valor;
    quantidadeMovimentacoes++;
}
void cadastrarConta() {
    if (quantidadeContas == MAX_CONTAS) {
        cout << "Limite de contas cadastradas atingido.\n";
        return;
    }
    int numero;
    if (!lerInteiro("Numero da conta: ", numero)) return;
    if (numero <= 0 || localizarConta(numero) != -1) {
        cout << "Numero invalido ou ja cadastrado.\n";
        return;
    }
    string nome;
    string documento;
    if (!lerTexto("Nome do cliente: ", nome)) return;
    if (!lerTexto("CPF do cliente: ", documento)) return;
    int tipo;
    do {
        if (!lerInteiro("Tipo da conta (1 - Corrente, 2 - Poupanca): ", tipo)) return;
        if (tipo != 1 && tipo != 2) {
            cout << "Tipo de conta invalido.\n";
        }
    } while (tipo != 1 && tipo != 2);
    double saldoInicial;
    do {
        if (!lerReal("Saldo inicial: ", saldoInicial)) return;
        if (saldoInicial < 0) {
            cout << "O saldo nao pode ser negativo.\n";
        }
    } while (saldoInicial < 0);
    numeroConta[quantidadeContas] = numero;
    nomeCliente[quantidadeContas] = nome;
    cpf[quantidadeContas] = documento;
    tipoConta[quantidadeContas] = tipo;
    saldo[quantidadeContas] = saldoInicial;
    contaAtiva[quantidadeContas] = true;
    quantidadeContas++;
    cout << "Conta cadastrada com sucesso.\n";
}
void listarContas() {
    if (quantidadeContas == 0) {
        cout << "Nenhuma conta cadastrada.\n";
        return;
    }
    for (int i = 0; i < quantidadeContas; i++) {
        cout << "Numero: " << numeroConta[i]
             << " | Cliente: " << nomeCliente[i]
             << " | CPF: " << cpf[i]
             << " | Tipo: " << (tipoConta[i] == 1 ? "Corrente" : "Poupanca")
             << " | Saldo: R$ " << saldo[i]
             << " | Situacao: " << (contaAtiva[i] ? "Ativa" : "Inativa") << "\n";
    }
}
void consultarConta() {
    int numero;
    int indice;
    if (!lerContaExistente(numero, indice)) return;
    cout << "Numero: " << numeroConta[indice] << "\n"
         << "Cliente: " << nomeCliente[indice] << "\n"
         << "CPF: " << cpf[indice] << "\n"
         << "Tipo: " << (tipoConta[indice] == 1 ? "Corrente" : "Poupanca") << "\n"
         << "Saldo: R$ " << saldo[indice] << "\n"
         << "Situacao: " << (contaAtiva[indice] ? "Ativa" : "Inativa") << "\n";
}
bool depositar(int numero, int indice, double valor) {
    if (!contaAtiva[indice]) {
        cout << "A conta esta inativa.\n";
        return false;
    }
    if (valor <= 0) {
        cout << "O valor do deposito deve ser positivo.\n";
        return false;
    }
    if (quantidadeMovimentacoes == MAX_MOVIMENTACOES) {
        cout << "Limite de movimentacoes atingido.\n";
        return false;
    }
    saldo[indice] += valor;
    registrarMovimentacao(numero, 'D', valor);
    return true;
}
void depositarMenu() {
    int numero;
    int indice;
    if (!lerContaExistente(numero, indice)) return;
    if (!contaAtiva[indice]) {
        cout << "A conta esta inativa.\n";
        return;
    }
    double valor;
    if (!lerReal("Valor do deposito: ", valor)) return;
    if (!depositar(numero, indice, valor)) return;
    cout << "Deposito realizado com sucesso.\n";
}
bool sacar(int numero, int indice, double valor) {
    if (!contaAtiva[indice]) {
        cout << "A conta esta inativa.\n";
        return false;
    }
    if (valor <= 0) {
        cout << "O valor do saque deve ser positivo.\n";
        return false;
    }
    if (valor > saldo[indice]) {
        cout << "Saldo insuficiente.\n";
        return false;
    }
    if (quantidadeMovimentacoes == MAX_MOVIMENTACOES) {
        cout << "Limite de movimentacoes atingido.\n";
        return false;
    }
    saldo[indice] -= valor;
    registrarMovimentacao(numero, 'S', valor);
    return true;
}
void sacarMenu() {
    int numero;
    int indice;
    if (!lerContaExistente(numero, indice)) return;
    if (!contaAtiva[indice]) {
        cout << "A conta esta inativa.\n";
        return;
    }
    double valor;
    if (!lerReal("Valor do saque: ", valor)) return;
    if (!sacar(numero, indice, valor)) return;
    cout << "Saque realizado com sucesso.\n";
}
void transferir() {
    int origem;
    int indiceOrigem;
    if (!lerContaExistente(origem, indiceOrigem)) return;
    int destino;
    int indiceDestino;
    if (!lerContaExistente(destino, indiceDestino)) return;
    if (indiceOrigem == indiceDestino) {
        cout << "As contas de origem e destino devem ser diferentes.\n";
        return;
    }
    double valor;
    if (!lerReal("Valor da transferencia: ", valor)) return;
    if (valor <= 0) {
        cout << "O valor da transferencia deve ser positivo.\n";
        return;
    }
    if (valor > saldo[indiceOrigem]) {
        cout << "Saldo insuficiente.\n";
        return;
    }
    if (MAX_MOVIMENTACOES - quantidadeMovimentacoes < 2) {
        cout << "Nao ha espaco para registrar a transferencia.\n";
        return;
    }
    double saldoOrigemAnterior = saldo[indiceOrigem];
    int movimentacoesAnteriores = quantidadeMovimentacoes;
    if (!sacar(origem, indiceOrigem, valor)) return;
    if (!depositar(destino, indiceDestino, valor)) {
        saldo[indiceOrigem] = saldoOrigemAnterior;
        quantidadeMovimentacoes = movimentacoesAnteriores;
        return;
    }
    cout << "Transferencia realizada com sucesso.\n";
}
void exibirExtrato() {
    int numero;
    int indice;
    if (!lerContaExistente(numero, indice)) return;
    bool encontrou = false;
    cout << "Extrato da conta " << numero << ":\n";
    for (int i = 0; i < quantidadeMovimentacoes; i++) {
        if (contaMovimentacao[i] == numero) {
            cout << (tipoMovimentacao[i] == 'D' ? "Deposito: R$ " : "Saque: R$ ")
                 << valorMovimentacao[i] << "\n";
            encontrou = true;
        }
    }
    if (!encontrou) {
        cout << "Nenhuma movimentacao registrada.\n";
    }
}
void verificarSaldo() {
    int numero;
    int indice;
    if (!lerContaExistente(numero, indice)) return;
    if (!contaAtiva[indice]) {
        cout << "A conta esta inativa.\n";
        return;
    }
    cout << "Saldo atual: R$ " << saldo[indice] << "\n";
}
void alterarTipo() {
    int numero;
    int indice;
    if (!lerContaExistente(numero, indice)) return;
    if (!contaAtiva[indice]) {
        cout << "A conta esta inativa.\n";
        return;
    }
    int novoTipo;
    do {
        if (!lerInteiro("Novo tipo (1 - Corrente, 2 - Poupanca): ", novoTipo)) return;
        if (novoTipo != 1 && novoTipo != 2) {
            cout << "Tipo de conta invalido.\n";
        }
    } while (novoTipo != 1 && novoTipo != 2);
    tipoConta[indice] = novoTipo;
    cout << "Tipo alterado com sucesso.\n";
}
void alternarConta() {
    int numero;
    int indice;
    if (!lerContaExistente(numero, indice)) return;
    contaAtiva[indice] = !contaAtiva[indice];
    cout << "Conta " << (contaAtiva[indice] ? "ativada" : "desativada") << ".\n";
}
int main() {
    cout << fixed << setprecision(2);
    int opcao;
    do {
        cout << "********************************\n"
             << "**        BANCO DO JOÃO       **\n"
             << "********************************\n"
             << "1 - Cadastrar conta\n"
             << "2 - Listar contas\n"
             << "3 - Consultar conta\n"
             << "4 - Depositar\n"
             << "5 - Sacar\n"
             << "6 - Transferir\n"
             << "7 - Exibir extrato\n"
             << "8 - Verificar saldo\n"
             << "9 - Alterar tipo da conta\n"
             << "10 - Ativar/Desativar conta\n"
             << "11 - Sair\n";
        if (!lerInteiro("Escolha uma opcao: ", opcao)) {
            cout << "\nPrograma encerrado.\n";
            break;
        }
        switch (opcao) {
            case 1: cadastrarConta(); break;
            case 2: listarContas(); break;
            case 3: consultarConta(); break;
            case 4: depositarMenu(); break;
            case 5: sacarMenu(); break;
            case 6: transferir(); break;
            case 7: exibirExtrato(); break;
            case 8: verificarSaldo(); break;
            case 9: alterarTipo(); break;
            case 10: alternarConta(); break;
            case 11: cout << "Programa encerrado.\n"; break;
            default: cout << "Opcao invalida.\n";
        }
    } while (opcao != 11);
    return 0;
}
