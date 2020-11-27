/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   board_control.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 18:51:17 by eduwer            #+#    #+#             */
/*   Updated: 2020/11/27 17:12:49 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lemipc.h>

void	move_to_pos(t_ctx *ctx, uint8_t pos_x, uint8_t pos_y)
{
	if (pos_x >= ctx->board_size || pos_y >= ctx->board_size)
		return ;
	ctx->board[ctx->pos_x + ctx->pos_y * ctx->board_size] = 0;
	ctx->pos_x = pos_x;
	ctx->pos_y = pos_y;
	ctx->board[ctx->pos_x + ctx->pos_y * ctx->board_size] = ctx->team_id + 1;
}

uint8_t	get_cell_content(t_ctx *ctx, uint8_t x, uint8_t y)
{
	if (x >= ctx->board_size || y >= ctx->board_size)
		return (0);
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

/*
** Tries to find a random position the board,
** If there is already something we find the first empty cell on the board
** Instead of choosing a random position
*/

void	set_start_position(t_ctx *ctx)
{
	int	x;
	int	y;

	srand(time(NULL));
	x = (uint8_t)((double)rand() / (double)RAND_MAX * (double)ctx->board_size);
	y = (uint8_t)((double)rand() / (double)RAND_MAX * (double)ctx->board_size);
	if (get_cell_content(ctx, x, y) == 0)
	{
		ctx->pos_x = x;
		ctx->pos_y = y;
		move_to_pos(ctx, x, y);
		return ;
	}
	x = 0;
	while (x < ctx->board_size * ctx->board_size)
	{
		if (ctx->board[x] == 0)
		{
			ctx->pos_x = x % ctx->board_size;
			ctx->pos_y = x / ctx->board_size;
			ctx->board[x] = ctx->team_id + 1;
			return ;
		}
		x++;
	}
}
