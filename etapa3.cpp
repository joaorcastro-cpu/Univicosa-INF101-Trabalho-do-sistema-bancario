#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
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
bool separarCampos(const string& linha, string campos[], int quantidade) {
    int campo = 0;
    for (int i = 0; linha[i] != '\0'; i++) {
        if (linha[i] == ';') {
            if (campo >= quantidade - 1) return false;
            campo++;
        } else {
            campos[campo] += linha[i];
        }
    }
    return campo == quantidade - 1;
}
bool converterInteiro(const string& texto, int& valor) {
    int i = 0;
    int sinal = 1;
    if (texto[i] == '-') {
        sinal = -1;
        i++;
    } else if (texto[i] == '+') {
        i++;
    }
    if (texto[i] == '\0') return false;
    int resultado = 0;
    for (; texto[i] != '\0'; i++) {
        if (texto[i] < '0' || texto[i] > '9') return false;
        int digito = texto[i] - '0';
        if (resultado > 214748364 || (resultado == 214748364 && digito > 7)) return false;
        resultado = resultado * 10 + digito;
    }
    valor = resultado * sinal;
    return true;
}
bool converterReal(const string& texto, double& valor) {
    int i = 0;
    double sinal = 1;
    if (texto[i] == '-') {
        sinal = -1;
        i++;
    } else if (texto[i] == '+') {
        i++;
    }
    if (texto[i] == '\0') return false;
    bool pontoDecimal = false;
    bool encontrouDigito = false;
    double parteInteira = 0;
    double parteDecimal = 0;
    double divisor = 0.1;
    for (; texto[i] != '\0'; i++) {
        if (texto[i] == '.' && !pontoDecimal) {
            pontoDecimal = true;
        } else {
            if (texto[i] < '0' || texto[i] > '9') return false;
            encontrouDigito = true;
            int digito = texto[i] - '0';
            if (pontoDecimal) {
                parteDecimal += digito * divisor;
                divisor *= 0.1;
            } else {
                parteInteira = parteInteira * 10 + digito;
            }
        }
    }
    if (!encontrouDigito) return false;
    valor = sinal * (parteInteira + parteDecimal);
    return true;
}
bool temPontoEVirgula(const string& texto) {
    for (int i = 0; texto[i] != '\0'; i++) {
        if (texto[i] == ';') return true;
    }
    return false;
}
int localizarConta(int numero) {
    for (int i = 0; i < quantidadeContas; i++) {
        if (numeroConta[i] == numero) return i;
    }
    return -1;
}
bool lerContaExistente(int& numero, int& indice) {
    if (!lerInteiro("Numero da conta: ", numero)) return false;
    indice = localizarConta(numero);
    if (indice == -1) {
        cout << "Conta nao encontrada.\n";
        return false;
    }
    return true;
}
bool salvarContas() {
    ofstream arquivo("contas.txt");
    if (!arquivo) {
        cout << "Erro: nao foi possivel abrir contas.txt para gravacao.\n";
        return false;
    }
    arquivo << fixed << setprecision(2);
    for (int i = 0; i < quantidadeContas; i++) {
        arquivo << numeroConta[i] << ";" << nomeCliente[i] << ";" << cpf[i] << ";"
                << tipoConta[i] << ";" << saldo[i] << ";" << (contaAtiva[i] ? 1 : 0) << "\n";
    }
    arquivo << flush;
    if (!arquivo) {
        cout << "Erro: nao foi possivel salvar contas.txt.\n";
        return false;
    }
    return true;
}
bool salvarMovimentacoes() {
    ofstream arquivo("movimentacoes.txt");
    if (!arquivo) {
        cout << "Erro: nao foi possivel abrir movimentacoes.txt para gravacao.\n";
        return false;
    }
    arquivo << fixed << setprecision(2);
    for (int i = 0; i < quantidadeMovimentacoes; i++) {
        arquivo << contaMovimentacao[i] << ";" << tipoMovimentacao[i] << ";"
                << valorMovimentacao[i] << "\n";
    }
    arquivo << flush;
    if (!arquivo) {
        cout << "Erro: nao foi possivel salvar movimentacoes.txt.\n";
        return false;
    }
    return true;
}
bool salvarEstado() {
    bool contasSalvas = salvarContas();
    bool movimentacoesSalvas = salvarMovimentacoes();
    return contasSalvas && movimentacoesSalvas;
}
void carregarContas() {
    ifstream arquivo("contas.txt");
    if (!arquivo) return;
    string linha;
    int numeroLinha = 0;
    while (getline(arquivo, linha)) {
        numeroLinha++;
        string campos[6];
        int numero;
        int tipo;
        int ativa;
        double valorSaldo;
        bool registroValido =
            separarCampos(linha, campos, 6) &&
            converterInteiro(campos[0], numero) &&
            converterInteiro(campos[3], tipo) &&
            converterReal(campos[4], valorSaldo) &&
            converterInteiro(campos[5], ativa) &&
            numero > 0 && tipo >= 1 && tipo <= 2 &&
            valorSaldo >= 0 && (ativa == 0 || ativa == 1) &&
            campos[1] != "" && campos[2] != "" &&
            localizarConta(numero) == -1;
        if (!registroValido) {
            cout << "Aviso: registro invalido na linha " << numeroLinha << " de contas.txt.\n";
            continue;
        }
        if (quantidadeContas == MAX_CONTAS) {
            cout << "Aviso: limite de contas atingido; os demais registros foram ignorados.\n";
            break;
        }
        numeroConta[quantidadeContas] = numero;
        nomeCliente[quantidadeContas] = campos[1];
        cpf[quantidadeContas] = campos[2];
        tipoConta[quantidadeContas] = tipo;
        saldo[quantidadeContas] = valorSaldo;
        contaAtiva[quantidadeContas] = ativa == 1;
        quantidadeContas++;
    }
}
void carregarMovimentacoes() {
    ifstream arquivo("movimentacoes.txt");
    if (!arquivo) return;
    string linha;
    int numeroLinha = 0;
    while (getline(arquivo, linha)) {
        numeroLinha++;
        string campos[3];
        int numero;
        double valor;
        bool registroValido =
            separarCampos(linha, campos, 3) &&
            converterInteiro(campos[0], numero) &&
            converterReal(campos[2], valor) &&
            numero > 0 && localizarConta(numero) != -1 &&
            (campos[1] == "D" || campos[1] == "S") &&
            valor > 0;
        if (!registroValido) {
            cout << "Aviso: registro invalido na linha " << numeroLinha
                 << " de movimentacoes.txt.\n";
            continue;
        }
        if (quantidadeMovimentacoes == MAX_MOVIMENTACOES) {
            cout << "Aviso: limite de movimentacoes atingido; os demais registros foram ignorados.\n";
            break;
        }
        contaMovimentacao[quantidadeMovimentacoes] = numero;
        tipoMovimentacao[quantidadeMovimentacoes] = campos[1][0];
        valorMovimentacao[quantidadeMovimentacoes] = valor;
        quantidadeMovimentacoes++;
    }
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
    if (temPontoEVirgula(nome) || temPontoEVirgula(documento)) {
        cout << "Nome e CPF nao podem conter ponto e virgula.\n";
        return;
    }
    int tipo;
    do {
        if (!lerInteiro("Tipo da conta (1 - Corrente, 2 - Poupanca): ", tipo)) return;
        if (tipo != 1 && tipo != 2) cout << "Tipo de conta invalido.\n";
    } while (tipo != 1 && tipo != 2);
    double saldoInicial;
    do {
        if (!lerReal("Saldo inicial: ", saldoInicial)) return;
        if (saldoInicial < 0) cout << "O saldo nao pode ser negativo.\n";
    } while (saldoInicial < 0);
    numeroConta[quantidadeContas] = numero;
    nomeCliente[quantidadeContas] = nome;
    cpf[quantidadeContas] = documento;
    tipoConta[quantidadeContas] = tipo;
    saldo[quantidadeContas] = saldoInicial;
    contaAtiva[quantidadeContas] = true;
    quantidadeContas++;
    if (!salvarContas()) {
        quantidadeContas--;
        salvarContas();
        cout << "Conta nao cadastrada porque nao foi possivel salvar os dados.\n";
        return;
    }
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
bool depositar(int numero, int indice, double valor, bool salvar = true) {
    if (!contaAtiva[indice]) {
        cout << "A conta esta inativa.\n";
        return false;
    }
    if (!(valor > 0)) {
        cout << "O valor do deposito deve ser positivo.\n";
        return false;
    }
    if (quantidadeMovimentacoes == MAX_MOVIMENTACOES) {
        cout << "Limite de movimentacoes atingido.\n";
        return false;
    }
    double saldoAnterior = saldo[indice];
    int movimentacoesAnteriores = quantidadeMovimentacoes;
    saldo[indice] += valor;
    registrarMovimentacao(numero, 'D', valor);
    if (salvar && !salvarEstado()) {
        saldo[indice] = saldoAnterior;
        quantidadeMovimentacoes = movimentacoesAnteriores;
        salvarEstado();
        cout << "Deposito desfeito porque nao foi possivel salvar os dados.\n";
        return false;
    }
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
bool sacar(int numero, int indice, double valor, bool salvar = true) {
    if (!contaAtiva[indice]) {
        cout << "A conta esta inativa.\n";
        return false;
    }
    if (!(valor > 0)) {
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
    double saldoAnterior = saldo[indice];
    int movimentacoesAnteriores = quantidadeMovimentacoes;
    saldo[indice] -= valor;
    registrarMovimentacao(numero, 'S', valor);
    if (salvar && !salvarEstado()) {
        saldo[indice] = saldoAnterior;
        quantidadeMovimentacoes = movimentacoesAnteriores;
        salvarEstado();
        cout << "Saque desfeito porque nao foi possivel salvar os dados.\n";
        return false;
    }
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
    if (!(valor > 0)) {
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
    double saldoDestinoAnterior = saldo[indiceDestino];
    int movimentacoesAnteriores = quantidadeMovimentacoes;
    if (!sacar(origem, indiceOrigem, valor, false)) return;
    if (!depositar(destino, indiceDestino, valor, false)) {
        saldo[indiceOrigem] = saldoOrigemAnterior;
        saldo[indiceDestino] = saldoDestinoAnterior;
        quantidadeMovimentacoes = movimentacoesAnteriores;
        return;
    }
    if (!salvarEstado()) {
        saldo[indiceOrigem] = saldoOrigemAnterior;
        saldo[indiceDestino] = saldoDestinoAnterior;
        quantidadeMovimentacoes = movimentacoesAnteriores;
        salvarEstado();
        cout << "Transferencia desfeita porque nao foi possivel salvar os dados.\n";
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
    if (!encontrou) cout << "Nenhuma movimentacao registrada.\n";
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
        if (novoTipo != 1 && novoTipo != 2) cout << "Tipo de conta invalido.\n";
    } while (novoTipo != 1 && novoTipo != 2);
    int tipoAnterior = tipoConta[indice];
    tipoConta[indice] = novoTipo;
    if (!salvarContas()) {
        tipoConta[indice] = tipoAnterior;
        salvarContas();
        cout << "Alteracao desfeita porque nao foi possivel salvar os dados.\n";
        return;
    }
    cout << "Tipo alterado com sucesso.\n";
}
void alternarConta() {
    int numero;
    int indice;
    if (!lerContaExistente(numero, indice)) return;
    bool situacaoAnterior = contaAtiva[indice];
    contaAtiva[indice] = !contaAtiva[indice];
    if (!salvarContas()) {
        contaAtiva[indice] = situacaoAnterior;
        salvarContas();
        cout << "Alteracao desfeita porque nao foi possivel salvar os dados.\n";
        return;
    }
    cout << "Conta " << (contaAtiva[indice] ? "ativada" : "desativada") << ".\n";
}
int main() {
    carregarContas();
    carregarMovimentacoes();
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
            cout << "Programa encerrado.\n";
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
