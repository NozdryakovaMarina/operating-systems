#include <stdio.h>
#include <string.h>
int i = 0;
int flag = 0;
unsigned char etalon[] = { "xyz" };
int check_pass() {
	unsigned char password[4];
	printf("Enter password > ");
	scanf("%s", password);
	while (etalon[i] != 0) if (password[i] != etalon[i++]) flag = 1;
	return flag;
}
int main() {
	_asm { cpuid };
	if (check_pass() == 0)
		printf("OK");
	else
		printf("Bad password");
	return 0;
}