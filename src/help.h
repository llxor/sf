#import <stdio.h>

const char *help_msg =
"USAGE: sf [command]\n\n"
"sf provides a simple frontend to compilers.\n\n"
"Keybindings:\n"
"- Up / Down: navigates though the error/warning messages\n"
"- Enter: opens the file at the location of the error/warning\n"
"- q: quits the application\n\n"
"Report bugs to https://github.com/llxor/sf";

void display_help() {
  puts(help_msg);
}
