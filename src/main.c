/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 15:55:13 by eduwer            #+#    #+#             */
/*   Updated: 2020/11/26 19:17:19 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lemipc.h>

static void	init_shm_infos(t_ctx *ctx)
{
	int		i;
	key_t	key;

	acquire_sem(ctx);
	*ctx->shared_ptr = ctx->board_size;
	*(ctx->shared_ptr + 1) = ctx->nb_teams;
	*(ctx->shared_ptr + 2) = ctx->nb_players_per_team;
	i = 0;
	while (i < ctx->nb_teams * ctx->nb_players_per_team)
	{
		if ((key = msgget(IPC_PRIVATE, 0644)) == -1)
			error_and_exit(ctx, "Error on msgget", true);
		ft_memcpy(ctx->shared_ptr + 3 + ctx->nb_teams +\
			(i * sizeof(key_t)), &key, sizeof(key_t));
		i++;
	}
	ft_memset(get_board_ptr(ctx), 0, ctx->board_size * ctx->board_size);
	release_sem(ctx);
	ft_printf("Host: initialized shared memory: board size: %d, %d teams, %d \
players per team\n", ctx->board_size, ctx->nb_teams, ctx->nb_players_per_team);
}

void		get_player_id(t_ctx *ctx)
{
	acquire_sem(ctx);
	ctx->player_id = (*(uint8_t *)(ctx->shared_ptr + 3 + ctx->team_id));
	if (ctx->player_id >= ctx->nb_players_per_team)
		error_and_exit(ctx, "This team is already full, exiting.", false);
	++(*(uint8_t *)(ctx->shared_ptr + 3 + ctx->team_id));
	release_sem(ctx);
}

void		init_host(t_ctx *ctx, int ac, char **av)
{
	ctx->is_host = true;
	ctx->team_id = 0;
	get_infos_from_cli(ctx, ac, av);
	ctx->shared_ptr_size = get_shm_size(ctx);
	if (ftruncate(ctx->fd, ctx->shared_ptr_size) == -1)
		perror_and_exit(ctx, "Error on ftruncat", true);
	if ((ctx->shared_ptr = mmap(NULL, ctx->shared_ptr_size, \
		PROT_READ | PROT_WRITE, MAP_SHARED, ctx->fd, 0)) == NULL)
		perror_and_exit(ctx, "Error on mmap", ctx->is_host);
	init_shm_infos(ctx);
	get_player_id(ctx);
}

void		init_client(t_ctx *ctx, size_t size, int ac, char **av)
{
	ctx->is_host = false;
	if (ac != 2)
		print_usage(ctx, av[0], false);
	ctx->shared_ptr_size = size;
	if ((ctx->shared_ptr = mmap(NULL, ctx->shared_ptr_size, \
		PROT_READ | PROT_WRITE, MAP_SHARED, ctx->fd, 0)) == NULL)
		perror_and_exit(ctx, "Error on mmap", ctx->is_host);
	get_infos_from_shm(ctx);
	ctx->team_id = (uint8_t)ft_atoi(av[1]) - 1;
	if (ctx->team_id >= ctx->nb_teams)
		error_and_exit(ctx, "Team number is too high", false);
	get_player_id(ctx);
}

int			main(int argc, char **argv)
{
	struct stat	stats;
	t_ctx		context;

	ft_bzero(&context, sizeof(context));
	if ((context.fd = shm_open(LEMIPC, O_RDWR | O_CREAT, 0644)) < 0)
		perror_and_exit(&context, "Error on shm_open", true);
	if (fstat(context.fd, &stats) == -1)
		perror_and_exit(&context, "Error on fstat", true);
	if ((context.sem = sem_open(LEMIPC, O_RDWR | O_CREAT, 0644, 1))\
		== SEM_FAILED)
		perror_and_exit(&context, "Error on sem_open", true);
	if (stats.st_size == 0)
		init_host(&context, argc, argv);
	else
		init_client(&context, stats.st_size, argc, argv);
	ft_printf("Process initiated, waiting for messages\n");
	ft_printf("Team: %d, Player: %d\n", context.team_id + 1, \
		context.player_id + 1);
	while (1)
		;
	free_ressources(&context, context.is_host);
	return (0);
}
