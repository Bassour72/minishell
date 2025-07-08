/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echoctl.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:01:30 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/08 14:33:20 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ECHOCTL_H
# define ECHOCTL_H

# include <termios.h>
# include <unistd.h>

void	enable_echoctl(void);
void	disable_echoctl(void);

#endif