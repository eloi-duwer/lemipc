/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemipc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 16:08:57 by eduwer            #+#    #+#             */
/*   Updated: 2020/11/25 21:48:17 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LEMIPC_H__
# define __LEMIPC_H__
# include <errno.h>
# include <fcntl.h>
# include <libft.h>
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
# include <mqueue.h>
# define LEMIPC_BOARD "/lemipc_shared_infos"
# define MQ_HOST "/lemipc_host"

typedef struct	s_ctx {
	bool		is_host;
	uint8_t		nb_teams;
	uint8_t		nb_players_per_team;
	uint8_t		team_id;
	uint8_t		player_id;
}				t_ctx;

#endif
