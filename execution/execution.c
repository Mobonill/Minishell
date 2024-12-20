/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: morgane <morgane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 18:43:01 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/20 19:04:30 by morgane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char **transform_env_list_to_tab(t_shell *shell, t_exec *exec)
{
	t_env	*cur;
	int		i;
	int		size_env;

	cur = shell->env;
	size_env = ft_envsize_minishell(shell->env);
	exec->env = malloc(sizeof(char *) * (size_env + 1));
	if (!exec->env)
		return NULL;
	exec->env[size_env] = NULL;
	i = 0;
	while (cur)
	{
		exec->env[i] = ft_strdup(cur->content);
		if (!exec->env[i])
		{
			while (--i >= 0)
				free(exec->env[i]);
			free(exec->env);
			exec->env = NULL;
			return NULL;
		}
		cur = cur->next;
		i++;
	}
	return exec->env;
}


int init_exec_memory(t_exec *exec, t_simple_cmds *parser)
{
	exec->env = NULL;
	exec->num_pipes = ft_lstsize_minishell(parser) - 1;
	exec->pid = malloc(sizeof(pid_t) * (exec->num_pipes + 1));
	exec->fd = malloc(sizeof(int *) * (exec->num_pipes));
	exec->input = -1;
	exec->output = -1;
	exec->num_heredoc = 0;
	if (!exec->fd || !exec->pid)
	{
		perror("");
		free(exec->fd);
		free(exec->pid);
		free(exec);
		g_global_exit = errno;
		return (g_global_exit);
	}
	return (0);
}

int allocate_pipes(t_exec *exec, t_shell *shell)
{
	int i = -1;
	while (++i < exec->num_pipes)
	{
		if (exec->fd)
		{
			exec->fd[i] = malloc(sizeof(int) * 2);
			if (!exec->fd[i] || pipe(exec->fd[i]) != 0)
			{
				while (--i >= 0)
					free(exec->fd[i]);
				free(exec->fd);
				free(exec->pid);
				cleanup_and_exit(exec, shell);
				g_global_exit = errno;
				return (g_global_exit);
			}
		}
	}
	return (0);
}

void cleanup_exec_resources(t_exec *exec)
{
	free(exec->pid);
	if (exec->fd)
	{
		int i = -1;
		while (++i < exec->num_pipes)
			free(exec->fd[i]);
		free(exec->fd);
	}
	free(exec);
}

void save_and_restore_std(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
}

void restore_std(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

int execute_builtin(t_simple_cmds *parser, t_shell *shell, t_exec *exec)
{
	int saved_stdin;
	int saved_stdout;

	save_and_restore_std(&saved_stdin, &saved_stdout);
	if (handle_redirections_and_heredoc(exec, parser, 0, shell) < 0)
	{
		restore_std(saved_stdin, saved_stdout);
		return (-1);
	}
	if (!ft_strcmp(parser->str[0], "cd"))
		builtin_cd(parser, shell->env);
	else if (!ft_strcmp(parser->str[0], "export"))
		builtin_export(parser, shell);
	else if (!ft_strcmp(parser->str[0], "unset"))
		ft_unset(parser->str, shell);
	else if (!ft_strcmp(parser->str[0], "env"))
		ft_env(shell->env);
	else if (!ft_strcmp(parser->str[0], "exit"))
		builtin_exit(parser, shell);
	else if (!ft_strcmp(parser->str[0], "echo"))
		builtin_echo(parser);
	else if (!ft_strcmp(parser->str[0], "pwd"))
		ft_pwd(shell->env);
	restore_std(saved_stdin, saved_stdout);
	return (0);
}

int execute_minishell(t_shell *shell, t_simple_cmds *parser)
{
	t_exec *exec;
	int result;

	exec = malloc(sizeof(t_exec));
	if (!exec)
		return (errno);
	if (ft_lstsize_minishell(parser) == 1 && is_builtin(parser->str[0]))
	{
		result = execute_builtin(parser, shell, exec);
		free(exec);
		return (result);
	}
	if (init_exec_memory(exec, parser) != 0)
		return (1);
	if (allocate_pipes(exec, shell) != 0)
		return (1);
	fork_system_call(parser, exec, shell);
	int i = 0;
	while (i <= exec->num_pipes)
	{
		waitpid(exec->pid[i], &exec->status, 0);
		if (WIFEXITED(exec->status))
			g_global_exit = WEXITSTATUS(exec->status);
		else if (WIFSIGNALED(exec->status))
			g_global_exit = 128 + WTERMSIG(exec->status);
		i++;
	}
	cleanup_exec_resources(exec);
	return (g_global_exit);
}
int handle_redirections_and_heredoc(t_exec *exec, t_simple_cmds *parser, int i, t_shell *shell)
{
	(void)shell;

	if (handle_redirections(exec, parser) != 0)
		return (g_global_exit);
	if (LastHeredocIsRedirected(exec) != 0)
		return (g_global_exit);
	if (exec->num_heredoc > 0 && i < exec->num_heredoc)
	{
		if (exec->heredoc_fd[i] != -1)
		{
			close(exec->heredoc_fd[i]);
			exec->heredoc_fd[i] = -1;
		}
	}
	if (!parser || !parser->str || !parser->str[0])
		return (cleanup_heredoc_files(exec), g_global_exit);
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
			child_process(exec, cur, i, shell);
		cur = cur->next;
	}
	parent_process(exec, parser);
}

