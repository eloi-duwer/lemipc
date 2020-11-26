/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 23:36:38 by eduwer            #+#    #+#             */
/*   Updated: 2020/11/26 18:19:13 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lemipc.h>

void	perror_and_exit(t_ctx *ctx, char *msg, bool free_shared_ressources)
{
	char	*str;

	str = strerror(errno);
	ft_fdprintf(2, "%s: %s\n", msg, str);
	free_ressources(ctx, free_shared_ressources);
	exit(1);
}

void	error_and_exit(t_ctx *ctx, char *msg, bool free_shared_ressources)
{
	ft_fdprintf(2, "%s\n", msg);
	free_ressources(ctx, free_shared_ressources);
	exit(1);
}

void	print_usage(t_ctx *ctx, char *prog, bool free_shared_ressources)
{
	ft_printf("Usage for the first instance: \n\
%s <board_size> <nb_teams> <nb_players_per_team>\n\
All arguments are positive integers between 2 and 256,\n\
board_size is the size of the board,\n\
nb_teams is the number of teams,\n\
nb_players_per_team is the number of players per team.\n\
The game doesn\'t starts until all players have joined\n\n\
For all other instances:\n\
%s <team_nb>\n\
Where team_nb is the number of the team that instance must be in\n", \
		prog, prog);
	free_ressources(ctx, free_shared_ressources);
	exit(1);
}
