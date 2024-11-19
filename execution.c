/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 18:43:01 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/19 18:27:36 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/lexer.h"

int	ft_handle_heredoc(t_simple_cmds *parser)
{
	char	*line;
	int		tmp_fd;

	line = NULL;
	tmp_fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0600);
	while (1)
	{
		line = readline(">");
		// gerer les signaux;
			//afficher le message d'erreur bash en cas de ctrC/D
		if (!line || ft_strcmp(line, parser->redirections->str) == 0)
			break;
		if (tmp_fd < 0)
		{
			perror("heredoc");
			exit(1);
		}
		line = get_next_line(tmp_fd); 
		free(line);
	}
	free(line);
	return (tmp_fd);
}


// int	my_choosen_exec(char *str, t_shell *shell, t_parser *parser)
// {
// 	char *unset[] = {
// 		"UNSET" ,
// 		"1pager" ,
// 		"_less",
// 		"PWD",
// 		NULL
// 	};

// 	char *export[] = {
// 		"EXPORT" ,
// 		"LESS=2TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT1",
// 		"ttttttttttttttttt=tttttttttttt" ,
// 		"452ffffffffffffffffffffff",
// 		"_ttttttttttttttttttt"
// 		"-ffffffffffffffffffffffff"
// 		"tttttttttt754t",
// 		"t=454ttttttttttttttttttttt",
// 		NULL
// 	};
// 	t_parser *cur;

// 	cur = NULL;
// 	while (cur != NULL)
// 	{
// 		if (cur->redirections->str == HERE_DOC)
// 		{
// 			ft_handle_heredoc(parser);
// 		}
// 		cur = cur->next;
// 	}

	
// 	// si on a un shell->hidden, on strcmp(cdm, hidden) et si c'est == et qu'il y a un = juste apres, on entre une value et on add le node.
// 	// // if (shell->hidden)
// 	// // {
// 	// 		cur = 
// 	// // 		while ()
// 	// // }
// 	if (!ft_strcmp(str, "ECHO"))
// 		return 0;
// 		// return (ft_cd); ( ? =cd retourne 1 en cas d erreur dee chemin)
// 	else if (!ft_strcmp(str, "PWD"))
// 		return (ft_pwd(shell->env), 0);
// 	else if (!ft_strcmp(export[0], "EXPORT"))
// 		return (ft_export(export, shell), 0);
// 	else if (!ft_strcmp(unset[0], "UNSET"))
// 		return (ft_unset(unset, shell), 0);
// 	else if(!ft_strcmp(str, "ENV"))
// 		return (ft_env(shell->env), 0);
// 	else if (!ft_strcmp(str, "EXIT"))
// 		return 0;
// 		// return (ft_exit);
// 	else
// 		execute_minishell();
// }

void	execute_minishell(t_shell *shell, t_simple_cmds *parser)
{
	t_exec	*exec;
	int		i;

	exec = malloc(sizeof(t_exec));
	if (!exec)
		return (errno);
	exec->env = shell->env;
	exec->path = find_path(parser, shell);
	exec->num_pipes = parser->num_redirections;
	exec->pid = malloc(sizeof(pid_t) * parser->num_redirections + 1);
	exec->fd = malloc(sizeof(int *) * exec->num_pipes + 1); //1 ofr redirection
	exec->heredoc = NULL;
	if (!exec->fd || !exec->pid)
		return (errno);
	i = -1;
	while (++i < exec->num_pipes)
	{
		exec->fd[i] = malloc(sizeof(int) * 2);
		if (!exec->fd[i] || pipe(exec->fd[i]) != 0)
		{
			free_pipes(exec);
			return (errno);
		}
	}
	fork_system_call(parser, exec);
}
void	fork_system_call(t_simple_cmds *parser, t_exec *exec)
{
	int	i;

	i = -1;
	if (!exec->pid)
	{
		perror("");
		free_pipes(exec);
		exit(errno);
	}
	while (++i <= parser->num_redirections)
	{
		exec->pid[i] = fork();
		if (exec->pid[i] < 0)
		{
			perror("");
			free_all(exec);
			exit(errno);
		}
		else if (exec->pid[i] == 0)
			child_process(exec, parser->str[0], i);
	}
	parent_process(exec);
}
// int	manage_dup(int oldfd, int newfd)
// {
// 	if (dup2(oldfd, newfd) < 0)
// 		return (errno);
// 	return (0);
// }

// int	child_process(t_exec *exec, t_simple_cmds *parser, int i)
// {
// 	if (i == 0)
// 	{
// 		open_input(exec, parser);
// 		if (exec->input != -1)
// 		{
// 			manage_dup(exec->input, STDIN_FILENO);
// 			close(exec->input);
// 		}
// 		manage_dup(exec->fd[i][1], STDOUT_FILENO);
// 		close(exec->fd[i][1]);
// 	}
// 	else if (i == exec->num_pipes)
// 	{
// 		open_output(exec, parser);
// 		manage_dup(exec->fd[i - 1][0], STDIN_FILENO);
// 		close(exec->fd[i - 1][0]);
// 		manage_dup(exec->output, STDOUT_FILENO);
// 		close(exec->output);
// 	}
// 	else
// 	{
// 		manage_dup(exec->fd[i - 1][0], STDIN_FILENO);
// 		close(exec->fd[i - 1][0]);
// 		manage_dup(exec->fd[i][1], STDOUT_FILENO);
// 		close(exec->fd[i][1]);
// 	}
// 	closing_child_pipes(exec, parser);
// 	exit(0);
// }
// void	closing_child_pipes(t_exec *exec, t_parser *parser)
// {
// 	int	j;

// 	j = -1;
// 	while (++j < exec->num_pipes)
// 	{
// 		close(exec->fd[j][0]);
// 		close(exec->fd[j][1]);
// 	}
// 	execute_command(parser, exec);
// }

// void	execute_command(t_parser *parser, t_exec *exec)
// {
// 	char	*cmd_path;
// 	t_env	*cur;

// 	cur = exec->env;
// 	cmd_path = find_path(parser->cmd[0], exec->env);
// 	if (!cmd_path)
// 	{
// 		ft_fprintf(2, "%s: command not found\n", parser->cmd);
// 		free_cmd_argv(parser->cmd);
// 		free_all(exec);
// 		exit(127);
// 	}
// 	if (execve(cmd_path, parser->cmd, exec->env) == -1)
// 	{
// 		perror("Execve error");
// 		free_cmd_argv(parser->cmd);
// 		free(cmd_path);
// 		free_all(exec);
// 		exit(126);
// 	}
// 	free(cmd_path);
// 	free_cmd_argv(parser->cmd);
// }

// void	parent_process(t_exec *exec)
// {
// 	int	i;
// 	int	final_status;

// 	i = 0;
// 	final_status = 0;
// 	while (i < exec->num_pipes)
// 	{
// 		close(exec->fd[i][0]);
// 		close(exec->fd[i][1]);
// 		++i;
// 	}
// 	i = 0;
// 	while (i <= exec->num_pipes)
// 	{
// 		waitpid(exec->pid[i], &exec->status, 0);
// 		if (WIFEXITED(exec->status))
// 			final_status = WEXITSTATUS(exec->status);
// 		else if (WIFSIGNALED(exec->status))
// 			final_status = 128 + WTERMSIG(exec->status);
// 		i++;
// 	}
// 	free_all(exec);
// 	exit(final_status);
// }
