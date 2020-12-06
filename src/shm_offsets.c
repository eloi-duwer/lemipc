/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shm_offsets.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 23:45:43 by eduwer            #+#    #+#             */
/*   Updated: 2020/12/06 03:14:02 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lemipc.h>

/*
** Structure of shared memory:
** board_size, nb_teams, nb_players_per_team (uint8_t)
** Array of uint8_t of size nb_teams, representing the number of players who
** joined by team. It is used at the creation of the process,
** and to determine if everybody joined the game
** Array of int, identifiers of MSGQ to every players, sorted by team
** Board, square of board_size length, each square containing the number
** of the team if a player is present, 0 else
*/

size_t	get_shm_size(t_ctx *ctx)
{
	return (3 + ctx->nb_teams + (ctx->nb_teams * ctx->nb_players_per_team * \
		sizeof(int)) + ctx->board_size * ctx->board_size);
}

uint8_t	*get_board_ptr(t_ctx *ctx)
{
	return (uint8_t *)(ctx->shared_ptr + (3 + ctx->nb_teams + \
		(ctx->nb_teams * ctx->nb_players_per_team * sizeof(int))));
}

int		*get_msg_queues(t_ctx *ctx)
{
	return (int *)(ctx->shared_ptr + (3 + ctx->nb_teams));
}

int		get_queue(t_ctx *ctx, uint8_t team_id, uint8_t player_id)
{
	int	*queue_ptr;

	queue_ptr = (int *)(ctx->shared_ptr + 3 + ctx->nb_teams);
	return (queue_ptr[((team_id - 1) * ctx->nb_players_per_team) + \
		(player_id - 1)]);
}

void	rm_own_queue(t_ctx *ctx)
{
	int	*queue_ptr;

	queue_ptr = (int *)(ctx->shared_ptr + 3 + ctx->nb_teams);
	msgctl(ctx->own_msgq, IPC_RMID, NULL);
	queue_ptr[(ctx->team_id - 1) * ctx->nb_players_per_team + \
		(ctx->player_id - 1)] = -1;
}
