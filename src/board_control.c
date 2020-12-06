/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   board_control.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 18:51:17 by eduwer            #+#    #+#             */
/*   Updated: 2020/12/06 02:56:43 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lemipc.h>

bool	move_to_pos(t_ctx *ctx, uint8_t pos_x, uint8_t pos_y)
{
	if (pos_x >= ctx->board_size || pos_y >= ctx->board_size)
		return (false);
	ctx->board[ctx->pos_x + ctx->pos_y * ctx->board_size] = 0;
	ctx->pos_x = pos_x;
	ctx->pos_y = pos_y;
	ctx->board[ctx->pos_x + ctx->pos_y * ctx->board_size] = ctx->team_id;
	return (true);
}

uint8_t	get_cell_content(t_ctx *ctx, uint8_t x, uint8_t y)
{
	if (x >= ctx->board_size || y >= ctx->board_size)
		return (0);
	return (ctx->board[x + y * ctx->board_size]);
}

bool	is_surrounded(t_ctx *ctx)
{
	int		i;
	int		try_x;
	int		try_y;
	uint8_t	content;
	int		nb_contacts;

	i = 0;
	nb_contacts = 0;
	while (i < 9)
	{
		try_x = ctx->pos_x + i / 3 - 1;
		try_y = ctx->pos_y + i % 3 - 1;
		if (try_x >= 0 && try_x < ctx->board_size && try_y >= 0 \
			&& try_y < ctx->board_size)
		{
			content = get_cell_content(ctx, try_x, try_y);
			if (content != 0 && content != ctx->team_id)
				nb_contacts++;
		}
		i++;
	}
	if (nb_contacts >= 2)
		return (true);
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
			ctx->board[x] = ctx->team_id;
			return ;
		}
		x++;
	}
}
