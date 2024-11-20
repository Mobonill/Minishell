/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_and_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: morgane <morgane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:17:47 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/20 14:34:45 by morgane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/lexer.h"

// void open_input(t_exec *exec, t_simple_cmds *parser)
// {
// 	t_simple_cmds *cur;

// 	cur = parser;
// 	exec->input = -1;
// 	while (cur != NULL)
// 	{
// 		if (cur->redirections->token == 2)
// 		{
// 			if (exec->input != -1)
// 				close(exec->input);
// 			printf("cur->redir->str = %s\n", cur->redirections->str);
// 			exec->input = open(cur->redirections->str, O_RDONLY);
// 			printf("exec->input = %d\n", exec->input);
// 			if (exec->input < 0)
// 			{
// 				ft_fprintf(2, "bash: %s: ", cur->redirections->str);
// 				perror("");
// 				free(exec->pid);
// 				exit(1);
// 			}
// 		}
// 		else if (cur->redirections->token == 3)
// 		{
// 			if (exec->input != -1)
// 				close(exec->input);
// 			exec->input = ft_handle_heredoc(cur);
// 			// restaurer les signaux;
// 		}
// 		cur = cur->next;
// 	}
// }

// int main()
// {
// 	t_exec exec;
// 	t_simple_cmds cmd1, cmd2, cmd3, cmd4;
// 	t_lexer redir1, redir2, redir3, redir4;

// 	// Simule les redirections
// 	redir1.token = 2; // <
// 	redir1.str = "infile1";
// 	redir2.token = 3; // <<
// 	redir2.str = "eof";
// 	redir3.token = 2; // <
// 	redir3.str = "infile2";
// 	redir4.token = 3;
// 	redir4.str = "end";

// 	// Simule les commandes chaînées
// 	cmd1.redirections = &redir1;
// 	cmd1.next = &cmd2;

// 	cmd2.redirections = &redir2;
// 	cmd2.next = &cmd3;

// 	cmd3.redirections = &redir3;
// 	cmd3.next = &cmd4;

// 	cmd4.redirections = &redir4;
// 	cmd4.next = NULL;

// 	// Initialise l'exécution
// 	exec.input = -1;
// 	exec.pid = malloc(sizeof(int) * 1);

// 	// Appelle open_input
// 	open_input(&exec, &cmd1);

// 	// Vérifie l'état final
// 	if (exec.input != -1)
// 	{
// 		printf("Final input descriptor: %d\n", exec.input);
// 		close(exec.input);
// 	}

// 	free(exec.pid);
// 	return 0;
// }

int	handle_redirections(t_simple_cmds *parser)
{
	t_lexer	*redir;
	int		fd;

	redir = parser->redirections;
	while (redir)
	{
		if (redir->token == IN)
		{
			fd = open(redir->str, O_RDONLY);
			if (fd < 0)
				return (perror(redir->str), -1);
			if (dup2(fd, STDIN_FILENO) < 0)
				return (perror("dup2 failed"), close(fd), -1);
			close(fd);
		}
		else if (redir->token == OUT)
		{
			fd = open(redir->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
				return (perror(redir->str), -1);
			if (dup2(fd, STDOUT_FILENO) < 0)
				return (perror("dup2 failed"), close(fd), -1);
			close(fd);
		}
		else if (redir->token == APPEND)
		{
			fd = open(redir->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
				return (perror(redir->str), -1);
			if (dup2(fd, STDOUT_FILENO) < 0)
				return (perror("dup2 failed"), close(fd), -1);
			close(fd);
		}
		else if (redir->token == HEREDOC)
		{
			if (handle_heredoc(redir->str, parser) < 0)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}

// void open_output(t_exec *exec, t_simple_cmds *parser)
// {
// 	t_parser *cur;

// 	cur = parser;
// 	while (cur != NULL)
// 	{
// 		if (cur->redirections->token == 4)
// 		{
// 			if (exec->output != -1)
// 				close(exec->output);
// 			exec->output = open(cur->redirections->str, O_CREAT | O_RDWR | O_TRUNC, 0666);
// 			if (exec->output < 0)
// 			{
// 				ft_fprintf(2, "bash: %s: %s", cur->redirections->str, perror);
// 				free_pipes(exec);
// 				free(exec->pid);
// 				exit(1);
// 			}
// 		}
// 		cur = cur->next;
// 	}
// }
// void free_pipes(t_exec *exec)
// {
// 	int i;

// 	i = 0;
// 	if (exec->fd)
// 	{
// 		i = 0;
// 		while (i <= exec->num_pipes + 2) // pour les redirections
// 		{
// 			free(exec->fd[i]);
// 			i++;
// 		}
// 		free(exec->fd);
// 	}
// }
// void free_all(t_exec *exec)
// {
// 	free_pipes(exec);
// 	free(exec->pid);
// }

// void free_cmd_argv(t_simple_cmds *parser)
// {
// 	int i;

// 	i = 0;
// 	while (parser->cmd[i])
// 	{
// 		free(parser->cmd[i]);
// 		i++;
// 	}
// 	free(parser->cmd);
// }
