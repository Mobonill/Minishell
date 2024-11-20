/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_and_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:17:47 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/20 16:34:58 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/lexer.h"

int	handle_redirections(t_exec *exec, t_simple_cmds *parser)
{
	t_lexer	*redir;

	redir = parser->redirections;
	while (redir)
	{
		if (redir->token == IN)
		{
			exec->input = open(redir->str, O_RDONLY);
			if (exec->input < 0)
				return (perror(redir->str), -1);
			if (dup2(exec->input, STDIN_FILENO) < 0)
				return (perror("dup2 failed"), close(exec->input), -1);
			close(exec->input);
		}
		else if (redir->token == OUT)
		{
			exec->output = open(redir->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (exec->output < 0)
				return (perror(redir->str), -1);
			if (dup2(exec->output, STDOUT_FILENO) < 0)
				return (perror("dup2 failed"), close(exec->output), -1);
			close(exec->output);
		}
		else if (redir->token == APPEND)
		{
			exec->output = open(redir->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (exec->output < 0)
				return (perror(redir->str), -1);
			if (dup2(exec->output, STDOUT_FILENO) < 0)
				return (perror("dup2 failed"), close(exec->output), -1);
			close(exec->output);
		}
		else if (redir->token == HEREDOC)
		{
			if (handle_heredoc(parser) < 0)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}

int	ft_handle_heredoc(t_simple_cmds *parser)
{
	char	*line;
	int		tmp_fd;

	line = NULL;
	tmp_fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (tmp_fd < 0)
	{
		perror("heredoc");
		exit(1);
	}
	while (1)
	{
		line = readline(">");
		heredoc_signals();
		if (!line || ft_strcmp(line, parser->redirections->str) == 0)
			break;
		ft_fprintf(tmp_fd, "%s\n", line);
		free(line);
	}
	free(line);
	close(tmp_fd);
	tmp_fd = open(".heredoc_tmp", O_RDONLY);
	return (tmp_fd);
}



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
//
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
//
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
// }