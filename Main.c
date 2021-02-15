#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Assuming that heap is 127 bytes long and memory is byte-addressable
size_t HEAP_SIZE = 127;

int find_free_block(unsigned char *Heap, size_t alloc_size) {
	// Finds a free block of memory using the "first fit" allocation strategy and returns the index of its header

	unsigned char *start = &Heap[0]; // Header address of the first block
	unsigned char *end = &Heap[HEAP_SIZE-1]; // Footer address of the last block
	end -= (*end >> 1) - 1; // Change pointer to address of last block's header
	unsigned char *p = start;

	// While not passed end and already allocated and too small
	while((p < end) && ((*p & 1) || (*p <= alloc_size)))
		p += (*p >> 1); // Go to the next block

	// If a block of requested size is not available.
	if ((*p & 1)) return -1;

	return p - start;
}


int add_block(unsigned char *Heap, size_t payload_size) {
	size_t alloc_size = payload_size + 2; // Additional space for storing header and footer

	if (payload_size <= 0 || alloc_size > HEAP_SIZE) return -1;

	int prev_index_header = find_free_block(Heap, alloc_size);
	if (prev_index_header == -1) return -1;

	int prev_index_footer = prev_index_header + (Heap[prev_index_header] >> 1) - 1;
	int cur_index_header = prev_index_header; // Same index
	int cur_index_footer = cur_index_header + alloc_size - 1;

	// Add header and footer to the new block
	Heap[cur_index_header] = (alloc_size << 1) | 1; // Block size and allocation bit
	Heap[cur_index_footer] = Heap[cur_index_header];
	
	// Set length for the remaining part of the old block
	Heap[cur_index_footer+1] = (prev_index_footer - alloc_size) << 1;
	Heap[prev_index_footer] = Heap[cur_index_footer+1];

	return cur_index_header+1;
}

void blocklist(unsigned char * Heap) {
	// print out blocks in order: pointer to start, payload size, allocation status

	unsigned char *start = &Heap[0]; // Header address of the first block
	unsigned char *end = &Heap[HEAP_SIZE-1]; // Footer address of the last block

	// while start < end, then print out the blocks
	while (start < end) {
		// print each block information:
		// pointer to start, payload size, allocation status
		// start,           *start & -2,   *start & 1

		if (*start & 1 == 1) { // if allocated
			printf("%d, %d, allocated.", start, *start & -2);
		}
		else { // if free
			printf("%d, %d, free.", start, *start & -2);
		}
		
		start += (*start >> 1); // goes to next block
	}

}

void writemem(int index, char * str, unsigned char * Heap) {
	unsigned char *target = &Heap[index];
	int size_of_str = strlen(str);
	*target = size_of_str; // this will be the header to hold the size
	*target = *target | 1; // mark allocation status as 1 by doing OR operation between current heap index and 1
	target++; // go to next address
	int i;
	for (i = 0; i < size_of_str; i++) {
		*target = str[i];

		target++; // this goes to the next address?
	}
	
	*target = size_of_str; // this will be the footer to hold the size
	*target = *target | 1; // mark allocation status as 1 by doing OR operation between current heap index and 1
	
}

void printmem(int index, int num_chars_to_print, unsigned char * Heap) {

}

int main() {
	// The heap is organized as an implicit free list. The heap is initially completely unallocated,
	// so it contains a single free block which is as big as the entire heap. Memory is initialized
	// so that all addresses (other than the header and the footer of the initial free block) contain 0.
	unsigned char *Heap = (unsigned char *) calloc(HEAP_SIZE, 1);

	// The most-significant 7 bits of the header and footer contain size of the block while the LSB indicates allocation bit.
	Heap[0] = HEAP_SIZE << 1; 
	Heap[HEAP_SIZE-1] = HEAP_SIZE << 1;
	
	free(Heap);

	printf("Testing this A4");

	return 0;
}