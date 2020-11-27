/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msq_listening.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 15:27:12 by eduwer            #+#    #+#             */
/*   Updated: 2020/11/27 19:39:02 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lemipc.h>

static int	get_next_player_msgq(t_ctx *ctx, \
	uint8_t *team_id, uint8_t *player_id)
{
	if (*player_id >= ctx->nb_players_per_team - 1 \
		&& *team_id >= ctx->nb_teams - 1)
	{
		*player_id = 0;
		*team_id = 0;
	}
	else if (*team_id >= ctx->nb_teams - 1)
	{
		*team_id = 0;
		(*player_id)++;
	}
	else
		(*team_id)++;
	return (get_queue(ctx, *team_id, *player_id));
}

static void	msg_next_player(t_ctx *ctx)
{
	uint8_t		ids[2];
	int			msgid;
	t_message	msg;

	ids[0] = ctx->team_id;
	ids[1] = ctx->player_id;
	while (true)
	{
		msgid = get_next_player_msgq(ctx, &ids[0], &ids[1]);
		if (msgid == ctx->own_msgq)
			error_and_exit(ctx, \
				"All other players seems to have exited, quitting", true);
		if (msgid != -1)
		{
			msg.etype = 1;
			msg.content[0] = (uint8_t)e_play;
			msgsnd(msgid, &msg, 1, 0);
			return ;
		}
	}
}

static void	check_start_game(t_ctx *ctx)
{
	uint8_t i;

	i = 0;
	acquire_sem(ctx);
	while (i < ctx->nb_teams)
	{
		if (*(uint8_t *)(ctx->shared_ptr + 3 + i) != ctx->nb_players_per_team)
		{
			release_sem(ctx);
			return ;
		}
		i++;
	}
	ft_printf("All players have joined, sending msg to start the game\n");
	print_board(ctx);
	msg_next_player(ctx);
	release_sem(ctx);
}

void		play_turn(t_ctx *ctx)
{
	ft_printf("Hey my turn to play!\n");
	sleep(1);
	acquire_sem(ctx);
	msg_next_player(ctx);
	release_sem(ctx);
}

void		start_listening(t_ctx *ctx)
{
	t_message msg;

	check_start_game(ctx);
	while (true)
	{
		if (msgrcv(ctx->own_msgq, &msg, 3, 0, 0) == -1)
			perror_and_exit(ctx, "Error on msgrcv", ctx->is_host);
		else if ((enum e_msg_type)msg.content[0] == e_attack)
		{
			ctx->target_x = msg.content[1];
			ctx->target_y = msg.content[2];
		}
		else if ((enum e_msg_type)msg.content[0] == e_play)
		{
			play_turn(ctx);
		}
		else if ((enum e_msg_type)msg.content[0] == e_host_promotion)
			ctx->is_host = true;
		else if ((enum e_msg_type)msg.content[0] == e_game_ended)
		{
			free_ressources(ctx, false);
			exit(0);
		}
	}
}
