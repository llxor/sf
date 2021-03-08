#include <config.h>

struct error
{
	char file[100], msg[500];
	int line, col;
};

void buildcmd(char *cmd, int argc, char **argv)
{
	for (int i = 1; i < argc; i++) {
		strcat(cmd, argv[i]);
		strcat(cmd, " ");
	}
	strcat(cmd, " 2>&1");
}

int exitcode = 0;

int parse(const char cmd[], struct error errors[MAX_ERR], char exe[])
{
	FILE *proc = popen(cmd, "r");

	if (proc == NULL) {
		puts("process failed to start");
		exit(1);
	}

	char buffer[1000];
	int len = 0;

	while (fgets(buffer, sizeof buffer, proc) != NULL) {
		struct error e = {.line = -1,.col = -1 };
		sscanf(buffer, "%[^:]:%d:%d: %[^\n]", e.file, &e.line, &e.col, e.msg);

		if (e.line != -1) {
			if (e.col == -1) {
				sscanf(buffer, "%[^:]:%d: %[^\n]", e.file, &e.line, e.msg);
				e.col = 0;
			}

			errors[len++] = e;
		}

		else if (strcmp(e.file, exe) == 0) {
			sscanf(buffer, "%[^\n]", e.msg);
			errors[len++] = e;
		}

		if (len == MAX_ERR) {
			break;
		}
	}

	exitcode = WEXITSTATUS(pclose(proc));
	return len;
}

char command[1000] = {0};
void load_command(struct error err)
{
	const char *src = editor;
	char *write = command;

	while (*src)
	{
		if (*src == '%')
		{
			switch(*(++src))
			{
			case 'f':
				strcpy(write, err.file);
				write += strlen(err.file);
				break;

			case 'l':
				  write += sprintf(write, "%d", err.line);
				  break;

			case 'c':
				  write += sprintf(write, "%d", err.col);
				  break;
			}
			src++;
		}

		else {
			*write++ = *src++;
		}
	}
}
