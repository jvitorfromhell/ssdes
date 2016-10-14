#include <string>

using namespace std;

class SBox {
private:
	int** matrix, lines, columns;

public:
	SBox(int lines, int columns, string filename);
	~SBox();

	int get(int key);
};

int E(int Di_minus_one);
int F(int Di_minus_one, int Ki, SBox &s1, SBox &s2);
int round(int block, int Ki, SBox &s1, SBox &s2);