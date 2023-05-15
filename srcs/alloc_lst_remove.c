/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_lst_remove.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwillens <iwillens@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 15:18:01 by iwillens          #+#    #+#             */
/*   Updated: 2023/05/15 19:34:54 by iwillens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_malloc.h"

void	free_alloc(t_alloc *alloc, t_zone *zone)
{
	if (alloc->prev)
		alloc->prev->next = alloc->next;
	if (alloc->next)
		alloc->next->prev = alloc->prev;
	if (zone->allocs == alloc)
		zone->allocs = alloc->next;
	if (!(alloc->next) && !(alloc->prev) && zone != g_zones && zone != g_zones->next)
		zone_remove(zone);
}

/*
** loop through the zones to find the allocation.
*/
void	alloc_remove(void *ptr)
{
	t_zone	*zone;
	t_alloc	*alloc;

	zone = g_zones;
	while (zone)
	{
		alloc = zone->allocs;
		if ((char *)zone < (char *)ptr
			&& (char *)ptr < (char *)zone + zone->size)
		{
			while (alloc)
			{
				if (alloc->ptr == ptr)
				{
					/*free*/
					free_alloc(alloc, zone);
					return ;
				}
				else if ((char *)ptr > (char *)alloc
					&& (char *)ptr < ((char *)(alloc->ptr) + alloc->size))
				{
					/*attempting free on address which was not malloc()-ed (bad-free)*/
					return ;
				}
				alloc = alloc->next;
			}
		}
		zone = zone->next;
	}
	/*if we reached here, the memory was not malloc()-ed*/
}
