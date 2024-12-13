/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: morgane <morgane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 18:43:01 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/13 14:49:14 by morgane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char **transform_env_list_to_tab(t_shell *shell, t_exec *exec)
{
	t_env *cur;
	int i;
	int size_env;

	cur = shell->env;
	i = 0;
	size_env = ft_envsize_minishell(shell->env);
	exec->env = malloc(sizeof(char *) * (size_env + 1));
	if (!exec->env)
		return NULL;
	exec->env[size_env] = NULL;
	while (cur)
	{
		exec->env[i] = ft_strdup(cur->content);
		if (!exec->env[i])
		{
			while (i-- > 0)
				free(exec->env[i]);
			free(exec->env);
		}
		cur = cur->next;
		i++;
	}
	return (exec->env);
}

int execute_minishell(t_shell *shell, t_simple_cmds *parser)
{
	t_exec *exec;
	int i;

	exec = NULL;
	i = -1;
	exec = malloc(sizeof(t_exec));
	if (!exec)
		return (errno);
	if (ft_lstsize_minishell(parser) == 1)
	{
		ft_memset(exec, 0, sizeof(t_exec));
		if (parser->str && is_builtin(parser->str[0]))
		{
			int builtin_status = execute_builtin(parser, shell);
			return (builtin_status);
		}
	}
	exec->env = NULL;
	exec->path = find_path(parser, shell);
	exec->num_pipes = ft_lstsize_minishell(parser) - 1;
	exec->pid = malloc(sizeof(pid_t) * (exec->num_pipes + 1));
	exec->fd = malloc(sizeof(int *) * (exec->num_pipes));
	exec->input = -1;
	exec->output = -1;
	if (!exec->fd || !exec->pid)
	{
		perror("");
		return (errno);
	}
	i = -1;
	while (++i < exec->num_pipes)
	{
		exec->fd[i] = malloc(sizeof(int) * 2);
		if (!exec->fd[i] || pipe(exec->fd[i]) != 0)
		{
			cleanup_and_exit(exec, shell);
			return (errno);
		}
	}
	fork_system_call(parser, exec, shell);
	// free(exec->pid);
	if (exec->fd)
	{
		while (++i < exec->num_pipes)
			free(exec->fd[i]);
		free(exec->fd);
	}
	free(exec);
	return (0);
}

void fork_system_call(t_simple_cmds *parser, t_exec *exec, t_shell *shell)
{
	int i;
	t_simple_cmds *cur;

	i = -1;
	cur = parser;
	while (++i <= exec->num_pipes)
	{
		exec->pid[i] = fork();
		if (exec->pid[i] < 0)
		{
			perror("");
			free_all(exec);
			exit(errno);
		}
		else if (exec->pid[i] == 0)
		{
			child_process(exec, cur, i, shell);
		if (!parser || !parser->str || !parser->str[0])
		{
			ft_fprintf(2, "Minishell: syntax error near unexpected token\n");
			cleanup_heredoc_files(exec);
			free_all(exec);
			return;
		}
		}
		cur = cur->next;
	}
	// printf("I am going to parent process\n");
	// fflush(stdout);
	parent_process(exec);
}

int child_process(t_exec *exec, t_simple_cmds *parser, int i, t_shell *shell)
{
	if (handle_redirections(exec, parser) < 0)
	{
		perror("");
		exit(1);
	}
	if (exec->num_heredoc > 0)
	{
		if (dup2(exec->heredoc_fd[0], STDIN_FILENO) < 0)
		{
			perror("dup2 failed for heredoc");
			cleanup_and_exit(exec, shell);
		}
		close(exec->heredoc_fd[0]);
	}
	if (exec->num_pipes > 0)
	{
		if (i == 0)
		{
			if (exec->output != -1)
				dup2(exec->output, STDOUT_FILENO);
			else if (dup2(exec->fd[i][1], STDOUT_FILENO) < 0)
			{
				perror("");
				cleanup_and_exit(exec, shell);
			}
			// printf("Pipe first %d created: read=%d, write=%d\n", i, exec->fd[i][0], exec->fd[i][1]);
			// fflush(stdout);
		}
		else if (i == exec->num_pipes)
		{
			if (dup2(exec->fd[i - 1][0], STDIN_FILENO < 0))
			{
				perror("");
				cleanup_and_exit(exec, shell);
			}
			// printf("Pipe last %d input: fd=%d -> STDIN\n", i, exec->fd[i - 1][0]);
			// fflush(stdout);
			if (exec->output != -1)
			{
				if (dup2(exec->output, STDOUT_FILENO) < 0)
				{
					perror("");
					cleanup_and_exit(exec, shell);
				}
				// printf("Output redirection applied: fd=%d -> STDOUT\n", exec->output);
				// fflush(stdout);
			}
		}
		else
		{
			if (dup2(exec->fd[i - 1][0], STDIN_FILENO) < 0 || dup2(exec->fd[i][1], STDOUT_FILENO) < 0)
			{
				perror("");
				cleanup_and_exit(exec, shell);
			}
			// printf("Pipe mid %d created: input fd=%d -> STDIN, output fd=%d -> STDOUT\n", i, exec->fd[i - 1][0], exec->fd[i][1]);
			// fflush(stdout);
		}
	}
	closing_child_pipes(exec, i);
	execute_command(parser, shell, exec);
	return (0);
}


