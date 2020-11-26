/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   board_control.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 18:51:17 by eduwer            #+#    #+#             */
/*   Updated: 2020/11/26 19:23:07 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lemipc.h>

void	move_to_pos(t_ctx *ctx, uint8_t pos_x, uint8_t pos_y)
{
	ctx->board[ctx->pos_x + ctx->pos_y * ctx->board_size] = 0;
	ctx->pos_x = pos_x;
	ctx->pos_y = pos_y;
	ctx->board[ctx->pos_x + ctx->pos_y * ctx->board_size] = ctx->team_id + 1;
}

uint8_t	get_cell_content(t_ctx *ctx, uint8_t x, uint8_t y)
{
	return (ctx->board[x + y * ctx->board_size]);
}

bool	is_surrounded(t_ctx *ctx)
{
	uint8_t x[2];
	uint8_t y[2];
	uint8_t	nb_touching;

	y[0] = ctx->pos_y - (uint8_t)(ctx->pos_y > 0);
	y[1] = ctx->pos_y + (uint8_t)(ctx->pos_y < ctx->board_size - 1);
	x[1] = ctx->pos_x + (uint8_t)(ctx->pos_x < ctx->board_size - 1);
	nb_touching = 0;
	while (y[0] != y[1])
	{
		x[0] = ctx->pos_x - (uint8_t)(ctx->pos_x > 0);
		while (x[0] != x[1])
		{
			if (get_cell_content(ctx, x[0], y[0]) != 0 \
				&& get_cell_content(ctx, x[0], y[0]) != ctx->team_id)
				nb_touching++;
			if (nb_touching >= 2)
				return (true);
			x[0]++;
		}
		y[0]++;
	}
	return (false);
}
