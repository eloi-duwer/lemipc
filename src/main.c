/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 15:55:13 by eduwer            #+#    #+#             */
/*   Updated: 2020/11/25 22:05:50 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lemipc.h>

size_t	get_struct_size(int size_board, int nb_teams, int nb_players_per_team)
{
	(void)nb_teams;
	(void)nb_players_per_team;
	return (2 + size_board * size_board);
}

void	perror_and_exit(char *msg)
{
	char	*str;

	str = strerror(errno);

	ft_fdprintf(2, "%s: %s\n", msg, str);
	exit(1);
}

void	init_message_queue(bool is_host)
{
	mqd_t			mq_id;
	char			msg[8192];
	struct mq_attr	attr;
	if (is_host)
	{
		ft_memset(&attr, 0, sizeof attr);
		attr.mq_maxmsg = 64;
		attr.mq_msgsize = 64;
		if ((mq_id = mq_open(MQ_HOST, O_RDONLY | O_CREAT, 0644, NULL)) == (mqd_t)-1)
			perror_and_exit("Error on mq_open with create");
		if (mq_receive(mq_id, msg, 8192, NULL) == -1)
			perror_and_exit("Error on mq_receive");
		ft_printf("Received message: %s\n", msg);
	}
	else
	{
		if ((mq_id = mq_open(MQ_HOST, O_WRONLY)) == (mqd_t)-1)
			perror_and_exit("Error on mq_open");
		if (mq_send(mq_id, "Hi from player 2!\0", 18, 1) == -1)
			perror_and_exit("Error on mq_send");
	}
	mq_close(mq_id);
	if (is_host)
		mq_unlink(MQ_HOST);
}

void	create_mem_if_needed(int fd)
{
	struct stat	stats;
	void		*addr;
	size_t		struct_size;
	bool		initialize;

	struct_size = get_struct_size(5, 1, 1);
	if (fstat(fd, &stats) == -1)
		perror_and_exit("Error on fstat");
	ft_printf("fstat size: %d\n", (int)stats.st_size);
	if (stats.st_size == 0)
	{
		initialize = true;
		if (ftruncate(fd, struct_size) == -1)
			perror_and_exit("Error on ftruncat");
	}
	else
		initialize = false;
	if ((addr = mmap(NULL, struct_size, \
		PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == NULL)
		perror_and_exit("Error on mmap");
	ft_printf("mmaped successfully, address is %p\n", addr);
	if (initialize == true)
		ft_memcpy(addr, "coucou\n", 7);
	else
		ft_printf("Content of memory: %s\n", addr);
	init_message_queue(initialize);
	if (initialize == true)
	{
		ft_printf("Closing shm interface\n");
		munmap(addr, struct_size);
		close(fd);
		shm_unlink(LEMIPC_BOARD);
	}
}

int		main(int argc, char **argv)
{
	int fd;

	(void)argc;
	(void)argv;
	if ((fd = shm_open(LEMIPC_BOARD, O_RDWR | O_CREAT, 0644)) < 0)
		perror_and_exit("Error on shm_open");
	ft_printf("fd = %d\n", fd);
	create_mem_if_needed(fd);
	return (0);
}
