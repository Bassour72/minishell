#include "../../include/execution.h"

int pwd_print_working_directory(t_tree *root)
{
	char	*pathname ;

	pathname = getcwd(0, 100);
	if (!pathname)
		return 1;
	printf("%s\n",pathname);
	free(pathname);
	return 0;
}