void execute_command(t_simple_cmds *parser, t_shell *shell, t_exec *exec)
{
	char *cmd_path;
	int size_env;
	int i = 0;

	while (parser != NULL)
	{
		// if (parser->str[0] == NULL)
		// 	break;
		if (parser->str && is_builtin(parser->str[0]))
		{
			execute_builtin(parser, shell);
			parser = parser->next;
		}
		size_env = ft_envsize_minishell(shell->env);
		cmd_path = find_path(parser, shell);
		printf("cmd path = %s \n", cmd_path);
		fflush(stdout);
		if (parser && parser->str)
		{
			if (!cmd_path )
			{
				ft_fprintf(2, "%s: command not found\n", parser->str[0]);
				free_cmd_argv(parser);
				free_all(exec);
				exit(127);
			}
		}
		if (exec->env)
			free_exec_env(exec->env);
		exec->env = malloc(sizeof(char *) * (size_env + 1));
		if (!exec->env)
			return;
		exec->env[size_env] = NULL;
		exec->env = transform_env_list_to_tab(shell, exec);
		// printf("parser str = %s\n", parser->str[0]);
		// fflush(stdout);
		// printf(" j'essaye d'exec  %d fois\n", i);
		fflush(stdout);
		i++;
		if (parser && parser->str && cmd_path)
		{
			if (execve(cmd_path, parser->str, exec->env) == -1)
			{
				perror(parser->str[0]);
				free(cmd_path);
				free_cmd_argv(parser);
				free_all(exec);
				exit(126);
			}
			free(cmd_path);
			free_cmd_argv(parser);
			free_all(exec);
		}
		parser = parser->next;
		cleanup_heredoc_files(exec);
	}
}

	int parent_process(t_exec * exec)
	{
		int i;
		int final_status;

		i = 0;
		final_status = 0;
		while (i < exec->num_pipes)
		{
			// printf("Parent closed fd[%d][0]\n", i);
			// printf("Parent closed fd[%d][1]\n", i);
			close(exec->fd[i][0]);
			close(exec->fd[i][1]);
			++i;
		}
		i = 0;
		while (i <= exec->num_pipes)
		{
			waitpid(exec->pid[i], &exec->status, 0);
			if (WIFEXITED(exec->status))
				final_status = WEXITSTATUS(exec->status);
			else if (WIFSIGNALED(exec->status))
				final_status = 128 + WTERMSIG(exec->status);
			i++;
		}
		// free_all(exec);
		return (final_status);
	}

int execute_builtin(t_simple_cmds *parser, t_shell *shell)
{
	// 	// si on a un shell->hidden, on strcmp(cdm, hidden) et si c'est == et qu'il y a un = juste apres, on entre une value et on add le node.
	// 	// // if (shell->hidden)
	// 	// // {
	// 	// 		cur =
	// 	// // 		while ()
	// 	// // }
	if (!ft_strcmp(parser->str[0], "cd"))
	    return (builtin_cd(parser, shell->env), 0); // =cd retourne 1 en cas d erreur dee chemin)
	// if (!strcmp(parser->str[0], "export"))
	//     return (ft_export(parser, shell), 0);
	if (!ft_strcmp(parser->str[0], "unset"))
		return (ft_unset(parser->str, shell), 0);
	if (!ft_strcmp(parser->str[0], "env"))
		return (ft_env(shell->env), 0);
	if (!ft_strcmp(parser->str[0], "exit"))
		return (builtin_exit(parser, shell), 0);
	if (!ft_strcmp(parser->str[0], "echo"))
		return (builtin_echo(parser), 0);
	return (0);
}

// int execute_single_command(t_simple_cmds *parser, t_shell *shell, t_exec *exec)
// {
// 	pid_t pid;
// 	char *cmd_path;
// 	int status;
// 	int	size_env;
// 	int builtin_status;

// 	if (handle_redirections(exec, parser) < 0)
// 	{
// 		ft_fprintf(stderr, "Error: Redir failed\n");
// 		return (1);
// 	}
// 	if (!parser->str || !parser->str[0])
// 	{
// 		fprintf(stderr, "Error: Command is empty\n");
// 		return (1);
// 	}
// 	if (is_builtin(parser->str[0]))
// 	{
// 		builtin_status = execute_builtin(parser, shell);
// 		return (builtin_status);
// 	}
// 	cmd_path = find_path(parser, shell);
// 	if (!cmd_path)
// 	{
// 		ft_fprintf(stderr, "%s: command not found\n", parser->str[0]);
// 		return (127);
// 	}
// 	pid = fork();
// 	if (pid < 0)
// 	{
// 		perror("");
// 		free(cmd_path);
// 		return (1);
// 	}
// 	else if (pid == 0)
// 	{
// 		size_env = ft_envsize_minishell(shell->env);
// 		if (exec->env)
// 			free_exec_env(exec->env);
// 		exec->env = malloc(sizeof(char *) * (size_env + 1));
// 		if (!exec->env)
// 			return (1);
// 		exec->env[size_env] = NULL;
// 		exec->env = transform_env_list_to_tab(shell, exec);
// 		if (execve(cmd_path, parser->str, exec->env) == -1)
// 		{
// 			perror(parser->str[0]);
// 			free(cmd_path);
// 			exit(126);
// 		}
// 	}
// 	else
// 	{
// 		waitpid(pid, &status, 0);
// 		free(cmd_path);
// 		if (WIFEXITED(status))
// 			status = WEXITSTATUS(exec->status);
// 		if (WIFSIGNALED(status))
// 			status = 128 + WTERMSIG(exec->status);
// 	}
// 	// IL FAUT FREE FILENAMES DES HEREDOCS BEFORE UNLINK
// 	unlink(".heredoc_tmp");
// 	return (0);
// }
