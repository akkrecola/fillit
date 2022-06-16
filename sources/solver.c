/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elehtora <elehtora@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 11:41:18 by elehtora          #+#    #+#             */
/*   Updated: 2022/06/16 16:46:12 by elehtora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

static void	print_board(uint16_t *board)
{
	int	j;
	int	i;

	j = 0;
	while (j < BOARD_SIZE)
	{
		i = MAX_SHIFT;
		while (i >= 0)
		{
			if (board[j] & 1L << i)
				printf("#");
			else
				printf(".");
			i--;
		}
		printf("\n");
		j++;
	}
	printf("\n");
}

static int	solve(t_piece *p, uint16_t *board, int area)
{
	uint8_t	x;
	uint8_t	y;

	if (p->id == 0)
	{
		printf("ID is 0, we placed all pieces and a solution was found!");
		return (1);
	}
	x = 0;
	y = 0;
	while (y + p->height <= area)
	{
		while (x + p->width <= area)
		{
			printf("Board state while trying to place %c at X %hhu, Y %hhu\n", p->id, x, y);
			print_board(board);
			if ((*(uint64_t *)&board[y] & p->piece >> x) == 0)
			{
				*(uint64_t *)&board[y] ^= p->piece >> x;
				p->pos = ((uint16_t) x) << 8 | y;
				printf("Placement of %c at X %hhu, Y %hhu\n", p->id, x, y);
				print_board(board);
				if (solve(p + 1, board, area) == 1)
					return (1);
				*(uint64_t *)&board[y] ^= p->piece >> x;
				printf("State after removing %c from X %hhu, Y%hhu\n", p->id, x, y);
				p->pos = NOT_PLACED;
				print_board(board);
			}
			else
				printf("Piece %c didn't fit at X %hhu,  Y %hhu, checking next position..\n\n", p->id, x, y);
			x++;
		}
		x = 0;
		y++;
	}
	printf("Piece %c didn't fit on the board, returning 0 and trying a different order.\n\n", p->id);
	return (0);
}

/*https://stackoverflow.com/questions/4816322/mapping-x-y-to-single-number-value*/
static void	print_solution(uint16_t *board, t_piece *p, char *solution, int area)
{
	uint8_t		x;
	uint8_t		y;
	uint8_t		shift;
	uint8_t		offset;

	while (p->id != 0)
	{
		x = (uint8_t)(p->pos >> 8);
		y = (uint8_t)(p->pos & 0xff);
		/*printf("Position for id %c: %hu\n", p->id, p->pos);*/
		/*printf("X for id %c: %hhu\n", p->id, x);*/
		/*printf("Y for id %c: %hhu\n", p->id, y);*/

		/*Align with top*/
		/*
		 *while ((p->piece & 0xFFFF000000000000L) == 0)
		 *    p->piece <<= BOARD_SIZE;
		 */
		shift = 15;
		while (shift < PIECE_BITS)
		{
			while (shift % 16 != 0)
			{
				/*printf("%hhu ", shift);*/
				if (p->piece & (1L << shift))
				{
					offset = 15 - (shift % 16);
					/*printf("INSIDE, shift: %hhu, x: %hhu, y: %hhu, offset: %hhu\n", shift, x, y, offset);*/
					/*solution[ (y * 16) + ((3 - (shift / 16)) * 16) + (x + (16 - (shift % 16))) ] = p->id;*/
					/*printf("Solution placement at %d\n", y * 16 + (shift - (shift % 16)) + x + offset);*/
					solution[ (y * 16 + (shift - (shift % 16))) + x + offset ] = p->id;
					/*printf("%d\n", y * 16 + x + (shift - (shift % 16)));*/
					/*printf("ROW %d\n", (3 - (shift / 16)) * 16); // row*/
				}
				shift--;
			}
			shift += 31;
		}
		p++;
	}
	ft_putendl("");
	shift = 0;
	while (shift < area)
	{
		ft_putmem(solution + (shift * BOARD_SIZE), area);
		ft_putendl("");
		shift++;
	}
}

/*
 *Driver for a recursive solver. Extends the solve area if a solution is not
 *found for area n - 1
 */
int	solve_driver(t_piece *pieces, uint16_t *board, int piece_total)
{
	int	area;
	/*solution is 16x16 grid char array (not C string)*/
	static char	solution[BOARD_SIZE * BOARD_SIZE];

	/*Initialize solution as blanks (.)*/
	ft_memset(solution, '.', BOARD_SIZE * BOARD_SIZE + BOARD_SIZE);
	area = 2;
	while (area * area < piece_total * 4)
		area++;

	/*TODO Flush board and N->pos between every area increment*/
	while (solve(pieces, board, area) != 1 && area <= BOARD_SIZE)
	{
		ft_bzero(board, 2 * BOARD_SIZE);
		area++;
	}
	printf("\nBoard state:\n");
	print_board(board);
	printf("The hex values of the rows on the board:\n");
	for (int i = 0; i < BOARD_SIZE; i++)
		printf("%d.\trow is \t%hX\n", i, board[i]);
	print_solution(board, pieces, &solution[0], area);
	return (0);
}
