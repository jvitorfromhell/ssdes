#include <iostream>
#include <string>
#include "ssdes.h"

using namespace std;

/*
	Script para rodar testes no sistema SSDES.
*/

int main() {
	int select, key, rounds;
	string input, output;

	cout << "\tSimulador do Super Simplified DES";

	while (1) {
		cout << "\n\t1. Teste padrão - encriptar" << endl;
		cout << "\t2. Teste padrão - desencriptar" << endl;
		cout << "\t3. Teste do usuário - encriptar" << endl;
		cout << "\t4. Teste do usuário - desencriptar" << endl;
		cout << "\t5. Sair" << endl << "\t";
		cin >> select;

		switch (select) {
			case 1: encrypt_text("input.txt", "input_encrypted.txt", 45, 16);
					break;

			case 2: decrypt_text("input_encrypted.txt", "input_decrypted.txt", 45, 16);
					break;

			case 3: cout << "\n\tNome do arquivo de entrada: ";
					cin >> input;
					cout << "\tNome do arquivo de saida: ";
					cin >> output;
					cout << "\tChave: ";
					cin >> key;
					cout << "\tRounds: ";
					cin >> rounds;
					encrypt_text(input, output, key, rounds);
					break;

			case 4: cout << "\n\tNome do arquivo de entrada: ";
					cin >> input;
					cout << "\tNome do arquivo de saida: ";
					cin >> output;
					cout << "\tChave: ";
					cin >> key;
					cout << "\tRounds: ";
					cin >> rounds;
					decrypt_text(input, output, key, rounds);
					break;

			case 5: exit(0);

			default: cout << "\n\tOpção inválida, tente novamente\n" << endl;
					 break;
		}
	}
	return 0;
}