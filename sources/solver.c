/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elehtora <elehtora@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 11:41:18 by elehtora          #+#    #+#             */
/*   Updated: 2022/06/10 19:24:17 by elehtora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"
#include "utils.h"

static void	toggle(t_piece *p, uint16_t *bits, uint8_t x, uint8_t y)
{
	/*Replace BOARD_SIZE with dynamic size from caller*/
	if (x < (BOARD_SIZE - p->width + 1) && y < (BOARD_SIZE - p->height + 1))
		*(uint64_t *)&bits[y] ^= p->piece >> x;
}

static int	what(t_piece *piece, uint16_t *board, int area)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (piece->id)
	{
		while (y < BOARD_SIZE)
		{
			if (*(uint64_t *)&board[y] & piece->piece >> x)
				x++;
			else
			{
				toggle(piece, &board[0], x, y);
				break;
			}
			if (x + piece->width > BOARD_SIZE)
			{
				y++;
				x = 0;
			}
		}
	}
	return (0);
}

/*Sorry for the bad variable names, but this is about to get tight as fuck.*/
/*
 *static int	solve(t_piece *p, uint16_t *b, uint8_t area, t_piece *prev)
 *{
 *    [>Accept<]
 *    if (p->id == 0)
 *        return (1);
 *    if (solve(pieces, board, area) == 1)
 *        update_solution();
 *
 *    while (p->y + p->height < area)
 *    {
 *        while (*(uint64_t *)&b[p->y] & (p->piece >> p->x))
 *        {
 *            
 *        }
 *        *(uint64_t *)&b[p->y] ^= //
 *    }
 *}
 */

/*
 *Driver for a recursive solver. Extends the solve area if a solution is not
 *found for area n - 1
 */
int	solve_driver(t_piece *pieces, uint16_t *board, uint8_t piece_total)
{
	int	area;

	area = ft_nat_sqrt(piece_total * 4);
	/*
	 *what(pieces, board, area);
	 *print_board(board);
	 */
	while (solve(pieces, board, area) != 1)
		area++;
	return (0);
}
