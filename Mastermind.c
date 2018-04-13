#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Mastermind.h"

void		my_combi(t_struct *data)
{
  int		i;

  i = 0;
  srand(time(NULL));
  while (i < 4)
    {
      data->combi[i] = rand() % 6 + 1 + '0';
      i++;
    }
  data->combi[i] = '\0';
}

void		init_plateau(t_struct *data)
{
  int		i = 0;
  int		n = 0;

  data->plateau = malloc(sizeof(int *) * data->size);
  while (i < data->size)
    {
      data->plateau[i] = malloc(sizeof(int) * (5));
      data->plateau[i][n] = i + 1;
      i++;
    }
  data->plateau[i] = NULL;
}

int		intlen(int nb)
{
  int		i = 0;

  while (nb > 0)
    {
      nb /= 10;
      i++;
    }
  return (i);
}

void	print_line(t_struct *data, char space[], int i)
{
  int	n = 1;

  printf("%s%d| ", space, data->plateau[i][0]);
  while (n <= 4)
    {
      if (data->plateau[i][n] == 1)
        printf("\033[0;31m%d\033[00m", data->plateau[i][n]);
      else if (data->plateau[i][n] == 2)
	printf("\033[0;33m%d\033[00m", data->plateau[i][n]);
      else if (data->plateau[i][n] == 3)
	printf("\033[0;32m%d\033[00m", data->plateau[i][n]);
      else if (data->plateau[i][n] == 4)
	printf("\033[0;34m%d\033[00m", data->plateau[i][n]);
      else if (data->plateau[i][n] == 5)
	printf("\033[0;35m%d\033[00m", data->plateau[i][n]);
      else if (data->plateau[i][n] == 6)
	printf("%d", data->plateau[i][n]);
      printf(" | ");
      fflush(stdout);
      n++;
    }
  printf("%s\n", data->croixrond[i]);
}

void		print_tab(t_struct *data)
{
  int		n;
  int		i;
  char		space[data->size];

  i = 0;
  n = 0;
  system("clear");
  printf("    -----MASTERMIND-----\n\n");
  while (data->plateau[i])
    {
      while (n < 6)
	space[n++] = ' ';
      if (intlen(data->plateau[i][0]) == 2)
	space[4] = '\0';
      else
	space[5] = '\0';
      if (data->plateau[i][1] == 0)
	printf("%s%d|   |   |   |   |\n", space, data->plateau[i][0]);
      else
	print_line(data, space, i);
      i++;
    }
}

int	check_buf(char *buf)
{
  int	i;

  i = 0;
  while (buf[i] != '\0' && buf[i] != '\n')
    {
      if (buf[i] >= '1' && buf[i] <= '6')
	i++;
      else
	return (-1);
    }
  if (i == 4)
    return (1);
  return (-1);
}

int	check_size(char *buf)
{
  int	i;

  i = 0;
  while (buf[i] != '\0' && buf[i] != '\n')
    {
      if (buf[i] >= '0' && buf[i] <= '9')
	i++;
      else
	return (-1);
    }
  return (1);
}

void		remp_tab(t_struct *data, char *buf, int i)
{
  data->plateau[i][1] = buf[0] - '0';
  data->plateau[i][2] = buf[1] - '0';
  data->plateau[i][3] = buf[2] - '0';
  data->plateau[i][4] = buf[3] - '0';
}

void		checkcrosse(t_struct *data, int i)
{
  int		n = 1;
  int		w = 0;
  int		p = 1;
  int		statut = 0;

  while (data->croixrond[i][w])
    w++;
  while (n < 5)
    {
      statut = 0;
      p = 1;
      if (data->plateau[i][n] == data->combi[n - 1] - '0')
	n++;
      else
	{
	  while (statut == 0 && p < 5)
	    {
	      if (p == n)
		p++;
	      if (data->plateau[i][n] == data->combi[p - 1] - '0')
		{
		  data->croixrond[i][w++] = 'x';
		  statut = 1;
		}
	      p++;
	    }
	  n++;
	}
    }
  data->croixrond[i][5] = '\0';
}

