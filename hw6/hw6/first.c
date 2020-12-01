#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct llblock {
	unsigned long tag;
	long long counter;
	struct llblock *next;
};

long long setsize(struct llblock *head) {
	if(head == NULL) {
		return  -1;
	}

	long long counter = 0;
	struct llblock *cur = head;

	while(cur != NULL) {
		counter = counter + 1;
		cur = cur->next;
	}

	return counter;
}

// If tag is found in one of the blocks in the provided set, then the index of block is returned. Or else, "-1" is returned
long long findblock(struct llblock *head, unsigned long tag) {
	if(head == NULL) {
		return -1;
	}

	struct llblock *cur = head;

	while(cur != NULL) {
		// printf("Current Tag: %lu\n", cur->tag);
		if(cur->tag == tag) {
			return 1;
		}

		cur = cur->next;
	}

	return -1;
}


// Checks the highest counter in the head list
long long maxcount(struct llblock *head) {

	if(head == NULL)
		return -1;

	struct llblock *cur = head;
	long long counter = 0;

	while(cur != NULL) {
		if(cur->counter > counter) {
			counter = cur->counter;
		}

		cur = cur->next;
	}

	return counter;
}

void delete(struct llblock *node) {
	struct llblock *temp;

	while (node != NULL) {
		temp = node;
		node = node->next;
		free(temp);
	}

	return;
}

