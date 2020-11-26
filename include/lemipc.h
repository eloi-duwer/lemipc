/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemipc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 16:08:57 by eduwer            #+#    #+#             */
/*   Updated: 2020/11/26 19:22:28 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIPC_H
# define LEMIPC_H
# include <errno.h>
# include <fcntl.h>
# include <libft.h>
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ipc.h>
# include <sys/mman.h>
# include <sys/msg.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
# include <semaphore.h>
# define LEMIPC "/lemipc"
# define DEFAULT_PLAYERS_PER_TEAM

typedef struct	s_ctx {
	bool		is_host;
	bool		has_semaphore;
	int			fd;
	sem_t		*sem;
	uint8_t		board_size;
	uint8_t		nb_teams;
	uint8_t		nb_players_per_team;
	uint8_t		team_id;
	uint8_t		player_id;
	uint8_t		pos_x;
	uint8_t		pos_y;
	uint8_t		*board;
	size_t		shared_ptr_size;
	uint8_t		*shared_ptr;
}				t_ctx;

enum	e_msg_type	{
	ping,
	attack,
	play,
	host_promotion,
	game_ended
};

/*
** print_helpers.c
** Functions about printing errors and usage
*/

void			error_and_exit(t_ctx *ctx, char *msg, \
	bool free_shared_ressources);
void			perror_and_exit(t_ctx *ctx, char *msg, \
	bool free_shared_ressources);
void			print_usage(t_ctx *ctx, char *prog, \
	bool free_shared_ressources);

/*
** helpers.c
** Functions about handling semaphores and shared memory managment
*/

void			free_ressources(t_ctx *ctx, bool free_shared_ressources);
void			get_infos_from_cli(t_ctx *ctx, int ac, char **av);
void			get_infos_from_shm(t_ctx *ctx);
void			acquire_sem(t_ctx *ctx);
void			release_sem(t_ctx *ctx);

/*
** shm_offsets.c
** Functions for getting various informations stored on shared memory
*/

size_t			get_shm_size(t_ctx *ctx);
uint8_t			*get_board_ptr(t_ctx *ctx);
key_t			*get_msg_queues(t_ctx *ctx);

/*
** board_control.c
** Functions about handling game board: moving to a position,
** getting content of a cell...
*/

void			move_to_pos(t_ctx *ctx, uint8_t pos_x, uint8_t pos_y);
uint8_t			get_cell_content(t_ctx *ctx, uint8_t x, uint8_t y);
bool			is_surrounded(t_ctx *ctx);

#endif
