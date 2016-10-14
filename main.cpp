#include <iostream>
#include "ssdes.h"

using namespace std;

int main() {
	encrypt_text("input", 12);
	decrypt_text("input_encrypted", 12);
	return 0;
}