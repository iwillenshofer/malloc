/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwillens <iwillens@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 11:54:27 by iwillens          #+#    #+#             */
/*   Updated: 2023/06/03 18:58:28 by iwillens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTS_H
# define TESTS_H

# include "../includes/malloc.h"

# ifndef TINY_LIMIT
#  define TINY_LIMIT 10
# endif

# ifndef MEDIUM_LIMIT
#  define MEDIUM_LIMIT 20
# endif

void	test_malloc(void);
void	test_program_start(void);
void	test_realloc_bounderies(void);
void	test_realloc(void);

#endif