int		checkcircle(t_struct *data, int i)
{
  int		n = 1;
  int		w = 0;

  while (n < 5)
    {
      if (data->plateau[i][n] == data->combi[n - 1] - '0')
	data->croixrond[i][w++] = 'o';
      n++;
    }
  data->croixrond[i][5] = '\0';
  return (w);
}

void		my_game(t_struct *data)
{
  char		buf[10];
  int		i = 0;
  int		len;
  int		statut = 0;

  system("clear");
  while (i < data->size && statut == 0)
    {
      print_tab(data);
      printf("\n");
      printf("Entrez votre proposition : ");
      fflush(stdout);
      buf[0] ='\0';
      len = read(0, buf, 100);
      if (len == 0)
	exit(EXIT_SUCCESS);
      buf[len] = '\0';
      while (check_buf(buf) == -1)
	{
	  printf("Entrez votre proposition : ");
	  fflush(stdout);
	  buf[0] ='\0';
	  len = read(0, buf, 100);
	  if (len == 0)
	    exit(EXIT_SUCCESS);
	  buf[len] = '\0';
	}
      remp_tab(data, buf, i);
      if (checkcircle(data, i) == 4)
	statut = 1;
      checkcrosse(data, i);
      i++;
    }
  if (statut == 1)
    printf("Félicitation, vous avez trouvé la bonne combinaison\n");
  else
    {
      print_tab(data);
      printf("PERDU\nLa combinaison était %s\n", data->combi);
    }
}

void		init_croixrond(t_struct *data)
{
  int		i = 0;

  data->croixrond = malloc(sizeof(char *) * data->size);
  while (i < data->size)
    {
      data->croixrond[i] = malloc(sizeof(char) * (5));
      data->croixrond[i][0] = '\0';
      i++;
    }
  data->croixrond[i] = NULL;
}

void	print_logo(void)
{
  printf("___  ___  ___   _____ _____ ______________  ________ _   _______ \n");
  printf("|  \\/  | / _ \\ /  ___|_   _|  ___| ___ \\  \\/  |_   _| \\ | |  _  \\ \n");
  printf("| .  . |/ /_\\ \\\\ `--.  | | | |__ | |_/ / .  . | | | |  \\| | | | | \n");
  printf("| |\\/| ||  _  | `--. \\ | | |  __||    /| |\\/| | | | | . ` | | | | \n");
  printf("| |  | || | | |/\\__/ / | | | |___| |\\ \\| |  | |_| |_| |\\  | |/ /  \n");
  printf("\\_|  |_/\\_| |_/\\____/  \\_/ \\____/\\_| \\_\\_|  |_/\\___/\\_| \\_/___/   \n");
}

void		my_menu(t_struct *data)
{
  char		*buff;
  int		len;

  buff = malloc(sizeof(char) * 100);
  system("clear");
  print_logo();
  printf("\n       -MENU-\n\n");
  printf("Enter Your Name: ");
  fflush(stdout);
  len = read(0, buff, 39);
  if (len <= 0)
    exit(EXIT_SUCCESS);
  buff[len] = '\0';
  data->name = malloc(sizeof(char) * (strlen(buff) + 1));
  strcpy(data->name, buff);
  free(buff);
  buff = malloc(sizeof(char) * 100);
  printf("\nCoup maximum (defaut = 10, max 99): ");
  fflush(stdout);
  len = read(0, buff, 40);
  buff[len] = '\0';
  if (len == 0 || len < 2)
    data->size = 10;
  else if (len > 3)
    data->size = 10;
  else if (len == 3 || len == 2)
    if (check_size(buff) != -1)
      data->size = atoi(buff);
    else
      data->size = 10;
  init_plateau(data);
  init_croixrond(data);
  my_game(data);
}


int		main()
{
  t_struct	data;

  my_combi(&data);
  my_menu(&data);

}
