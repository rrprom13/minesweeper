#ifndef __MINESWEEPER_H__
#define __MINESWEEPER_H__

#include <curses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>

#define NO_KEY_PRESSED (-1)
#define even(a) ((a) % 2 == (0) ? (a) : (a + 1))

int get_key_pressed()
{
	int ch = getch();
	if (ch != ERR) {
		return ch;
	}
	return NO_KEY_PRESSED;
}

int draw_frame(int a, int b, int c)
{	
	WINDOW* wnd = initscr();
	int nrows, ncols;
	getmaxyx(wnd, nrows, ncols);
	curs_set(1);
	clear();
	noecho();
	cbreak();
	keypad(wnd, TRUE);
	nodelay(wnd, TRUE);
	set_escdelay (25);
	box (wnd, ACS_VLINE, ACS_HLINE);
	int i, j, row = 1, col = 1;
	move (row, col);
	for (i = 0 ; i <= a * 2; i++) { 
		for (j = 0 ; j <= b ; j++) { 
			addch (ACS_VLINE);
			if (i % 2 == 0)
				if (j < b)
					addch (ACS_HLINE);
			col = col + 2;
			move (row, col);
		}
		if (i < a * 2) {
			col = 1;
			row++;
			move (row, col);
		}
	}		
	mvprintw (21, b * 2 + 10, "Current position: %d  %d", (a + 1) / 2, (b + 1) / 2);
	mvprintw (18, b * 2 + 10, "Mines left: %d", c);
	move (row = even (a), col = even (b));
	refresh ();
	return col;
}

void draw_menu ()
{
	int nrows, ncols;
	WINDOW* wnd2 = initscr();
	getmaxyx(wnd2, nrows, ncols);
	curs_set(0);
	clear();
	noecho();
	cbreak();
	keypad(wnd2, TRUE);
	nodelay(wnd2, TRUE);
	set_escdelay (25);
	box (wnd2, ACS_VLINE, ACS_HLINE);
	mvaddstr (2, 5, "New Game");
	mvaddstr (5, 5, "Settings");
	mvaddstr (8, 5, "Quit");
	move (2, 2);
	addch ('*');
	refresh();
}

void draw_settings (int a, int b, int c)
{
	int nrows, ncols;
	WINDOW* wnd3 = initscr();
	getmaxyx(wnd3, nrows, ncols);
	curs_set(0);
	clear();
	noecho();
	cbreak();
	keypad(wnd3, TRUE);
	nodelay(wnd3, TRUE);
	set_escdelay (25);
	box (wnd3, ACS_VLINE, ACS_HLINE);
	mvaddstr (2, 5, "Rows: ");
	mvprintw (2, 15, "%d", a);
	mvaddstr (5, 5, "Columns: ");
	mvprintw (5, 15, "%d", b);
	mvaddstr (8, 5, "Bombs: ");
	mvprintw (8, 15, "%d", c);
	move (2, 2);
	addch ('*');
	refresh();
}

void dl (int a)
{
	int i = 0;
	while (i < a) {
		delch ();
		i++;
	}
	i = 0;
	while (i < a) {
		insch (' ');
		i++;
	}
}

void up (int *a, int *b, int *c)
{
	move ((*a), (*b));
	delch ();
	insch (' ');
	(*a) = (*a) - 3;
	move ((*a), (*b));
	addch ('*');
	(*c)--;
}

int steag (int a, int b)
{
	move (a,b); 
	char ch;
	ch = inch ();
	if (ch == 'V') {
		delch ();
		insch (' ');
		return 1;
	}
	addch ('V');
	return 0;
}

void down (int *a, int *b, int *c)
{
	move ((*a), (*b));
	delch ();
	insch (' ');
	(*a) = (*a) + 3;
	move ((*a), (*b));
	addch ('*');
	(*c)++;
	refresh();
}

void check (int *a)
{
	if ((*a) != 9) 
		(*a)++;
}

int countup (int a, int row)
{
	a++;
	mvprintw (row, 15, "%d", a);
	refresh();
	return a;
}

