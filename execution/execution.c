/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 18:43:01 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/13 19:12:20 by mobonill         ###   ########.fr       */
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
	exec->num_heredoc = 0;
	if (!exec->fd || !exec->pid)
	{
		perror("");
		cleanup_heredoc_files(exec);
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
	free(exec->pid);
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
		}
		cur = cur->next;
	}
	parent_process(exec, parser);
}

int child_process(t_exec *exec, t_simple_cmds *parser, int i, t_shell *shell)
{
	if (handle_redirections(exec, parser) < 0)
	{
		perror("");
		return (1);
	}
	if (exec->num_heredoc > 0 && i < exec->num_heredoc)
	{
		if (dup2(exec->heredoc_fd[i], STDIN_FILENO) < 0)
		{
			perror("");
			cleanup_and_exit(exec, shell);
		}
		close(exec->heredoc_fd[i]);
		exec->heredoc_fd[i] = -1;
	}
	if ((!parser || !parser->str || !parser->str[0]) && exec->num_pipes == 0)
	{
		cleanup_heredoc_files(exec);
		exit (0);
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
	char	*cmd_path;
	int		size_env;
	int		i;;

	i = 0;
	if (!parser || !parser->str || !parser->str[0])
	{
		free_all(exec);
		ft_fprintf(2, "no parser\n");
		cleanup_and_exit(exec, shell);
		return;
	}
	while (parser != NULL)
	{
		if (is_builtin(parser->str[0]))
		{
			execute_builtin(parser, shell);
			parser = parser->next;
		}
		size_env = ft_envsize_minishell(shell->env);
		cmd_path = find_path(parser, shell);
		if (!cmd_path)
		{
			ft_fprintf(2, "%s: command not found\n", parser->str[0]);
			free_cmd_argv(parser);
			free_all(exec);
			exit(127);
		}
		if (exec->env)
			free_exec_env(exec->env);
		exec->env = malloc(sizeof(char *) * (size_env + 1));
		if (!exec->env)
			return;
		exec->env[size_env] = NULL;
		exec->env = transform_env_list_to_tab(shell, exec);
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
	}
}

int parent_process(t_exec *exec, t_simple_cmds *parser)
{
	int i;
	int final_status;


	(void)parser;
	i = 0;
	final_status = 0;
	while (i < exec->num_pipes)
	{
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

	return (final_status);
}

int execute_builtin(t_simple_cmds *parser, t_shell *shell)
{
	if (!ft_strcmp(parser->str[0], "cd"))
		return (builtin_cd(parser, shell->env), 0); // =cd retourne 1 en cas d erreur dee chemin)
	// if (!ft_strcmp(parser->str[0], "export")) return (ft_export(parser, shell), 0);
	// 	return(ft_export())
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
