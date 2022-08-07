/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 18:51:29 by ebresser          #+#    #+#             */
/*   Updated: 2022/08/07 22:58:25 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine(void *args)
{
	t_status	*status;
	int			i;
	pthread_t		checker;
	void		*current_philo;

	status = (t_status *)args;
//	pthread_mutex_lock(&status->n_thread_philo);
	i = status->n_thread;
	current_philo = &(status->philo[status->n_thread]);
//	pthread_mutex_unlock(&status->n_thread_philo);
	if (pthread_create(&checker,
				NULL, &routine_checker, current_philo))
		return (FALSE);
	pthread_detach(checker);
	if(status->input.num_of_times_eat > 0 && \
		status->input.num_of_times_eat > status->philo[i].num_of_times_ate)
		while (routine_execute(status, i))
		continue ;
	return (NULL);
}

int	routine_execute(t_status *status, int i)
{
	if (philo_eat(status, i) == FALSE)
		return (FALSE);
	if (status->input.num_of_times_eat > 0 && \
		status->philo[i].num_of_times_ate >= status->input.num_of_times_eat)
	{
		philo_print(status, status->philo[i].id, BOLD_ORANGE, SATIATED);
		return (FALSE);
	}
	if (philo_sleep(status, i) == FALSE)
		return (FALSE);
	if (philo_think(status, i) == FALSE)
		return (FALSE);
	return (TRUE);
}

void	*routine_checker(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	while (1)
	{
		if (philo_is_dead(philo) == TRUE)
			break ;
	}
	return (NULL);
}

int	philo_print(t_status *status, int id, char *color, char *action)
{
	long long	now;

	now = duration_ms(status->t0);
	if (status->philo_dead == TRUE)
		return (FALSE);
	pthread_mutex_lock(&status->printer);
	if (status->philo_dead == TRUE)
	{
		pthread_mutex_unlock(&status->printer);
		return (FALSE);
	}
	else
		printf("%s%-10lld %-3d %-30s%s\n", color, now, id, action, RESET);
	pthread_mutex_unlock(&status->printer);
	return (TRUE);
}