int countdown (int a, int row)
{
	a--;
	move (row, 15);
	dl (3);
	mvprintw (row, 15, "%d", a);
	refresh();
	return a;
}	

void color (int a, int b, int c, int v[][30])
{	
	start_color();
	if (c == 1)
		init_pair (c, COLOR_WHITE, COLOR_GREEN);
	else
		init_pair (c, COLOR_WHITE, COLOR_RED);
	move (a, b);
	delch ();  
	attron (COLOR_PAIR(c));
	int x, y;
	x = (a - 1) / 2;
	y = (b - 1) / 2;
	insch (' ');
	if (c == 1)
		if (v[x][y] != 0)
			mvprintw (a, b, "%d", v[x][y]);
	attroff (COLOR_PAIR(c));
}

void mines (int a, int v[][30], int x, int y, int r, int c)
{
	int i, s, t;
	srand (time (NULL));
	if (r != 1 || c != 1)
		for (i = 0 ; i < a ; i++) {
			do { 
				s = rand () % r;
				t = rand () % c;
			} while ((s == x && t == y) || v[s][t] == 9);
			v[s][t] = 9;
		}	
	if (r == 1 && c > 1)
		for (t = 0 ; t < c ; t++)
			if (v[0][t] == 9) {
				if (t > 0)
					check(&v[0][t - 1]);
				if (t < c - 1)
					check(&v[0][t + 1]);
			}
	if (r == 2 && c == 1)
		if (v[1][0] == 9) 
			v[0][0]++;
	if (r == 2 && c > 1)
		for (t = 0 ; t < c ; t++) {
			if (v[0][t] == 9) {
				if (t > 0)
					check(&v[0][t - 1]);
				if (t < c - 1)
					check(&v[0][t + 1]);
				if (t > 0)
					check(&v[1][t - 1]);
				check(&v[1][t]);
				if (t < c - 1)
					check(&v[1][t + 1]);
			}
			if (v[1][t] == 9) {
				if (t > 0)
					check(&v[1][t - 1]);
				if (t < c - 1)
					check(&v[1][t + 1]);
				if (t > 0)
					check(&v[0][t - 1]);
				check(&v[0][t]);
				if (t < c - 1)
					check(&v[0][t + 1]);
			}
		}
	if (r > 2 && c == 1)
		for (s = 0 ; s < r ; s++)
			if (v[s][0] == 9) {
				if (s > 1)
					check(&v[s - 1][0]);
				if (s < r - 1)
					check(&v[s + 1][0]);
			}
	if (r > 2 && c == 2)
		for (s = 0 ; s < r ; s++) {
			if (v[s][0] == 9) {
				if (s > 1)
					check(&v[s - 1][0]);
				if (s < r - 1)
					check(&v[s + 1][0]);
				if (s > 1)
					check(&v[s - 1][1]);
				check(&v[s][1]);
				if (s < r - 1)
					check(&v[s + 1][1]);
			}
			if (v[s][1] == 9) {
				if (s > 1)
					check(&v[s - 1][1]);	
				if (s < r - 1)
					check(&v[s + 1][1]);	
				if (s > 1)
					check(&v[s - 1][0]);	
				check(&v[s][0]);	
				if (s < r - 1)
					check(&v[s + 1][0]);	
			}
		}
	if (r > 2 && c > 2) {
		if (v[0][0] == 9) {
			check (&v[0][1]);
			check (&v[1][0]);
			check (&v[1][1]);
		}
		if (v[0][c - 1] == 9) {
			check (&v[0][c - 2]);
			check (&v[1][c - 1]);
			check (&v[1][c - 2]);
		}
		if (v[r - 1][0] == 9) {
			check (&v[r - 2][0]);
			check (&v[r - 2][1]);
			check (&v[r - 1][1]);
		}
		if (v[r - 1][c - 1] == 9) {
			check (&v[r - 2][c - 2]);
			check (&v[r - 2][c - 1]);
			check (&v[r - 1][c - 2]);
		}
		for (t = 1 ; t < c - 1 ; t++)
			if (v[0][t] == 9) {
				check(&v[0][t - 1]);
				check(&v[0][t + 1]);
				check(&v[1][t - 1]);
				check(&v[1][t]);
				check(&v[1][t + 1]);
			}
		for (s = 1 ; s < r - 1 ; s++)
			if (v[s][c - 1] == 9) {
				check(&v[s - 1][c - 1]);
				check(&v[s + 1][c - 1]);
				check(&v[s - 1][c - 2]);
				check(&v[s][c - 2]);
				check(&v[s + 1][c - 2]);
			}
		for (t = c - 2 ; t > 0 ; t--)
			if (v[r - 1][t] == 9) {
				check(&v[r - 1][t - 1]);
				check(&v[r - 1][t + 1]);
				check(&v[r - 2][t + 1]);
				check(&v[r - 2][t]);
				check(&v[r - 2][t - 1]);
			}
		for (s = r - 2 ; s > 0 ; s--)
			if (v[s][0] == 9) {
				check(&v[s - 1][0]);
				check(&v[s + 1][0]);
				check(&v[s - 1][1]);
				check(&v[s][1]);
				check(&v[s + 1][1]);
			}
		for (s = 1 ; s < r - 1 ; s++)
			for (t = 1 ; t < c - 1 ; t++)
				if (v[s][t] == 9) {
					check(&v[s - 1][t - 1]);
					check(&v[s - 1][t]);
					check(&v[s - 1][t + 1]);
					check(&v[s][t - 1]);
					check(&v[s][t + 1]);
					check(&v[s + 1][t - 1]);
					check(&v[s + 1][t]);
					check(&v[s + 1][t + 1]);
				}
	}
}

