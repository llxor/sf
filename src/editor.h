char command[1000];

void load_command(struct error e)
{
	int len = command[0] = 0;

	for (const char *src = editor; *src != '\0'; src++) {
		if (*src == '[') {
			if (strncmp(src, "[file]", 6) == 0) {
				strcat(command, e.file);
				src += 5;
				len += strlen(e.file);
			}

			if (strncmp(src, "[line]", 6) == 0) {
				sprintf(&command[len], "%d", e.line);
				src += 5;
			}

			if (strncmp(src, "[col]", 5) == 0) {
				sprintf(&command[len], "%d", e.col);
				src += 4;
			}

			while (command[len]) {
				len++;
			}
		}

		else {
			command[len++] = src[0];
			command[len] = 0;
		}
	}
}
