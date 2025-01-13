#include <unistd.h>
#include <fcntl.h>
#include <termio.h>
#include <stdio.h>

#include "src/fonction_tete.h"

/*		FONCTION DONNER PAR LES INSTRUCTIONS       */
int kbhit()
{
	// la fonction retourne :
	// 1 si un caractere est present
	// 0 si pas de caractere present
	int un_caractere = 0;
	struct termios oldt, newt;
	int ch;
	int oldf;
	// mettre le terminal en mode non bloquant
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	ch = getchar();
	// restaurer le mode du terminal
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if (ch != EOF)
	{
		ungetc(ch, stdin);
		un_caractere = 1;
	}
	return un_caractere;
}
void goto_x_y(int x, int y)
{
	/**
	 * @brief Fonction qui déplace le curseur dans la console
	 * @param x position x.
	 * @param y position y.
	 */
	printf("\033[%d;%df", y, x);
}
void disable_echo()
{
	struct termios tty;
	// Obtenir les attributs du terminal
	if (tcgetattr(STDIN_FILENO, &tty) == -1)
	{
		perror("tcgetattr");
		exit(EXIT_FAILURE);
	}
	// Desactiver le flag ECHO
	tty.c_lflag &= ~ECHO;
	// Appliquer les nouvelles configurations
	if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1)
	{
		perror("tcsetattr");
		exit(EXIT_FAILURE);
	}
}
void enable_echo()
{
	struct termios tty;
	// Obtenir les attributs du terminal
	if (tcgetattr(STDIN_FILENO, &tty) == -1)
	{
		perror("tcgetattr");
		exit(EXIT_FAILURE);
	}
	// Reactiver le flag ECHO
	tty.c_lflag |= ECHO;
	// Appliquer les nouvelles configurations
	if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1)
	{
		perror("tcsetattr");
		exit(EXIT_FAILURE);
	}
}