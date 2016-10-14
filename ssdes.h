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