int main(int argc, char *argv[]) {
	// Error checking to see if all of the arguments passed in are proper.

	// Not enough arguments
	if (argc != 6) {
		printf("error");
		exit(1);
	}

	// Cache size is not a power of 2
	if ((atoi(argv[1]) & (atoi(argv[1]) - 1)) != 0) {
		printf("error");
		exit(1);
	}

	// Check if associativity argument is valid
	if(strstr(argv[2], "assoc:") != NULL) {
		int n = 0;
		sscanf(argv[2], "assoc:%d", &n);

		if ((n & (n - 1)) != 0) {
			printf("error");
			exit(1);
		}
	} else if ((strstr(argv[2], "assoc") == NULL) && (strcmp(argv[2], "direct") != 0)) {
		printf("error");
		exit(1);
	}

	// Check if the replacement policy argument is supported
	if((strcmp(argv[3], "lru") != 0) && (strcmp(argv[3], "fifo") != 0)) {
		printf("error");
		exit(1);
	}

	// Block size is not a power of 2
	if ((atoi(argv[4]) & (atoi(argv[4]) - 1)) != 0) {
		printf("error");
		exit(1);
	}

	int cache_size = atoi(argv[1]);
	int block_size = atoi(argv[4]);
	// printf("Block Size: %d", block_size);
	int num_of_sets = 0;
	int lines_per_set = 0;

	if(strstr(argv[2], "assoc:") != NULL) {
		sscanf(argv[2], "assoc:%d", &lines_per_set);
		// printf("Lines per set: %d\n", lines_per_set);
		num_of_sets = cache_size / (lines_per_set * block_size);
	} else if(strcmp(argv[2], "assoc") == 0) {
		num_of_sets = 1;
		lines_per_set = cache_size / block_size;
		// printf("Lines per set: %d\n", lines_per_set);
	} else if(strcmp(argv[2], "direct") == 0) {
		lines_per_set = 1;
		num_of_sets = cache_size / block_size;
		// printf("Lines per set: %d\n", lines_per_set);
	}

	// Calculating the sizes...

	unsigned long offset_bits = log2(block_size); //block offset
	// printf("Number of Sets: %d\n", num_of_sets);
	unsigned long set_bits = log2(num_of_sets);
	// printf("Set Bits: %ld\n", set_bits);

	unsigned long hits = 0;
	unsigned long misses = 0;
	unsigned long reads = 0;
	unsigned long writes = 0;

	// Creating the 2D Array
	struct llblock *cache[num_of_sets];

	for(int k = 0; k < num_of_sets; k++) {
		cache[k] = NULL;
	}

	// For Least Recently Used

	FILE *fp = fopen(argv[5], "r");
	char mode;
	unsigned long address;
	char u2[25];

	// Opening the trace file:
	while(fscanf(fp, "%s %c %lx\n", u2,  &mode, &address) == 3) {
		// printf("Address: %lx | Mode: %c\n", address, mode);
		unsigned long set = ((address >> offset_bits) & ((1 << set_bits) - 1));
		unsigned long tag = (address >> (offset_bits + set_bits));

		// printf("Tag: %lu Set: %lu\n", tag, set);
		// printf("FindBlock: %lli\n", findblock(cache[set], tag));

		// If the tag is not found in the set
		if (findblock(cache[set], tag) == -1) {

			// printf("Cache miss & Memory read incremented!\n");
			misses = misses + 1;
			reads = reads + 1;

			if (mode == 'W') {
				// printf("Memory write incremented!\n");
				writes = writes + 1;
			}

			// Cold cache....not fully occupied
			if(setsize(cache[set]) < lines_per_set) {
				// printf("The set is empty!\n");

				// Set is empty
				if(setsize(cache[set]) == -1) {
					cache[set] = malloc(sizeof(struct llblock));
					cache[set]->tag = tag;
					cache[set]->counter = 0;
					cache[set]->next = NULL;
				} else { // Set is partially filled
					// printf("The set is partially filled!\n");
					struct llblock *cur = cache[set];

					// traverse till the end of the set and add the block to the end
					while(cur->next != NULL) {
						cur = cur->next;
					}

					long long max = maxcount(cache[set]);
					cur->next = malloc(sizeof(struct llblock));
					cur->next->tag = tag;
					cur->next->next = NULL;

					if (strcmp(argv[3], "lru") == 0) {
						cur->next->counter = max + 1;
					}
				}
			} else { // Set is full :(  Will need to use eviction policy
				// printf("The set is full!\n");

				if (strcmp(argv[3], "lru") == 0) { // Least Recently Used

					// Find the node with the 0 counter and replace its value with the new tag
					struct llblock *cur = cache[set];

					while(cur->next != NULL && cur->counter != 0) {
						cur = cur->next;
					}

					cur->tag = tag;
					cur->counter = maxcount(cache[set]);

					// Decrement all the other counters by one and skip the cur node

					struct llblock *others = cache[set];

					while(others != NULL) {
						if (others != cur) {
							others->counter = others->counter - 1;
						}

						others = others->next;
					}
				} else { // First in First out
					struct llblock *cur = cache[set];
					long long temp = 0;

					while(temp < cache[set]->counter) {
						cur = cur->next;
						temp++;
					}

					// cur should now contain the node which needs to be evicted/swapped

					cur->tag = tag;

					if(cache[set]->counter == (lines_per_set - 1)) {
						cache[set]->counter = 0;
					} else {
						cache[set]->counter++;
					}
				}
			}
		} else { // The tag already exists in the set
			// printf("Cache hit incremented!\n");
			hits = hits + 1;

			if (mode == 'W') {
				// printf("Memory write incremented!\n");
				writes = writes + 1;
			}

			if (strcmp(argv[3], "lru") == 0) {
				// Find the node with the 0 counter and replace its value with the new tag
				struct llblock *cur = cache[set];

				while(cur != NULL && cur->tag != tag) {
					cur = cur->next; // Cur will hold the equivalent tag
				}

				long long max_counter = maxcount(cache[set]);
				struct llblock *cur1 = cache[set];

				while(cur1 != NULL) {
					if(cur1->counter > cur->counter) {
						cur1->counter = cur1->counter - 1;
					}

					cur1 = cur1->next;
				}

				cur->counter = max_counter;

			}

		}

		// printf("-------------\n");
	}

	fclose(fp);

	printf("Memory reads: %lu\n", reads);
	printf("Memory writes: %lu\n", writes);
	printf("Cache hits: %lu\n", hits);
	printf("Cache misses: %lu\n", misses);

	for(long long k = 0; k < num_of_sets; k++) {
		if(cache[k] != NULL) {
			delete(cache[k]);
		}
	}

}
