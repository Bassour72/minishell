/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 17:11:34 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/07 14:28:46 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>

typedef struct s_node
{
	char			*data;
	struct s_node	*next;
}				t_node;
typedef struct s_list
{
	char			*str_buffer;
	void			*content;
	struct s_list	*next;
}	t_list;
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42 
# endif

t_list	*ft_find_first_node(t_list *list);
char	*get_next_line(int fd);
char	*ft_read_line(t_list *list);
size_t	ft_is_found_newline(t_list *list);
size_t	ft_length_to_newline(t_list *list);
int		ft_listmap(t_list **list);
int		ft_create_list(t_list **list, int fd);
void	ft_strcpy_fromlist(t_list *list, char *str);
int		ft_listclear(t_list **list, t_list *new_node, char *buffer);
void	ft_listadd_back(t_list **list, char *buf);
char *ft_strcat(char *dest, const char *src);
char *ft_strcpy(char *dest, const char *src);
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t count, size_t size);
void	*ft_memset(void *s, int c, size_t len);
void	*ft_memcpy(void *dest, const void *src, size_t len);
void	*ft_memmove(void *dest, const void *src, size_t len);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
int		ft_isalnum(int c);
int		ft_toupper(int c);
int		ft_tolower(int c);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
char	*ft_strdup(const char *s);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int		ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlen(const char *s);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
size_t	ft_strlcat(char *dst, const char *src, size_t size);
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
long	ft_atoi(const char *str);
char	*ft_itoa(int n);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	**ft_split(char const *s, char c);
char	**ft_split2(char *s);
void	free_failed_list(char **list, int len);
int		ft_strcmp(const char *s1, const char *s2);
void	free_2d_arr(char **arr);
int		ft_isspace(int c);
char	**ft_split3(char *str);
void	free_list(t_node *list);

#endif
