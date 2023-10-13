/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmangall <lmangall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 14:59:48 by lmangall          #+#    #+#             */
/*   Updated: 2023/10/13 12:30:00 by lmangall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/builtins.h"
#include "../include/executor.h"
#include "../include/free.h"
#include "../include/node.h"
#include "../include/pipe.h"
#include "../include/redirect.h"
#include "../include/shell.h"
#include "../lib/libft/src/libft.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

void	exec_pipe_redir(struct node_s *node)
{
	if (node->operator== PIPE)
		execute_pipe_command(node);
	else if (node->operator== NONE)
		do_simple_command(node);
	// exit(g_exit_status);
	else
		exec_redirection(node);
}

void	first_child(struct node_s *node, int pipe_fd[2])
{
	close(STDOUT_FILENO);
	dup(pipe_fd[1]);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	exec_pipe_redir(node);
}
#if 0
void	second_child(struct node_s *node, int pipe_fd[2])
{
	close(STDIN_FILENO);
	dup(pipe_fd[0]);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	exec_pipe_redir(node);
}
#endif
void second_child(struct node_s *node, int pipe_fd[2])
{
    close(STDIN_FILENO);
    dup(pipe_fd[0]);
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    exec_pipe_redir(node);
    exit(EXIT_SUCCESS); // Beende den zweiten Kindprozess nach dem EOF
}


#if 0
void	execute_pipe_command(struct node_s *node)
{
	pid_t child_pid;
	int pipe_fd[2];
	int status;

	node->operator= NONE;

	if (pipe(pipe_fd) == -1)
	{
		// panic(data, PIPE_ERR, EXIT_FAILURE);
		ft_putstr_fd("pipe error\n", 2);
	}
	child_pid = fork();
	if (child_pid == -1)
	{
		// panic(data, FORK_ERR, EXIT_FAILURE);
		ft_putstr_fd("pipe error\n", 2);
	}
	if (child_pid == 0)
		first_child(node, pipe_fd);

	second_child(node->next_sibling, pipe_fd);

	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(child_pid, &status, 0);
	// g_exit_status = status;
}
#endif
void execute_pipe_command(struct node_s *node)
{
    pid_t child_pid1, child_pid2;
    int pipe_fd[2];
    int status;

    node->operator = NONE;

    if (pipe(pipe_fd) == -1)
    {
        // Fehlerbehandlung für pipe-Fehler hier einfügen
        perror("pipe");//are we allowed to use this ?
        exit(EXIT_FAILURE);
    }

    child_pid1 = fork();
    if (child_pid1 == -1)
    {
        // Fehlerbehandlung für fork-Fehler hier einfügen
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid1 == 0)
    {
        // Kindprozess 1
        first_child(node, pipe_fd);
    }
    else
    {
        // Elternprozess
        child_pid2 = fork();
        if (child_pid2 == -1)
        {
            // Fehlerbehandlung für fork-Fehler hier einfügen
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (child_pid2 == 0)
        {
            // Kindprozess 2
            second_child(node->next_sibling, pipe_fd);
        }
        else
        {
            // Elternprozess wartet auf beide Kinder
            close(pipe_fd[0]); // Leseseite der Pipe wird im Elternprozess nicht benötigt
            close(pipe_fd[1]); // Schreibseite der Pipe wird im Elternprozess nicht benötigt
            waitpid(child_pid1, &status, 0);
            waitpid(child_pid2, &status, 0);
            // Fehlerbehandlung für waitpid-Fehler hier einfügen
        }
    }
}
