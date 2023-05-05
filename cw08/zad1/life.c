#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

void ign_handler(int signo, siginfo_t *info, void *context) {}

int main()
{
	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode

	char *foreground = create_grid();
	char *background = create_grid();
	char *tmp;

	init_grid(foreground);

	// ustawianie maski dla sygnału SIGUSR1
	struct sigaction action;
	sigemptyset(&action.sa_mask);
	action.sa_sigaction = ign_handler;
	sigaction(SIGUSR1, &action, NULL);

	run_threads(foreground, background);

	while (true)
	{
		draw_grid(foreground);
		usleep(500 * 1000);

		// Step simulation
		update_grid();
		tmp = foreground;
		foreground = background;
		background = tmp;
	}

	endwin(); // End curses mode
	destroy_grid(foreground);
	destroy_grid(background);

	return 0;
}
