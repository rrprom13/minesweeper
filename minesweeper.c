#include <curses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <math.h>
#include <assert.h>

#include "minesweeper.h"
#define KEY_ESCAPE 27
#define even(a) ((a) % 2 == (0) ? (a) : (a + 1))

int main()
{
	WINDOW* wnd = initscr();
	int nrows, ncols;
	getmaxyx(wnd, nrows, ncols);

	clear();
	noecho();
	cbreak();
	keypad(wnd, TRUE);
	nodelay(wnd, TRUE);
	set_escdelay (25);

	int p, r = 9, c = 9, b = 10, bs = 10;
	move (1, draw_frame(r, c, b)); 
	p = get_key_pressed();
	int rand, col, br, bc, ecran = 1, cursor, cursor2, ok = 1, date = 1, date2 = 1; // ecran memoreaza locatia in joc: 1 - pt jocul pr-zis; 2 - pt meniu; 3 - pt settings
	move (rand = even (r), col = even (c));	// r, c pt nr linii, col selectat; br, bc pt casuta curenta; rand, col pt cursor
	int mine[30][30], marked[30][30], flag[30][30], game = 1, fade = 1; // mine retine amplasarea minelor, marked - casutele deja lovite
	zero (mine);
	zero (marked);
	zero (flag);
	time_t st;
	time_t st2;	
	time_t fin;
	time_t fin2;
	time_t minus;
	time_t minus2;
	st = time(NULL);
	fin = time(NULL);
	st2 = time(NULL);
	fin2 = time(NULL);
	while (1) {
		p = get_key_pressed();
		if (ecran == 1) {//game frame
			move (9, c * 2 + 10);
			dl (25);
			time_t rawtime;
			time (&rawtime);
			if (fade == 1) { 
				st = time(NULL);
				fade = 0;
			}
  			mvprintw (9, c * 2 + 10, "%s", ctime (&rawtime));
			box (wnd, ACS_VLINE, ACS_HLINE);
			if (rand > 2 && tolower (p) == KEY_UP && game == 1) {
				rand = rand - 2;
				move (rand, col);
			}
			if (rand < r * 2 && tolower (p) == KEY_DOWN && game == 1) {
				rand = rand + 2;
				move (rand, col);
			}
			if (col > 2 && tolower (p) == KEY_LEFT && game == 1) {
				col = col - 2;
				move (rand, col);
			}
			if (col < c * 2 && tolower (p) == KEY_RIGHT && game == 1) {
				col = col + 2;
				move (rand, col);
			}
			if (tolower (p) == 'm' && col % 2 == 0 && marked[rand / 2 - 1][col / 2 - 1] == 0 && game == 1) {
				if (steag (rand, col) == 1) {
					bs++;
					flag[rand / 2 - 1][col / 2 - 1] = 0;
				}
				else {
					bs--;
					flag[rand / 2 - 1][col / 2 - 1] = 1;
				}
				br = rand / 2;
				bc = col / 2;
				modify (c, br, bc, bs);
				move (rand, col);
			}
			if (tolower (p) == 'm' && col % 2 == 0 && game == 1 && marked[rand / 2 - 1][col / 2 - 1] == 1 && mine[rand / 2 - 1][col / 2 - 1] > 0 && mine[rand / 2 - 1][col / 2 - 1] < 9) {
				bonus2 (rand, col, r, c, mine, marked, flag, &bs);
				br = rand / 2;
				bc = col / 2;
				modify (c, br, bc, bs);
				move (rand, col);
			}
			if (tolower (p) == 'x' && date == 1) {
				st2 = time(NULL); 
				date = 0;
			}
			if (tolower (p) == 'x' && game == 1) {
				if (ok == 1) {
					int x, y;
					x = rand / 2 - 1;
					y = col / 2 - 1;
					mines (b, mine, x, y, r, c);
				}
				ok = 0;
				if (mine[rand / 2 - 1][col / 2 - 1] > 0 && mine[rand / 2 - 1][col / 2 - 1] < 9) {
					if (marked[rand / 2 - 1][col / 2 - 1] == 0 && flag[rand / 2 - 1][col / 2 - 1] == 0) {
						br = rand / 2;
						bc = col / 2;
						modify (c, br, bc, bs);
						color (rand, col, 1, mine);
						marked[br - 1][bc - 1] = 1;
					}
					if (marked[rand / 2 - 1][col / 2 - 1] == 1) 
						bonus1 (&rand, &col, r, c, mine, marked, flag);
				}
				if (mine[rand / 2 - 1][col / 2 - 1] == 0 && flag[rand / 2 - 1][col / 2 - 1] == 0) {
					int x, y;
					x = rand / 2 - 1;
					y = col / 2 - 1;
					fill (x, y, mine, marked, flag, r, c);
					br = rand / 2;
					bc = col / 2;
					modify (c, br, bc, bs);
					move (rand, col);
				}
				if (mine[rand / 2 - 1][col / 2 - 1] == 9 && flag[rand / 2 - 1][col / 2 - 1] == 0) {
					attron (A_BOLD | A_UNDERLINE);
					mvaddstr (1, c * 2 + 10, "Game over: press escape to enter the menu");
					attroff (A_BOLD | A_UNDERLINE);
					game = 0;
					date2 = 0;
					int i, j;
					for (i = 0 ; i < r ; i++)
						for (j = 0 ; j < c ; j++)
							if (mine[i][j] == 9) {
								color ((i + 1) * 2, (j + 1) * 2, 2, mine); 
								curs_set(0);
							}
				}
				move (rand, col);
			}
			move (rand, col);
			int k, l, okay = 1;
			for (k = 0 ; k < r ; k++)
				for (l = 0 ; l < c ; l++)
					if (mine[k][l] != 9)	
						if (marked[k][l] == 0)
							okay = 0;
			if (okay == 1) {
				attron (A_BOLD | A_UNDERLINE);
				mvaddstr (1, c * 2 + 10, "Congrats, you won: press escape to enter the menu");
				attroff (A_BOLD | A_UNDERLINE);
				game = 0;
				date2 = 0;
				curs_set (0);
			}	
			if (date2 == 1 && date == 0) {
				fin2 = time (NULL);
				minus2 = difftime(fin2, st2);
				mvprintw (15, c * 2 + 34, "%d", minus2);
				move (rand, col);
			}
		
			fin = time (NULL);
			minus = difftime(fin, st);
			mvprintw (12, c * 2 + 10, "Seconds from start-up: %d", minus);
			mvaddstr (15, c * 2 + 10, "Seconds from first hit: ");
			move (rand, col);
		}
		if (ecran != 2) //intrare in main menu
			switch (tolower(p)) {
				case KEY_ESCAPE:
					endwin();
					draw_menu();
					ecran = 2;
					rand = 2;
					col = 2;
					cursor = 1;
					ok = 0;
					break;
			}
		if (ecran == 2) { //main menu
			if (cursor == 3 && (tolower(p) == KEY_ENTER || tolower(p) == ' '))
				break;
			if (cursor == 2 && (tolower(p) == KEY_ENTER || tolower(p) == ' ')) {
				ecran = 3;
				cursor2 = 1;
				rand = 2;
				col = 2;
				endwin();
				draw_settings (r, c, b);
			}
			if (cursor == 1 && (tolower(p) == KEY_ENTER || tolower(p) == ' ')) {
				ecran = 1;
				br = (r + 1) / 2;
				bc = (c + 1) / 2;
				rand = even (r);
				col = even (c);
				ok = 1;
				game = 1;
				fade = 1;
				date = 1;
				date2 = 1;
				zero (mine);
				zero (marked);
				zero (flag);
				bs = b;
				endwin();
				draw_frame (r, c, b);
			}	
			if (tolower(p) == KEY_DOWN)
				if (cursor < 3) 
					down (&rand, &col, &cursor);
			if (tolower(p) == KEY_UP)
				if (cursor > 1)	
					up (&rand, &col, &cursor);
		}
		if (ecran == 3) { //settings
			if (cursor2 < 3 && tolower (p) == KEY_DOWN)
				down (&rand, &col, &cursor2);
			if (cursor2 > 1 && tolower (p) == KEY_UP)
				up (&rand, &col, &cursor2);
			if (tolower (p) == KEY_RIGHT) {
				if (cursor2 == 1) 
					if (r < 30)
						r = countup (r, 2);
				if (cursor2 == 2)
					if (c < 30)
						c = countup (c, 5);
				if (cursor2 == 3)
					if (b < r * c)
						b = countup (b, 8);
			}
			if (tolower (p) == KEY_LEFT) {
				if (cursor2 == 1)
					if (r > 1) {
						r = countdown (r, 2);
						while (b > r * c)
							b = countdown (b, 8);
					}
				if (cursor2 == 2)
					if (c > 1) {
						c = countdown (c, 5);
						while (b > r * c)
							b = countdown (b, 8);
					}
				if (cursor2 == 3)
					if (b > 1)
						b = countdown (b, 8);
			}
			bs = b;
		}
	}
	endwin();
	return 0;
}
