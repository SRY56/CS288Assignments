#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int opt;
	int c_flag = 0, d_flag = 0, u_flag = 0;
	int f_val = -1;
	int s_val = -1;
	char *input_file = NULL;
	char *output_file = NULL;

	// Parse command line options
	while ((opt = getopt(argc, argv, "cduf:s:")) != -1) {
		switch (opt) {
			case 'c':
				c_flag = 1;
				printf("Option: -c\n");
				break;
			case 'd':
				d_flag = 1;
				printf("Option: -d\n");
				break;
			case 'u':
				u_flag = 1;
				printf("Option: -u\n");
				break;
			case 'f':
				f_val = atoi(optarg);
				printf("Option: -f with argument '%s' (value: %d)\n", optarg, f_val);
				break;
			case 's':
				s_val = atoi(optarg);
				printf("Option: -s with argument '%s' (value: %d)\n", optarg, s_val);
				break;
			case ':':
				fprintf(stderr, "Error: optiom -%c requires an argument\n", optopt);
                                fprintf(stderr, "Useage: %s [-c] [-d] [-u] [-f fields] [-s char] input_file output_file\n",argv[0]);
				return 1;
			case '?':
				fprintf(stderr, "Error: unknown option '-%c' \n", optopt);
                                fprintf(stderr, "Useage: %s [-c] [-d] [-u] [-f fields] [-s char] input_file output_file\n",argv[0]);
                                return 1;
			default:
				fprintf(stderr, "Error: unknown option '-%c' \n", optopt);
				return 1;
		}
	}

	printf("optind: %d\n", optind);

	if (optind < argc) {
		printf("non-option arguments:");
		for (int i = optind; i < argc; i++) {
			printf(" %s", argv[i]);
		}
		printf("\n");
	} else {
		printf("non-option arguments: (none)\n");
	}

	return 0;
}
