/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 10:09:08 by eduwer            #+#    #+#             */
/*   Updated: 2020/11/26 19:25:50 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lemipc.h>

void	free_ressources(t_ctx *ctx, bool free_shared_ressources)
{
	int		i;

	if (ctx->sem != NULL && ctx->has_semaphore == true)
		release_sem(ctx);
	if (ctx->sem != NULL)
		sem_close(ctx->sem);
	if (free_shared_ressources)
	{
		if (ctx->sem != NULL)
			sem_unlink(LEMIPC);
		if (ctx->shared_ptr != NULL)
		{
			i = -1;
			while (++i < ctx->nb_teams * ctx->nb_players_per_team)
				if (get_msg_queues(ctx)[i] != -1)
					msgctl(get_msg_queues(ctx)[i], IPC_RMID, NULL);
		}
	}
	if (ctx->shared_ptr != NULL)
		munmap(ctx->shared_ptr, ctx->shared_ptr_size);
	if (ctx->fd > 0)
		close(ctx->fd);
	if (free_shared_ressources)
		shm_unlink(LEMIPC);
}

void	get_infos_from_cli(t_ctx *ctx, int ac, char **av)
{
	int	nb;

	if (ac != 4)
		print_usage(ctx, av[0], true);
	nb = ft_atoi(av[1]);
	if (nb <= 1 || nb > 255)
		error_and_exit(ctx, "The board size must \
be an integer between 2 and 255", true);
	ctx->board_size = (uint8_t)nb;
	nb = ft_atoi(av[2]);
	if (nb <= 1 || nb > 255)
		error_and_exit(ctx, "The number of teams must \
be an integer between 2 and 255", true);
	ctx->nb_teams = (uint8_t)nb;
	nb = ft_atoi(av[3]);
	if (nb <= 1 || nb > 255)
		error_and_exit(ctx, "The number of teams must \
be an integer between 2 and 255", true);
	ctx->nb_players_per_team = (uint8_t)nb;
	if (ctx->board_size * ctx->board_size < \
		ctx->nb_teams * ctx->nb_players_per_team)
		error_and_exit(ctx, \
			"The board is too small for the number of players", true);
}

void	get_infos_from_shm(t_ctx *ctx)
{
	if (ctx->shared_ptr_size <= 3)
		error_and_exit(ctx, "Shared memory is too small", false);
	acquire_sem(ctx);
	ctx->board_size = (uint8_t)(*ctx->shared_ptr);
	ctx->nb_teams = (uint8_t)(*(ctx->shared_ptr + 1));
	ctx->nb_players_per_team = (uint8_t)(*(ctx->shared_ptr + 2));
	release_sem(ctx);
	if (ctx->shared_ptr_size != get_shm_size(ctx))
		error_and_exit(ctx, "Shared memory have an unexpected size", false);
	if (ctx->board_size * ctx->board_size < \
		ctx->nb_teams * ctx->nb_players_per_team)
		error_and_exit(ctx, \
			"The board is too small for the number of players", false);
}

void	acquire_sem(t_ctx *ctx)
{
	sem_wait(ctx->sem);
	ctx->has_semaphore = true;
}

void	release_sem(t_ctx *ctx)
{
	sem_post(ctx->sem);
	ctx->has_semaphore = false;
}