void modify (int a, int b, int c, int d)
{
	move (21, a * 2 + 28);
	dl (3);
	move (21, a * 2 + 31);
	dl (3);
	move (18, a * 2 + 22);
	dl (4);
	mvprintw (21, a * 2 + 28, "%d  %d", b, c);
	mvprintw (18, a * 2 + 22, "%d", d);
}

void zero (int v[][30])
{
	int i, j;
	for (i = 0 ; i < 30 ; i++)
		for (j = 0 ; j < 30 ; j++)
			v[i][j] = 0;
}

int fill (int x, int y, int v[][30], int w[][30], int z[][30], int r, int c)// v = mine, w = marked, z = flag
{
	if (v[x][y] == 0 && z[x][y] == 0) { 
		if (w[x][y] == 0)
			color ((x + 1 ) * 2, (y + 1) * 2, 1, v);
		w[x][y] = 1;
		if (y > 0)
			if (v[x][y-1] > 0 && v[x][y-1] < 9 && w[x][y-1] == 0 && z[x][y-1] == 0) {
				w[x][y-1] = 1;
				color ((x + 1) * 2, y * 2, 1, v);
			}
		if (y < c - 1)
			if (v[x][y+1] > 0 && v[x][y+1] < 9 && w[x][y+1] == 0 && z[x][y+1] == 0) {
				w[x][y+1] = 1;
				color ((x + 1) * 2, (y + 2) * 2, 1, v);
			}
		if (x > 0)
			if (v[x-1][y] > 0 && v[x-1][y] < 9 && w[x-1][y] == 0 && z[x-1][y] == 0) {
				w[x-1][y] = 1;
				color (x * 2, (y + 1) * 2, 1, v);
			}
		if (x < r - 1)
			if (v[x+1][y] > 0 && v[x+1][y] < 9 && w[x+1][y] == 0 && z[x+1][y] == 0) {
				w[x+1][y] = 1;
				color ((x + 2) * 2, (y + 1) * 2, 1, v);
			} 
		//afisare cifre pe cele 4 directii 
		if (x > 0 && y > 0)
			if (v[x-1][y-1] > 0 && v[x-1][y-1] < 9 && w[x-1][y-1] == 0 && z[x-1][y-1] == 0) {
				w[x-1][y-1] = 1;
				color (x * 2, y * 2, 1, v);
			}
		if (x > 0 && y < c - 1)
			if (v[x-1][y+1] > 0 && v[x-1][y+1] < 9 && w[x-1][y+1] == 0 && z[x-1][y+1] == 0) {
				w[x-1][y+1] = 1;
				color (x * 2, (y + 2) * 2, 1, v);
			}
		if (x < r - 1 && y > 0)
			if (v[x+1][y-1] > 0 && v[x+1][y-1] < 9 && w[x+1][y-1] == 0 && z[x+1][y-1] == 0) {
				w[x+1][y-1] = 1;
				color ((x + 2) * 2, y * 2, 1, v);
			}
		if (x < r - 1 && y < c - 1)
			if (v[x+1][y+1] > 0 && v[x+1][y+1] < 9 && w[x+1][y+1] == 0 && z[x+1][y+1] == 0) {
				w[x+1][y+1] = 1;
				color ((x + 2) * 2, (y + 2) * 2, 1, v);
			}
		//afisare cifre pe diagonala
		if (y > 0)	
			if (v[x][y-1] == 0 && w[x][y-1] == 0 && z[x][y-1] == 0) 
				fill (x, y - 1, v, w, z, r, c);
		if (y < c - 1)
			if (v[x][y+1] == 0 && w[x][y+1] == 0 && z[x][y+1] == 0) 
				fill (x, y + 1, v, w, z, r, c);
		if (x > 0)
			if (v[x-1][y] == 0 && w[x-1][y] == 0 && z[x-1][y] == 0) 
				fill (x - 1, y, v, w, z, r, c);
		if (x < r - 1)
			if (v[x+1][y] == 0 && w[x+1][y] == 0 && z[x+1][y] == 0)
				fill (x + 1, y, v, w, z, r, c);
		//apelare recursiva if casuta West, East, North, South este egala cu 0 (fara mine, mine vecine, flag)
		if (x > 0 && y > 0)
			if (v[x-1][y-1] == 0 && w[x-1][y-1] == 0 && z[x-1][y-1] == 0) 
				fill (x - 1, y - 1, v, w, z, r, c);
		if (x > 0 && y < c - 1)
			if (v[x-1][y+1] == 0 && w[x-1][y+1] == 0 && z[x-1][y+1] == 0) 
				fill (x - 1, y + 1, v, w, z, r, c);
		if (x < r - 1 && y > 0)
			if (v[x+1][y-1] == 0 && w[x+1][y-1] == 0 && z[x+1][y-1] == 0) 
				fill (x + 1, y - 1, v, w, z, r, c);
		if (x < r - 1 && y < c - 1)
			if (v[x+1][y+1] == 0 && w[x+1][y+1] == 0 && z[x+1][y+1] == 0) 
				fill (x + 1, y + 1, v, w, z, r, c);
		//apelare recursiva if casuta diagonala este egala cu 0 (fara mine, mine vecine, flag)
		return 1;
	}
}

