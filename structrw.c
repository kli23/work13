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

int next_line(char *text) {
	int a = 1;
	while (* (text + a) != '\n')
		a++;
	return a++;
}

void read_csv() {
	printf("reading nyc_pop.csv\n");


	int a = open("nyc_pop.csv", O_RDONLY, 0); // opens csv file
	if (a == -1) {
		printf("Error: %s\n", strerror(errno));
		return;
	}


	struct stat ss;
	stat("nyc_pop.csv", &ss);
	int file_size = (&ss) -> st_size; // get the size of entire file to know how many bytes that need to be read


	char text[file_size];  // reads csv file into string
	int b = read(a, text, file_size);
	if (b == -1) {
		printf("Error: %s\n", strerror(errno));
		return;
	}


	int line_count = line_counter(text); //get number of lines
	

	struct pop_entry array[ (line_count - 1) * 5 ]; // create array of struct pop_entry


	int i, j;
	char *start = text;
	start += next_line(start); // passes through first line aka header
	// printf("%s", start); debugging


	char *boroughs[5] = {"Manhattan","Brooklyn","Queens","Bronx","Staten Island"};
	int yr;
	int pop[5]; // for the 5 populations, one per borough


	for (i = 0; i < line_count - 1; i++) { //store contents of file into array of pop entries
		sscanf(start, "%d,%d,%d,%d,%d,%d\n", &yr, pop, pop + 1, pop + 2, pop + 3, pop + 4);
		// printf("%d %d %d %d %d %d ", yr, pop[0], pop[1], pop[2], pop[3], pop[4]); debugging
		for (j = 0; j < 5; j++) {
			array[i * 5 + j].year = yr;
			array[i * 5 + j].population = pop[j];
			strcpy(array[i * 5 + j].boro, boroughs[j]);
		}
		start += next_line(start); // increment to next line of string
	}


	int c = open("nyc_pop.data", O_WRONLY | O_TRUNC | O_CREAT, 0644); // open new file 
	if (c == -1) {
		printf("Error: %s\n", strerror(errno));
		return;
	}


	int d = write(c, array, sizeof(array)); // write array of structs to new file
	if (d == -1) {
		printf("Error: %s\n", strerror(errno));
		return;
	}
	printf("wrote %d bytes to nyc_pop.data\n", d);
}

void read_data() {


	int a = open("nyc_pop.data", O_RDONLY, 0); // opens data file
	if (a == -1) {
		printf("Error: %s\n", strerror(errno));
		return;
	}


	struct stat ds;
	stat("nyc_pop.data", &ds);
	int file_size = (&ds) -> st_size; 
	struct pop_entry * array = malloc(file_size);


	int b = read(a, array, file_size);
	int inc = 0;
	while ( inc < file_size / sizeof(struct pop_entry) ) { // tried using *(array + inc) but didn't work
		printf("%d: year: %d\tboro: %s\tpop: %d\n", inc, array[inc].year, array[inc].boro, array[inc].population);
		inc++;
	}
}

void add_data() {


	int yr, pop;
	char borough[15];
	char string[sizeof(struct pop_entry)];
	printf("Enter year boro pop:  ");
    fgets(string, sizeof(struct pop_entry), stdin);
	sscanf(string, "%d %s %d", &yr, borough, &pop);


	struct pop_entry new;
	strcpy(new.boro, borough);
	new.year = yr;
	new.population = pop;


	int c = open("nyc_pop.data", O_WRONLY | O_APPEND); // open data file 
	if (c == -1) {
		printf("Error: %s\n", strerror(errno));
		return;
	}


	int d = write(c, &new, sizeof(new)); // appends new pop_entry
	if (d == -1) {
		printf("Error: %s\n", strerror(errno));
		return;
	}
	printf("Appended data to file: year: %d\t boro: %s\t pop: %d\n", new.year, new.boro, new.population);

}

void update_data() {


	int index, yr, pop;
	char borough[15];
	char string[sizeof(struct pop_entry) + sizeof(int)];


	struct stat ds;
	stat("nyc_pop.data", &ds);
	int file_size = (&ds) -> st_size; 


	printf("Enter entry year boro pop:  "); // std input index and pop_entry values
    fgets(string, sizeof(struct pop_entry) + sizeof(int), stdin);
	sscanf(string, "%d %d %s %d", &index, &yr, borough, &pop);


	struct pop_entry new; // make new pop_entry
	strcpy(new.boro, borough);
	new.year = yr;
	new.population = pop;


	int c = open("nyc_pop.data", O_WRONLY); // open data file 
	if (c == -1) {
		printf("Error: %s\n", strerror(errno));
		return;
	}


	lseek(c, sizeof(struct pop_entry) * index, SEEK_SET); // sets current position in open file to correct index


	int d = write(c, &new, sizeof(new));  // rewrites index
	if (d == -1) {
		printf("Error: %s\n", strerror(errno));
		return;
	}


	printf("File updated.\n");
	

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
