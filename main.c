#include "functions.h"

int main(void) {

	unsigned short condition = 1;

	while (condition) {

		condition = mainMenu();
	}

	return 0;
}