// int handle_redirections_and_heredoc(t_exec *exec, t_simple_cmds *parser, int i, t_shell *shell)
// {
// 	(void)shell;
// 	if (handle_redirections(exec, parser) < 0)
// 		return (1);
// 	if (LastHeredocIsRedirected(exec) != 0)
// 		return (-1);
// 	if (!parser || !parser->str || !parser->str[0])
// 		return (cleanup_heredoc_files(exec), 0);
// 	if (exec->num_heredoc > 0 && i < exec->num_heredoc)
// 	{
// 		if (exec->heredoc_fd[i] != -1)
// 		{
// 			close(exec->heredoc_fd[i]);
// 			exec->heredoc_fd[i] = -1;
// 		}
// 	}
// 	return (0);
// }

int handle_first_pipe(t_exec *exec, int i)
{
	if (exec->num_pipes > 0 && i == 0)
	{
		if (exec->output != -1)
			dup2(exec->output, STDOUT_FILENO);
		else if (dup2(exec->fd[i][1], STDOUT_FILENO) < 0)
		{
			perror("");
			return (1);
		}
	}
	return (0);
}
int handle_middle_pipe(t_exec *exec, int i)
{
	if (exec->num_pipes > 0 && i > 0 && i < exec->num_pipes)
	{
		if (dup2(exec->fd[i - 1][0], STDIN_FILENO) < 0 ||
			dup2(exec->fd[i][1], STDOUT_FILENO) < 0)
		{
			perror("");
			return (1);
		}
	}
	return (0);
}
int handle_last_pipe(t_exec *exec, int i)
{
	if (exec->num_pipes > 0 && i == exec->num_pipes)
	{
		if (dup2(exec->fd[i - 1][0], STDIN_FILENO) < 0)
		{
			perror("");
			return (1);
		}
		if (exec->output != -1)
		{
			if (dup2(exec->output, STDOUT_FILENO) < 0)
			{
				perror("");
				return (1);
			}
		}
	}
	return (0);
}
int child_process(t_exec *exec, t_simple_cmds *parser, int i, t_shell *shell)
{
	if (handle_redirections_and_heredoc(exec, parser, i, shell) < 0)
		exit(1);
	if ((!parser || !parser->str || !parser->str[0]) && exec->num_pipes == 0)
		exit(0);
	if (exec->num_pipes > 0)
	{
		if (i == 0)
		{
			if (handle_first_pipe(exec, i) < 0)
				exit(1);
		}
		else if (i == exec->num_pipes)
		{
			if (handle_last_pipe(exec, i) < 0)
				exit(1);
		}
		else
		{
			if (handle_middle_pipe(exec, i) < 0)
				exit(1);
		}
		closing_child_pipes(exec, i);
	}
	if (is_builtin(parser->str[0]))
	{
		execute_builtin(parser, shell, exec);
		exit(g_global_exit);
	}
	execute_command(parser, shell, exec);
	return (0);
}

void ft_cmd_path(char *cmd_path, t_simple_cmds *parser, t_exec *exec)
{
	if (cmd_path)
	{
		if (execve(cmd_path, parser->str, exec->env) == -1)
		{
			perror(parser->str[0]);
			free(cmd_path);
			free_cmd_argv(parser);
			free_all(exec);
			g_global_exit = 126;
			exit(126);
		}
	}
	else
	{
		free_cmd_argv(parser);
		free_all(exec);
		g_global_exit = 127;
		exit(127);
	}
}
void create_exec_env(t_shell *shell, t_exec *exec, char *cmd_path, t_simple_cmds *parser)
{
	exec->env = transform_env_list_to_tab(shell, exec);
	cmd_path = find_path(parser, shell);
	ft_cmd_path(cmd_path, parser, exec);
}
void execute_command(t_simple_cmds *parser, t_shell *shell, t_exec *exec)
{
	char *cmd_path;

	cmd_path = NULL;
	while (parser != NULL)
	{
		if (!parser || !parser->str || !parser->str[0])
		{
			parser = parser->next;
			continue;
		}
		// if (is_builtin(parser->str[0]))
		// {
		// 	exec->env = NULL;
		// 	execute_builtin(parser, shell, exec);
		// 	if (parser->str)
		// 		free_cmd_argv(parser);
		// 	parser = parser->next;
		// }
		create_exec_env(shell, exec, cmd_path, parser);
		free(cmd_path);
		free_cmd_argv(parser);
		free_all(exec);
		if (cmd_path)
			free(cmd_path);
	}
}

void parent_process(t_exec *exec, t_simple_cmds *parser)
{
	int i;

	(void)parser;
	i = 0;
	while (i < exec->num_pipes)
	{
		close(exec->fd[i][0]);
		close(exec->fd[i][1]);
		++i;
	}
	// i = 0;
	// while (i <= exec->num_pipes)
	// {
	// 	waitpid(exec->pid[i], &exec->status, 0);
	// 	if (WIFEXITED(exec->status))
	// 		g_global_exit = WEXITSTATUS(exec->status);
	// 	else if (WIFSIGNALED(exec->status))
	// 		g_global_exit = 128 + WTERMSIG(exec->status);
	// 	i++;
	// }
}

// int execute_builtin(t_simple_cmds *parser, t_shell *shell)
// {
// 	if (!ft_strcmp(parser->str[0], "cd"))
// 		return (builtin_cd(parser, shell->env), 0);
// 	if (!ft_strcmp(parser->str[0], "export"))
// 		return (builtin_export(parser, shell), 0);
// 	if (!ft_strcmp(parser->str[0], "unset"))
// 		return (ft_unset(parser->str, shell), 0);
// 	if (!ft_strcmp(parser->str[0], "env"))
// 		return (ft_env(shell->env), 0);
// 	if (!ft_strcmp(parser->str[0], "exit"))
// 		return (builtin_exit(parser, shell), 0);
// 	if (!ft_strcmp(parser->str[0], "echo"))
// 		return (builtin_echo(parser), 0);
// 	if (!ft_strcmp(parser->str[0], "pwd"))
// 		return (ft_pwd(shell->env), 0);
// 	return (0);
// }
