struct error {
	char file[100], msg[500];
	int line, col;
};

enum { MAX_ERR = 20 };
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
		sscanf(buffer, "%[^:]:%d:%d: %[^\n]", e.file, &e.line, &e.col,
		       e.msg);

		if (e.line != -1 && e.col != -1) {
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

void buildcmd(char *cmd, int argc, char **argv)
{
	for (int i = 1; i < argc; i++) {
		strcat(cmd, argv[i]);
		strcat(cmd, " ");
	}
	strcat(cmd, " 2>&1");
}
