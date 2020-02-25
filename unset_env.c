#include "ush.h"

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char	*new_s;
	size_t	i;

	if (s)
	{
		if (!(new_s = mx_strnew(len)))
			mx_printstr("error malloc");
		i = 0;
		while (i < len)
			*(new_s + i++) = *(s + start++);
		*(new_s + i) = '\0';
		return (new_s);
	}
	else
		return (NULL);
}

static int		ft_tablen(char **tab)
{
	int		len;

	len = 0;
	while (*(tab + len))
		len++;
	return (len);
}


void	ft_supprtotab(char ***tab, int suppr)
{
	char	**new;
	char	**old;
	int		len;
	int		i;
	int		j;

	len = ft_tablen(*tab) - 1;
	if (!(new = (char**)malloc(sizeof(char*) * (len + 1))))
		mx_printstr("Unable to malloc tab");
	*(new + len) = 0;
	i = 0;
	j = 0;
	while (*(*tab + j))
	{
		if (j != suppr)
			*(new + i++) = *(*tab + j++);
		else
			free(*(*tab + j++));
	}
	old = *tab;
	*tab = new;
	free(old);
}

int mx_unsetenv_builtin(char **arg, char ***env) {
    int	i;
	int	var_pos;

	if (!arg[0]) {
		mx_errors(UNSET_ERR, "uns");
		return 1;
	}
	i = -1;
	while (arg[++i]) {
		if ((var_pos = mx_isinenv(*env, arg[i])) == -1) {
			i++;
			continue ;
        }
        ft_supprtotab(env, var_pos);
		i++;
	}
	return 1;
}

int	mx_isinenv(char **env, char *var) {
	char	*temp;
	int		i;
	int		ret;

	i = 0;
	ret = -1;
	while (*(env + i))
	{
		temp = ft_strsub(*(env + i), 0,mx_strchr(*(env + i), '=') - *(env + i));
		if (mx_strequ(temp, var))
			ret = i;
		i++;
		free(temp);
	}
	return (ret);
}