/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_game_ended_msg.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 01:25:56 by eduwer            #+#    #+#             */
/*   Updated: 2020/12/13 18:56:07 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lemipc.h>

void	send_game_ended_and_exit(t_ctx *ctx)
{
	int			i;
	int			*queues;
	t_message	msg;

	i = 0;
	queues = get_msg_queues(ctx);
	msg.etype = 1;
	msg.content[0] = (uint8_t)e_game_ended;
	ft_printf("Team %d won the game!\nSending end game msg to other players \
and quitting...\n", ctx->team_id);
	while (i < ctx->nb_players_per_team * ctx->nb_teams)
	{
		if (queues[i] != -1 && queues[i] != ctx->own_msgq)
			msgsnd(queues[i], &msg, 1, 0);
		i++;
	}
	free_ressources(ctx, true);
	exit(0);
}
