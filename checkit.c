#include <stdio.h>

char *crypt();
char pw[] = "magicword";

main()
{

	printf(">>>%s<<<\n",crypt(pw,"mT"));
}
