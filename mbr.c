#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task1.h"
#include "task2.h"
#include "task3.h"
int main(int argc, char **argv)
{
	if (argv[1][0] - 48 == 1) TASK1 ();
	if (argv[1][0] - 48 ==2) TASK2 ();
	if (argv[1][0] - 48 == 3) TASK3 ();

	return 0;
}
