#include "../../include/execution.h"

static int	has_three_consecutive_dots(const char *arg)
{
	int	i;
	int	dot_count;

	i = 0;
	dot_count = 0;
	while (arg[i])
	{
		if (arg[i] == '.')
		{
			dot_count++;
			if (dot_count == 3)
			{
				fprintf(stderr, "cd: %s: invalid argument (three consecutive dots)\n", arg);
				return (1);
			}
		}
		else
			dot_count = 0;
		i++;
	}
	return (0);
}

static int	has_three_consecutive_hyphens(const char *arg)
{
	int	i;
	int	hyphen_count;

	i = 0;
	hyphen_count = 0;
	while (arg[i])
	{
		if (arg[i] == '-')
		{
			hyphen_count++;
			if (hyphen_count == 3)
			{
				fprintf(stderr, "cd: %s: invalid argument (three consecutive hyphens)\n", arg);
				return (1);
			}
		}
		else
			hyphen_count = 0;
		i++;
	}
	return (0);
}

static int	has_invalid_char_combination(const char *arg)
{
	int	i;

	i = 0;
	while (arg[i] && arg[i + 1])
	{
		if (arg[i] == '.' && arg[i + 1] == '-')
		{
			fprintf(stderr, "cd: %s: invalid argument (\".-\" not allowed)\n", arg);
			return (1);
		}
		if (arg[i] == '-' && (arg[i + 1] == '.' || arg[i + 1] == '/'))
		{
			if (arg[i + 1] == '.')
				fprintf(stderr, "cd: %s: invalid argument (\"-.\" not allowed)\n", arg);
			else
				fprintf(stderr, "cd: %s: invalid argument (\"-/\" not allowed)\n", arg);
			return (1);
		}
		i++;
	}
	return (0);
}

int	is_illegal_cd_arg(const char *arg)
{
	if (!arg)
		return (0);
	if (has_three_consecutive_dots(arg))
		return (1);
	if (has_three_consecutive_hyphens(arg))
		return (1);
	if (has_invalid_char_combination(arg))
		return (1);
	return (0);
}
