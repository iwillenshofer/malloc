/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_realloc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwillens <iwillens@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 14:46:05 by iwillens          #+#    #+#             */
/*   Updated: 2023/06/07 22:05:45 by iwillens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
** find an allocation by pointer inside a zone.
** will only return if it is the pointer allocated to the user.
** if the pointer lies within an allocation but is not alloc->ptr,
** returns null.
*/
t_alloc	*__find_alloc(void *ptr, t_zone *zone)
{
	t_alloc	*alloc;

	if (!(ptr) || !(zone) || (zone && !(zone->allocs)))
		return (NULL);
	alloc = zone->allocs;
	while (alloc)
	{
		if (alloc->ptr == ptr)
			return (alloc);
		else if ((char *)alloc < (char *)ptr
			&& (char *)ptr < (char *)alloc + alloc->size)
			return (NULL);
		alloc = alloc->next;
	}
	return (NULL);
}

/*
** tries to create a new allocation and, if succeds, copies and frees
** the previous pointer.
*/
t_alloc	*__reallocate(t_alloc *alloc, size_t size)
{
	t_alloc	*new_alloc;
	size_t	limit;

	new_alloc = _alloc_add(size);
	limit = size;
	if (size > alloc->size)
		limit = alloc->size;
	if (new_alloc && new_alloc->ptr)
	{
		ft_memcpy(new_alloc->ptr, alloc->ptr, limit);
		_alloc_remove(alloc->ptr);
		return (new_alloc);
	}
	else
		return (NULL);
}

/*
** returns true if there is the space required by size between current and
** next allocation.
*/
char	__is_space_realloc_middle(t_alloc *alloc, size_t size)
{
	return (alloc->next
		&& (size_t)((char *)(alloc->next) - _endalloc(alloc)) >= size);
}

/*
** returns true if there is no next allocation, and there is enough space
** between current alloc and the required size.
*/
char	__is_space_realloc_end(t_alloc *alloc, t_zone *zone, size_t size)
{
	return (!(alloc->next)
		&& zone->size - (_endalloc(alloc) - (char*)(zone)) >= size);
}

/*
** case 1: if types are different, new allocation will occur!
** case 2: shrinking size.
** case 3: there is space between current allocation and next allocation.
** case 4: there is no next allocation, and there is space at the end of
** the zone.
** else : there is no space for reallocation. Must reallocate.
** all in all, cases 2, 3 and 4 will just resize. Otherwise, reallocate.
*/
t_alloc	*_alloc_realloc(void *ptr, size_t size)
{
	char	new_type;
	t_zone	*zone;
	t_alloc	*alloc;
	t_alloc	*new_alloc;

	size = _aligned_size(size);
	new_type = _get_type(size);
	zone = _find_zone(ptr);
	alloc = __find_alloc(ptr, zone);
	new_alloc = alloc;
	if (!zone || !alloc)
		return (NULL);
	if (new_type != zone->type)
		new_alloc = __reallocate(alloc, size);
	else if ((size <= alloc->size) || (__is_space_realloc_middle(alloc, size))
		|| (__is_space_realloc_end(alloc, zone, size)))
		alloc->size = size;
	else
		new_alloc = __reallocate(alloc, size);
	return (new_alloc);
}