void bonus1 (int *a, int *b, int r, int c, int v[][30], int w[][30], int z[][30])// v = mine, w = marked, z = flag
{
	int x, y, flagged = 0, ok = 1;
	x = (*a) / 2 - 1;
	y = (*b) / 2 - 1;
	if (x > 0) {
		if (z[x-1][y] == 1)
			flagged++;
		if (y > 0) 
			if (z[x-1][y-1] == 1)
				flagged++;
		if (y < c - 1) 
			if (z[x-1][y+1] == 1)
				flagged++;
	} 
	if (y > 0) 
		if (z[x][y-1] == 1)
			flagged++;
	if (y < c - 1) 		
		if (z[x][y+1] == 1)
			flagged++;
	if (x < r - 1) {
		if (z[x+1][y] == 1)
			flagged++;
		if (y > 0) 
			if (z[x+1][y-1] == 1)
				flagged++;
		if (y < c - 1) 
			if (z[x+1][y+1] == 1)
				flagged++;
	}
//numararea casutelor vecine marcate cu flag
	if (v[x][y] == flagged) {
		if (x > 0) {
			if (w[x-1][y] == 0 && z[x-1][y] == 0) {
				if (v[x-1][y] != 9) {
					color (x * 2, (y + 1) * 2, 1, v);
					w[x-1][y] = 1;
					fill (x - 1, y, v, w, z, r, c);
				}
				else {
					(*a) = x * 2;
					(*b) = (y + 1) * 2;
					ok = 0;
				}  
			}
			if (y > 0 && ok == 1) 
				if (w[x-1][y-1] == 0 && z[x-1][y-1] == 0) {
					if (v[x-1][y-1] != 9) {
						color (x * 2, y * 2, 1, v);
						w[x-1][y-1] = 1;
						fill (x - 1, y - 1, v, w, z, r, c);
					}
					else {
						(*a) = x * 2;
						(*b) = y * 2;
						ok = 0;
					}  
				}
			if (y < c - 1 && ok == 1) 
				if (w[x-1][y+1] == 0 && z[x-1][y+1] == 0) {
					if (v[x-1][y+1] != 9) {
						color (x * 2, (y + 2) * 2, 1, v);
						w[x-1][y+1] = 1;
						fill (x - 1, y + 1, v, w, z, r, c);
					}
					else {
						(*a) = x * 2;
						(*b) = (y + 2) * 2;
						ok = 0;
					}  
				}
		} 
		if (y > 0 && ok == 1) 
			if (w[x][y-1] == 0 && z[x][y-1] == 0) {
				if (v[x][y-1] != 9) {
					color ((x + 1) * 2, y * 2, 1, v);
					w[x][y-1] = 1;
					fill (x, y - 1, v, w, z, r, c);
				}
				else {
					(*a) = (x + 1) * 2;
					(*b) = y * 2;
					ok = 0;
				} 
			} 
		if (y < c - 1 && ok == 1) 		
			if (w[x][y+1] == 0 && z[x][y+1] == 0) {
				if (v[x][y+1] != 9) {
					color ((x + 1) * 2, (y + 2) * 2, 1, v);
					w[x][y+1] = 1;
					fill (x, y + 1, v, w, z, r, c);
				}
				else {
					(*a) = (x + 1) * 2;
					(*b) = (y + 2) * 2;
					ok = 0;
				}  
			}
		if (x < r - 1 && ok == 1) {
			if (w[x+1][y] == 0 && z[x+1][y] == 0) {
				if (v[x+1][y] != 9) {
					color ((x + 2) * 2, (y + 1) * 2, 1, v);
					w[x+1][y] = 1;
					fill (x + 1, y, v, w, z, r, c);
				}
				else {
					(*a) = (x + 2) * 2;
					(*b) = (y + 1) * 2;
					ok = 0;
				}  
			}
			if (y > 0 && ok == 1) 
				if (w[x+1][y-1] == 0 && z[x+1][y-1] == 0) {
					if (v[x+1][y-1] != 9) {
						color ((x + 2) * 2, y * 2, 1, v);
						w[x+1][y-1] = 1;
						fill (x + 1, y - 1, v, w, z, r, c);
					}
					else {
						(*a) = (x + 2) * 2;
						(*b) = y * 2;
						ok = 0;
					}  
				}
			if (y < c - 1 && ok == 1) 
				if (w[x+1][y+1] == 0 && z[x+1][y+1] == 0) {
					if (v[x+1][y+1] != 9) {
						color ((x + 2) * 2, (y + 2) * 2, 1, v);
						w[x+1][y+1] = 1;
						fill (x + 1, y + 1, v, w, z, r, c);
					}
					else {
						(*a) = (x + 2) * 2;
						(*b) = (y + 2) * 2;
						ok = 0;
					}  
				}
		}
	}
//lovirea casutelor vecine daca nr de casute vecine cu flag = nr casuta curenta
}

