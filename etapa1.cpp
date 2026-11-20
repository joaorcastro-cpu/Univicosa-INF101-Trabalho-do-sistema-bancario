#include <iostream>
#include <string>
using namespace std;
int main() {
    int numeroConta = 0;
    string nomeCliente;
    string cpf;
    int tipoConta = 0;
    double saldo = 0;
    bool contaAtiva = false;
    bool contaCadastrada = false;
    int opcao;
    do {
        cout << "********************************\n";
        cout << "**        BANCO DO JOÃO       **\n";
        cout << "********************************\n";
        cout << "1 - Cadastrar conta\n";
        cout << "2 - Consultar conta\n";
        cout << "3 - Verificar saldo\n";
        cout << "4 - Alterar tipo da conta\n";
        cout << "5 - Ativar/Desativar conta\n";
        cout << "6 - Sair\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;
        switch (opcao) {
            case 1:
                if (!contaCadastrada) {
                    do {
                    cout << "Numero da conta: ";
                    cin >> numeroConta;
                    if (numeroConta <= 0) {
                        cout << "O numero deve ser maior que zero.\n";
                    }
                } while (numeroConta <= 0);
                cout << "Nome do cliente: ";
                cin >> nomeCliente;
                cout << "CPF do cliente: ";
                cin >> cpf;
                do {
                    cout << "Tipo da conta (1 - Corrente, 2 - Poupanca): ";
                    cin >> tipoConta;
                    if (tipoConta != 1 && tipoConta != 2) {
                        cout << "Tipo de conta invalido.\n";
                    }
                } while (tipoConta != 1 && tipoConta != 2);
                do {
                    cout << "Saldo inicial: ";
                    cin >> saldo;
                    if (saldo < 0) {
                        cout << "O saldo nao pode ser negativo.\n";
                    }
                } while (saldo < 0);
                contaAtiva = true;
                contaCadastrada = true;
                cout << "Conta cadastrada com sucesso.\n";
                } else {
                    cout << "Ja existe uma conta cadastrada.\n";
                }
                break;
            case 2:  
                if (contaCadastrada) {
                    cout << "Numero: " << numeroConta << "\n";
                    cout << "Cliente: " << nomeCliente << "\n";
                    cout << "CPF: " << cpf << "\n";
                    cout << "Tipo: " << (tipoConta == 1 ? "Corrente" : "Poupanca") << "\n";
                    cout << "Saldo: R$ " << saldo << "\n";
                    cout << "Situacao: " << (contaAtiva ? "Ativa" : "Inativa") << "\n";
                } else {
                    cout << "Nenhuma conta cadastrada.\n";
                }
                break;
            case 3:
                if (!contaCadastrada) {
                    cout << "Nenhuma conta cadastrada.\n";
                } else if (!contaAtiva) {
                    cout << "A conta esta inativa.\n";
                } else {
                    cout << "Saldo atual: R$ " << saldo << "\n";
                }
                break;
            case 4:
                
                if (!contaCadastrada) {
                    cout << "Nenhuma conta cadastrada.\n";
                } else if (!contaAtiva) {
                    cout << "A conta esta inativa.\n";
                } else {
                    int novoTipo;
                    do {
                        cout << "Novo tipo (1 - Corrente, 2 - Poupanca): ";
                        cin >> novoTipo;
                        if (novoTipo != 1 && novoTipo != 2) {
                            cout << "Tipo de conta invalido.\n";
                        }
                    } while (novoTipo != 1 && novoTipo != 2);
                    tipoConta = novoTipo;
                    cout << "Tipo alterado com sucesso.\n";
                }
                break;
            case 5:
                if (!contaCadastrada) {
                    cout << "Nenhuma conta cadastrada.\n";
                } else {
                    contaAtiva = !contaAtiva;
                    cout << "Conta " << (contaAtiva ? "ativada" : "desativada") << ".\n";
                }
                break;
            case 6:
                cout << "Programa encerrado.\n";
                break;
            default:
                cout << "Opcao invalida.\n";
        }
    } while (opcao != 6);
    return 0;
}