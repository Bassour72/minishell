#include "../../include/execution.h"

int	cd_change_current_directory(t_tree *root)
{
	if (chdir(root->data[1]) == 0 )
		printf("[%s]\n", root->data[1]);
	return 1;
}