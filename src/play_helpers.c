/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 18:38:25 by eduwer            #+#    #+#             */
/*   Updated: 2020/12/06 03:13:39 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lemipc.h>
#include <limits.h>

void		check_game_over(t_ctx *ctx)
{
	uint8_t	msg;

	if (is_surrounded(ctx) == false)
		return ;
	ctx->board[ctx->pos_x + ctx->pos_y * ctx->board_size] = 0;
	if (ctx->is_host == true)
	{
		msg = (uint8_t)e_host_promotion;
		send_msg_to_next_player(ctx, &msg, 1);
	}
	msg = (uint8_t)e_play;
	send_msg_to_next_player(ctx, &msg, 1);
	rm_own_queue(ctx);
	error_and_exit(ctx, "Player is surrounded, game over.", false);
}

static bool	check_target_and_send(t_ctx *ctx, int pos_x, int pos_y)
{
	uint8_t		content;
	t_message	msg;
	int			msgid;

	msg.etype = 1;
	if (pos_x < 0 || pos_x >= ctx->board_size \
		|| pos_y < 0 || pos_y >= ctx->board_size)
		return (false);
	content = get_cell_content(ctx, (uint8_t)pos_x, (uint8_t)pos_y);
	if (content == 0 || content == ctx->team_id)
		return (false);
	ctx->target_x = pos_x;
	ctx->target_y = pos_y;
	msg.content[0] = (uint8_t)e_attack;
	msg.content[1] = pos_x;
	msg.content[2] = pos_y;
	content = 1;
	while (content <= ctx->nb_players_per_team)
	{
		msgid = get_queue(ctx, ctx->team_id, content);
		if (msgid != -1 && msgid != ctx->own_msgq)
			msgsnd(msgid, &msg, 3, 0);
		content++;
	}
	return (true);
}

void		find_and_send_new_target(t_ctx *ctx)
{
	int		delta[2];
	int		dist;

	dist = 1;
	while (dist < (int)ctx->board_size)
	{
		delta[0] = -dist;
		while (delta[0] != dist)
		{
			delta[1] = -dist;
			while (delta[1] != dist)
			{
				if (check_target_and_send(ctx, ctx->pos_x + delta[0], \
					ctx->pos_y + delta[1]) == true)
					return ;
				delta[1]++;
			}
			delta[0]++;
		}
		dist++;
	}
	send_game_ended_and_exit(ctx);
}

static int	dist(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	return (ft_abs(x2 - x1) + ft_abs(y2 - y1));
}

void		move_towards_target(t_ctx *ctx)
{
	t_move	m;

	if (dist(ctx->pos_x, ctx->pos_y, ctx->target_x, ctx->target_y) == 1)
		return ;
	m.i = 0;
	m.min_dist = INT_MAX;
	while (m.i < 8)
	{
		m.try_x = ctx->pos_x + (m.i / 3) - 1;
		m.try_y = ctx->pos_y + (m.i % 3) - 1;
		if (m.try_x >= 0 && m.try_x < ctx->board_size && m.try_y >= 0 && \
			m.try_y < ctx->board_size && \
			get_cell_content(ctx, m.try_x, m.try_y) == 0 && \
			dist(m.try_x, m.try_y, ctx->target_x, ctx->target_y) < m.min_dist)
		{
			m.min_dist = dist(m.try_x, m.try_y, ctx->target_x, ctx->target_y);
			m.next_x = m.try_x;
			m.next_y = m.try_y;
		}
		m.i++;
	}
	if (m.min_dist != INT_MAX)
		move_to_pos(ctx, m.next_x, m.next_y);
}
