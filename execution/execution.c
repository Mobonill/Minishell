/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: morgane <morgane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 18:43:01 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/27 13:23:40 by morgane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**transform_env_list_to_tab(t_shell *shell, t_exec *exec)
{
	t_env	*cur;
	int		i;
	int		size_env;
	
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
		cur = cur->next;
		i++;
	}
	return (exec->env);
}

int	execute_minishell(t_shell *shell, t_simple_cmds *parser)
{
	t_exec	*exec;
	int		i;

	if (ft_lstsize_minishell(parser) == 1) // Une seule commande
	{
		if (is_builtin(parser->str[0])) // Vérifiez si c'est un builtin
			return (execute_builtin(parser, shell), 0);
		return (execute_single_command(parser, shell), 0); // Exécute une commande unique
	}
	exec = malloc(sizeof(t_exec));
	if (!exec)
		return (errno);
	exec->path = find_path(parser, shell);
	exec->num_pipes = ft_lstsize_minishell(parser) - 1;
	exec->pid = malloc(sizeof(pid_t) * (exec->num_pipes + 1));
	exec->fd = malloc(sizeof(int *) * (exec->num_pipes) + 3);
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
			return (errno);
		}
	}
	fork_system_call(parser, exec, shell);
	free(exec->pid);
	for (i = 0; i < exec->num_pipes; i++)
		free(exec->fd[i]);
	free(exec->fd);
	return (0);
}
void	fork_system_call(t_simple_cmds *parser, t_exec *exec, t_shell *shell)
{
	int	i;

	i = -1;
	while (++i <= exec->num_pipes)
	{
		exec->pid[i] = fork();
		printf("pid i %d\n", exec->pid[i]);
		if (exec->pid[i] < 0)
		{
			perror("");
			// free_all(exec);
			exit(errno);
		}
		else if (exec->pid[i] == 0)
		{
			child_process(exec, parser, i, shell);
		}
	}
	parent_process(exec);
}

int	child_process(t_exec *exec, t_simple_cmds *parser, int i, t_shell *shell)
{
	if (handle_redirections(exec, parser) < 0)
		exit (1);
	printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa");
	if (i == 0)
	{
		if (dup2(exec->fd[i][1], STDOUT_FILENO) < 0)
		{
			perror("FIRST dup2 failed");
			exit(1);
		}
		// close(exec->fd[i][1]);
	}
	else if (i == exec->num_pipes)
	{
		printf("BBBBBBBBBBBBBBBBBBBBBBBBBBB");
		if (dup2(exec->fd[i - 1][0], STDIN_FILENO) < 0)
		{
			perror("END dup2 failed");
			exit(1);
		}
		// close(exec->fd[i - 1][0]);
	}
	else
	{
		printf("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC");
		if (dup2(exec->fd[i - 1][0], STDIN_FILENO) < 0 || dup2(exec->fd[i][1], STDOUT_FILENO) < 0)
		{
			perror("MID dup2 failed\n");
			exit(1);
		}
		// close(exec->fd[i - 1][0]);
		// close(exec->fd[i][1]);
	}
	closing_child_pipes(exec, i);
	execute_command(parser, shell, exec);
	exit(0);
}

void	execute_command(t_simple_cmds *parser, t_shell *shell, t_exec *exec)
{
	char	*cmd_path;
	int		size_env;

	size_env = ft_envsize_minishell(shell->env);
	cmd_path = find_path(parser, shell);
	if (!cmd_path)
	{
		ft_fprintf(2, "%s: Hcommand not found\n", parser->str[0]);
		free_cmd_argv(parser);
		free_all(exec);
		exit(127);
	}
	exec->env = malloc(sizeof(char *) * (size_env + 1));
	exec->env[size_env] = NULL;
	if (!exec->env)
		return ;
	transform_env_list_to_tab(shell, exec);
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
}

int	parent_process(t_exec *exec)
{
	int	i;
	int	final_status;

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
	// if (!strcmp(parser->str[0], "cd"))
	//     return (ft_cd(parser->str, shell)); // =cd retourne 1 en cas d erreur dee chemin)
	// if (!strcmp(parser->str[0], "export"))
	//     return (ft_export(parser, shell), 0);
	if (!strcmp(parser->str[0], "unset"))
		return (ft_unset(parser->str, shell), 0);
	if (!strcmp(parser->str[0], "env"))
		return (ft_env(shell->env), 0);
	// if (!strcmp(parser->str[0], "exit"))
	//     return (ft_exit(parser->str), 0);
	// if (!strcmp(parser->str[0], "echo"))
	//     return (ft_echo(parser->str), 0);
	return (0);
}

int	execute_single_command(t_simple_cmds *parser, t_shell *shell)
{
	pid_t pid;
	char *cmd_path;
	int status;

	cmd_path = find_path(parser, shell);
	if (!cmd_path)
	{
		fprintf(stderr, "%s: command not found\n", parser->str[0]);
		return (127);
	}

	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		free(cmd_path);
		return (1);
	}
	else if (pid == 0)
	{
		if (execve(cmd_path, parser->str, shell->envp) == -1)
		{
			perror(parser->str[0]);
			free(cmd_path);
			exit(126);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		free(cmd_path);
		if (WIFEXITED(status))
			return WEXITSTATUS(status);
		if (WIFSIGNALED(status))
			return 128 + WTERMSIG(status);
	}
	return (0);
}
