/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemipc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 16:08:57 by eduwer            #+#    #+#             */
/*   Updated: 2020/12/06 01:36:46 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIPC_H
# define LEMIPC_H
# include <errno.h>
# include <fcntl.h>
# include <libft.h>
# include <semaphore.h>
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ipc.h>
# include <sys/mman.h>
# include <sys/msg.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <time.h>
# include <unistd.h>
# define LEMIPC "/lemipc"
# define DEFAULT_PLAYERS_PER_TEAM

typedef struct	s_message {
	long		etype;
	uint8_t		content[3];
}				t_message;

typedef struct	s_ctx {
	bool		is_host;
	bool		has_semaphore;
	int			fd;
	int			own_msgq;
	sem_t		*sem;
	uint8_t		board_size;
	uint8_t		nb_teams;
	uint8_t		nb_players_per_team;
	uint8_t		team_id;
	uint8_t		player_id;
	uint8_t		pos_x;
	uint8_t		pos_y;
	uint8_t		target_x;
	uint8_t		target_y;
	uint8_t		*board;
	size_t		shared_ptr_size;
	uint8_t		*shared_ptr;
}				t_ctx;

typedef struct	s_move {
	int			min_dist;
	int			i;
	int			try_x;
	int			try_y;
	int			next_x;
	int			next_y;
}				t_move;

enum	e_msg_type	{
	e_attack,
	e_play,
	e_host_promotion,
	e_game_ended
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
void			print_board(t_ctx *ctx);

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
int				*get_msg_queues(t_ctx *ctx);
int				get_queue(t_ctx *ctx, uint8_t team_id, uint8_t player_id);
void			rm_own_queue(t_ctx *ctx);

/*
** board_control.c
** Functions about handling game board: moving to a position,
** getting content of a cell...
*/

bool			move_to_pos(t_ctx *ctx, uint8_t pos_x, uint8_t pos_y);
uint8_t			get_cell_content(t_ctx *ctx, uint8_t x, uint8_t y);
bool			is_surrounded(t_ctx *ctx);
void			set_start_position(t_ctx *ctx);

/*
** msq_listening.c
** Functions about handling message queues
*/

void			start_listening(t_ctx *ctx);
void			send_msg_to_next_player(t_ctx *ctx, uint8_t *content, \
	ssize_t content_size);

/*
** play_helpers.c
** Functions to check if the player is surrounded by at least two other players
** If it is the case, the player removes itself and exits
** + Functions to set new target and move towards target
*/

void			check_game_over(t_ctx *ctx);
void			find_and_send_new_target(t_ctx *ctx);
void			move_towards_target(t_ctx *ctx);

/*
**	send_game_ended_msg.c
**	When the game is ended, we have to send a msg to all the players to quit
*/

void			send_game_ended_and_exit(t_ctx *ctx);

#endif
