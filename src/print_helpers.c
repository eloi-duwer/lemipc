/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 23:36:38 by eduwer            #+#    #+#             */
/*   Updated: 2020/12/06 01:52:18 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lemipc.h>

void		perror_and_exit(t_ctx *ctx, char *msg, bool free_shared_ressources)
{
	ft_fdprintf(2, "%s: %s\n", msg, strerror(errno));
	free_ressources(ctx, free_shared_ressources);
	exit(1);
}

void		error_and_exit(t_ctx *ctx, char *msg, bool free_shared_ressources)
{
	ft_fdprintf(2, "%s\n", msg);
	free_ressources(ctx, free_shared_ressources);
	exit(1);
}

void		print_usage(t_ctx *ctx, char *prog, bool free_shared_ressources)
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

static void	print_horizontal_line(uint8_t board_size)
{
	int i;

	i = 0;
	ft_printf("+");
	while (i < board_size * 2 - 1)
	{
		ft_printf("-");
		i++;
	}
	ft_printf("+\n");
}

void		print_board(t_ctx *ctx)
{
	int	i;

	i = 0;
	print_horizontal_line(ctx->board_size);
	while (i < ctx->board_size * ctx->board_size)
	{
		ft_printf("|");
		if (ctx->board[i] == 0)
			ft_printf(" ");
		else
			ft_printf("%d", (int)ctx->board[i]);
		if (i % ctx->board_size == ctx->board_size - 1)
			ft_printf("|\n");
		i++;
	}
	print_horizontal_line(ctx->board_size);
}
