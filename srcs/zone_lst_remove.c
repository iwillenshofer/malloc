/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zone_lst_remove.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwillens <iwillens@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 00:14:02 by iwillens          #+#    #+#             */
/*   Updated: 2023/05/15 18:27:52 by iwillens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_malloc.h"

void    deallocate(t_zone *zone)
{
	int ret;
	/*
	** TODO: SOME ERROR TREATMENT HERE, IN CASE ALLOCATION WAS NOT SUCCESSFULL.
	*/
	ret = munmap(zone, zone->size);
}

void    zone_remove(t_zone *zone)
{
	t_zone *head;

	head = g_zones;
	if (head == zone)
		g_zones = head->next;
	else
	{
		while (head && head != zone)
			head = head->next;
		if (head == zone)
			head->prev->next = head->next;
	}
	if (head->next)
		head->next->prev = head->prev;
	if (head->prev)
		head->prev->next = head->next;
	deallocate(head);
}
