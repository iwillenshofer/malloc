/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_add.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwillens <iwillens@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 15:18:20 by iwillens          #+#    #+#             */
/*   Updated: 2023/06/07 22:00:15 by iwillens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/malloc.h"

char	_get_type(size_t size)
{
	if (size > SMALL_LIMIT)
		return (LARGE);
	if (size > TINY_LIMIT)
		return (SMALL);
	return (TINY);
}

t_alloc	*__newalloc(t_alloc *pos, t_alloc *prev, t_alloc *next, size_t size)
{
	pos->size = size;
	pos->ptr = ((char *)pos + _aligned_size(sizeof(t_alloc)));
	pos->prev = NULL;
	pos->next = NULL;
	if (prev)
	{
		pos->prev = prev;
		prev->next = pos;
	}
	if (next)
	{
		pos->next = next;
		next->prev = pos;
	}
	return (pos);
}

/*
** A "spot" is a free space for allocation. This function returns a pointer
** to an allocation. returns null if no spot was created (which means there
** is no space in the zone.
*/
t_alloc	*__find_spot(t_zone *zone, size_t s)
{
	t_alloc	*alloc;
	t_alloc	*h;

	alloc = NULL;
	h = zone->allocs;
	if (zone->free < (s + _aligned_size(sizeof(t_alloc))))
		return (NULL);
	if (!(h))
		alloc = __newalloc((t_alloc *)(_endzone(zone)), NULL, NULL, s);
	else if (_is_space_begin_zone(h, zone, s))
		alloc = __newalloc((t_alloc *)(_endzone(zone)), NULL, zone->allocs, s);
	if (alloc && (!(zone->allocs) || (zone->allocs == alloc->next)))
		zone->allocs = alloc;
	while (!(alloc) && h)
	{
		if (_is_space_middle_alloc(h, s))
			alloc = __newalloc((t_alloc *)
					(_endalloc(h) + h->size), h, h->next, s);
		else if (_is_space_end_alloc(h, zone, s))
			alloc = __newalloc((t_alloc *)(_endalloc(h) + h->size), h, NULL, s);
		h = h->next;
	}
	if (alloc)
		zone->free -= (s + _aligned_size(sizeof(t_alloc)));
	return (alloc);
}

/*
** 1. determine type
**
** 2. loop through the zones of the determined type
** and then the allocations within that zone to find the minimum space
** where the required allocation can fit.
**
** 3. if not found, create a zone.
** 4. Create an allocation on the available space.
**
** return the pointer.	
*/
t_alloc	*_alloc_add(size_t size)
{
	char	type;
	t_alloc	*allocated;
	t_zone	*zone;

	zone = g_zones;
	size = _aligned_size(size);
	type = _get_type(size);
	allocated = NULL;
	while (zone && !(allocated) && type != LARGE)
	{
		if (zone->type == type)
			allocated = __find_spot(zone, size);
		zone = zone->next;
	}
	if (!(allocated))
		zone = _zone_add(type, size);
	if (!(allocated) && zone)
	{
		allocated = __find_spot(zone, size);
		zone->allocs = allocated;
	}
	return (allocated);
}