void bonus2 (int a, int b, int r, int c, int v[][30], int w[][30], int z[][30], int *bs)// v = mine, w = marked, z = flag
{
	int x, y, vecini = 0, mina = 0, marcat = 0;
	x = a / 2 - 1;
	y = b / 2 - 1;
	if (x > 0) {
		if (v[x-1][y] > 0)
			vecini++;
		if (v[x-1][y] == 9)
			mina++;
		if (v[x-1][y] > 0 && v[x-1][y] < 9 && w[x-1][y] == 1)
			marcat++;
		if (y > 0) {
			if (v[x-1][y-1] > 0)
				vecini++;
			if (v[x-1][y-1] == 9)
				mina++;
			if (v[x-1][y-1] > 0 && v[x-1][y-1] < 9 && w[x-1][y-1] == 1)
				marcat++;
		}
		if (y < c - 1) {
			if (v[x-1][y+1] > 0)
				vecini++;
			if (v[x-1][y+1] == 9)
				mina++;
			if (v[x-1][y+1] > 0 && v[x-1][y+1] < 9 && w[x-1][y+1] == 1)
				marcat++;
		}
	} 
	if (y > 0) {
		if (v[x][y-1] > 0)
			vecini++;
		if (v[x][y-1] == 9)
			mina++;
		if (v[x][y-1] > 0 && v[x][y-1] < 9 && w[x][y-1] == 1)
			marcat++;
	}
	if (y < c - 1) {
		if (v[x][y+1] > 0)
			vecini++;
		if (v[x][y+1] == 9)
			mina++;
		if (v[x][y+1] > 0 && v[x][y+1] < 9 && w[x][y+1] == 1)
			marcat++;
	}
	if (x < r - 1) {
		if (v[x+1][y] > 0)
			vecini++;
		if (v[x+1][y] == 9)
			mina++;
		if (v[x+1][y] > 0 && v[x+1][y] < 9 && w[x+1][y] == 1)
			marcat++;
		if (y > 0) {
			if (v[x+1][y-1] > 0)
				vecini++;
			if (v[x+1][y-1] == 9)
				mina++;
			if (v[x+1][y-1] > 0 && v[x+1][y-1] < 9 && w[x+1][y-1] == 1)
				marcat++;
		}
		if (y < c - 1) {
			if (v[x+1][y+1] > 0)
				vecini++;
			if (v[x+1][y+1] == 9)
				mina++;
			if (v[x+1][y+1] > 0 && v[x+1][y+1] < 9 && w[x+1][y+1] == 1)
				marcat++;
		}
//numarare nr total casute vecine, casute deja lovite (fara bombe sau 0) si casute minate
	}
	if (vecini == mina + marcat) {
		if (x > 0) {
			if (v[x-1][y] == 9 && z[x-1][y] == 0) {
				z[x-1][y] = 1;
				steag (x * 2, (y + 1) * 2); 
				(*bs)--;
			}
			if (y > 0) 
				if (v[x-1][y-1] == 9 && z[x-1][y-1] == 0) {
					z[x-1][y-1] = 1;
					steag (x * 2, y * 2);
					(*bs)--;
				}
			if (y < c - 1) 
				if (v[x-1][y+1] == 9 && z[x-1][y+1] == 0) {
					z[x-1][y+1] = 1;
					steag (x * 2, (y + 2) * 2);
					(*bs)--;
				}
		}
		if (y > 0) 
			if (v[x][y-1] == 9 && z[x][y-1] == 0) {
				z[x][y-1] = 1;
				steag ((x + 1) * 2, y * 2);
				(*bs)--;
			}
		if (y < c - 1)
			if (v[x][y+1] == 9 && z[x][y+1] == 0) {
				z[x][y+1] = 1;
				steag ((x + 1) * 2, (y + 2) * 2);
				(*bs)--;
			}
		if (x < r - 1) {
			if (v[x+1][y] == 9 && z[x+1][y] == 0) {
				z[x+1][y] = 1;
				steag ((x + 2) * 2, (y + 1) * 2);
				(*bs)--;
			}
			if (y > 0) 
				if (v[x+1][y-1] == 9 && z[x+1][y-1] == 0) {
					z[x+1][y-1] = 1;
					steag ((x + 2) * 2, y * 2);
					(*bs)--;
				}
			if (y < c - 1) 
				if (v[x+1][y+1] == 9 && z[x+1][y+1] == 0) {
					z[x+1][y+1] = 1;
					steag ((x + 2) * 2, (y + 2) * 2);
					(*bs)--;
				}
		}
	}
//marcarea casutelor vecine cu flag 
}	

#endif
