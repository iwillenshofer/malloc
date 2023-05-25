/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_mem_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwillens <iwillens@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/31 18:45:12 by iwillens          #+#    #+#             */
/*   Updated: 2023/05/25 18:51:21 by iwillens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	_clear_buffer(char *s)
{
	ft_bzero(s, PRINT_COLUMNS);
}

/*
** changes the color of print if needed. Prints black '.' if character is not
** printable, and the character if it is.
*/
void	_print_buffer_digit(char c, int *color)
{
	if (ft_isprint(c))
	{
		if (*color != WHITE)
		{
			*color = WHITE;
			ft_color(WHITE, REGULAR);
		}
		ft_putchar(c);
	}
	else
	{
		if (*color != BLACK)
		{
			*color = BLACK;
			ft_color(BLACK, REGULAR);
		}
		ft_putchar('.');
	}
}

void	_print_buffer(char *s)
{
	size_t	i;
	int		color;

	color = BLACK;
	ft_color(BLACK, REGULAR);
	i = 0;
	ft_putstr("  ");
	while (i < PRINT_COLUMNS)
	{
		_print_buffer_digit(s[i], &color);
		i++;
	}
	ft_color(RESET, REGULAR);
	ft_putchar('\n');
}

void	_print_zone_hex(t_zone *zone)
{
	char	*addr;
	t_alloc	*next_alloc;
	char	buffer[PRINT_COLUMNS + 1];

	_clear_buffer((char *)&buffer);
	addr = (char *)zone;
	next_alloc = zone->allocs;
	while (addr < (char *)(zone) + zone->size)
	{
		if (addr == (char *)zone)
			ft_color(WHITE, BOLD);
		else if (addr > (char *)zone && next_alloc && addr < (char *)next_alloc)
			ft_color(WHITE, REGULAR);
		else if (next_alloc && addr == (char *)next_alloc)
			ft_color(CYAN, BOLD);
		else if (next_alloc && addr > (char *)next_alloc && addr < (char *)(next_alloc->ptr))
			ft_color(CYAN, REGULAR);
		else if (next_alloc && addr > (char *)next_alloc && addr < (char *)next_alloc->ptr + next_alloc->size)
			ft_color(CYAN, FAINT);
		else
			ft_color(WHITE, FAINT);
		if (next_alloc && addr + 1 == (char *)(next_alloc->next))
			next_alloc = next_alloc->next;
		if (!((addr - (char *)zone) % PRINT_COLUMNS))
		{
			ft_puthex((size_t)(addr));
			ft_putchar(' ');
		}
		if (ALIGNMENT > 1 && !((addr - (char *)zone) % ALIGNMENT))
			ft_putchar(' ');
		ft_puthexbyte(*addr);
		buffer[(addr - (char *)zone) % PRINT_COLUMNS] = *addr;
		addr++;
		if (!((addr - (char *)zone) % PRINT_COLUMNS))
			_print_buffer((char *)&buffer);
		else
			ft_putchar(' ');
		ft_color(RESET, REGULAR);
	}
}

void	_print_alloc_ex(t_alloc *alloc)
{
	ft_color(BLACK, FAINT);
	ft_puthex((size_t)(alloc));
	ft_putchar(' ');
	ft_color(RESET, REGULAR);
	ft_puthex((size_t)(alloc->ptr));
	ft_putstr(" - ");
	ft_putnbr((size_t)(alloc->ptr) + alloc->size - 1);
	ft_putstr(" : ");
	ft_color(GREEN, REGULAR);
	if (alloc->size % ALIGNMENT)
		ft_color(RED, REGULAR);
	ft_putnbr(alloc->size);
	ft_putstr(" bytes\n");
	ft_color(RESET, REGULAR);
}

void	_print_free_bytes(t_zone *zone)
{
	size_t	free;
	size_t	count;
	t_alloc	*allocs;

	ft_putstr("ALLOC SIZE:");
	ft_putnbr(_aligned_size(sizeof(t_alloc)));
	ft_putstr(" : ");
	ft_putnbr((sizeof(t_alloc)));
	ft_putchar('\n');
	count = 0;
	allocs = zone->allocs;
	free = zone->size - _aligned_size(sizeof(t_zone));
	while (allocs)
	{
		count++;
		free -= allocs->size;
		free -= _aligned_size(sizeof(t_alloc));
		allocs = allocs->next;
	}
	ft_color(GREEN, REGULAR);
	ft_putnbr(free);
	ft_putstr(" free bytes. ");
	ft_color(RED, REGULAR);
	ft_putnbr(zone->size - free);
	ft_putstr(" used bytes. ");
	ft_color(YELLOW, REGULAR);
	ft_putnbr(count);
	ft_putstr(" allocations. ");
	ft_color(PURPLE, REGULAR);
	if (zone->type == TINY)
		ft_putnbr(TINY_LIMIT);
	else if (zone->type == MEDIUM)
		ft_putnbr(MEDIUM_LIMIT);
	else
		ft_putstr("undetermined");
	ft_putstr(" max size. ");
	ft_color(RESET, REGULAR);
}

void	_print_zone_ex(t_zone *zone)
{
	if (zone->type == TINY)
		ft_putstr("TINY: ");
	else if (zone->type == MEDIUM)
		ft_putstr("SMALL: ");
	else
		ft_putstr("LARGE: ");
	ft_puthex((size_t)zone);
	ft_color(CYAN, REGULAR);
	ft_putchar(' ');
	_print_free_bytes(zone);
	ft_putnbr((size_t)(zone->size));
	ft_putstr(" bytes");
	ft_color(RESET, REGULAR);
	ft_putchar('\n');
	_print_zone_hex(zone);
}

void	_print_zones_ex(void)
{
	t_zone	*head;

	head = g_zones;
	while (head)
	{
		_print_zone_ex(head);
		head = head->next;
	}
}