#include "../../include/monitor.h"

/**
 * @brief	Function ot print a error message
 * 
 * @param	error Error type. Cases defined in `t_error` enum,
 * 			defined on `monitor.h`
 * @param	argv Arguments
*/
int	argument_error(t_error error, char **argv)
{
	switch (error)
	{
		case ARG_COUNT:
			printf("Argument error!\n\tUsage: %s <LAG>\n", argv[0]);
			break;
		
		case ARG_LAG:
			printf("Incorrect time of cooldown! (\"%s\")\n", argv[1]);

		default:
			break;
	}
	return 1;
}

/**
 * @brief	It saves the arguments data
 * 
 * @param	argc Number of arguments
 * @param	argv Arguments
 * @param	data Pointer to the variable in which the info will be saved
 * 
 * @return	`NONE` on success; otherwise, the error value
*/
t_error	save_arguments(int argc, char **argv, t_data *data)
{
	if (argc != 2)
		return ARG_COUNT;
	
	data->lag = atol(argv[1]);
	if (data->lag < MIN_LAG)
		return ARG_LAG;
	
	return NONE;
}


int main(int argc, char *argv[])
{
	int		fd;
	t_data	data;
	t_error	arg_error;

	if (argc != 2)
		printf("Error: %s\n", argv[0]);
	
	/* Argument checker */
	arg_error = save_arguments(argc, argv, &data);
	if (arg_error)
		return argument_error(arg_error, argv);

	/*
		Open the shared memory. Also set the argument the type of process (CHECKER or MONITOR).
		If the memory block is open correctly, it is mapped to the process.
	*/
	fd = shared_memory_init(&data.whoami, SHARED_MEMORY_NAME, MSG_BLOCK_LENGTH);
	if (fd == -1)
	{
		perror("Error while creating/opening the shared memory");
		return 1;
	}

	data.info_shared = shared_memory_map(fd, MSG_BLOCK_LENGTH, data.whoami);
	if (data.info_shared == MAP_FAILED)
	{
		perror("Error mapping the shared memory");
		shared_memory_destroy(fd, SHARED_MEMORY_NAME, DELETE);
		return (1);
	}

	/* Process identification */
	if (data.whoami == CHECKER)
	{
		shared_memory_destroy(fd, SHARED_MEMORY_NAME, NO_DELETE);
		checker_routine(&data);
	}
	else
	{
		shared_memory_destroy(fd, SHARED_MEMORY_NAME, DELETE);
		monitor_routine(&data);
	}

	/* Resource release */
	if (shared_memory_munmap(data.info_shared, MSG_BLOCK_LENGTH))
	{
		perror("Error deleting the memory map");
		return 1;
	}

	return 0;
}
