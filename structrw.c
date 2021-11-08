#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

struct pop_entry {
	int year;
	int population;
	char boro[15];
};

int line_counter(char *text) {
	int n = 1;
	int i;
	for (i = 0; i < strlen(text); i++)
		if ( *(text + i) == '\n') n++;
	if ( *(text + strlen(text) - 1) == '\n') n--;
	return n;
}

void read_csv() {

	int a = open("nyc_pop.csv", O_RDONLY, 0);
	if (a == -1) {
		printf("Error: %s\n", strerror(errno));
		return;
	}


	struct stat ss;
	stat("nyc_pop.csv", &ss);
	int file_size = (&ss) -> st_size; // get the size of entire file to know how many bytes that need to be read


	char line[file_size];
	int b = read(a, line, file_size);
	if (b == -1) {
		printf("Error: %s\n", strerror(errno));
		return;
	}
	printf("\n%s", line);
	printf("\n%d", line_counter(line));


}

void read_data() {

}

void add_data() {

}

void update_data() {

}

int main(int argc, char *argv[]) {

	char command[100];

	if (argc == 1) { // if no command line arg, ask for one
        printf("Enter a command: ");
        fgets(command, 100, stdin);
    } 
    else if (argc > 1){ // $make run ARGS=directory_name
        strcpy(command, argv[1]);
    }

	
	if (!strcmp(command, "-read_csv")) read_csv();

	else if (!strcmp(command, "-read_data")) read_data();

	else if (!strcmp(command, "-add_data")) add_data();

	else if (!strcmp(command, "-update_data")) update_data();

	else printf("Invalid command\n");

}
