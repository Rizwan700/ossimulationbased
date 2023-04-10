#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME_DEN 50
#define MAX_FILES 100
#define BLOCK_SIZE 512
#define NUM_BLOCKS 1000

struct file {
    char filename[MAX_FILENAME_DEN];
    int start_block;
    int num_blocks;
};
int num_files = 0;
struct file files[MAX_FILES];

int allocated_blocks[NUM_BLOCKS] = {0};

void print_files() {
    printf("Files:\n");
    for (int i = 0; i < num_files; i++) {
        printf("%s - start block: %d, num blocks: %d\n", files[i].filename, files[i].start_block, files[i].num_blocks);
    }
}

void create_file(char* filename, int file_size) {
    if (num_files >= MAX_FILES) {
        printf("Maximum number of files reached!\n");
        return;
 }

    // Find contiguous free blocks
    int num_needed_blocks = (file_size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    int start_block = -1;
    int num_consecutive_blocks = 0;
    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (allocated_blocks[i] == 0) {
            if (start_block == -1) {
                start_block = i;
            }
            num_consecutive_blocks++;
            if (num_consecutive_blocks == num_needed_blocks) {
                break;
            }
        } else {
            start_block = -1;
            num_consecutive_blocks = 0;
        }
    }
   // Check if there are enough free blocks
    if (start_block == -1) {
        printf("Not enough free space to create file!\n");
        return;
    }

    // Allocate blocks
    for (int i = start_block; i < start_block + num_needed_blocks; i++) {
        allocated_blocks[i] = 1;
    }

    // Add file to files array
    struct file new_file;
    strncpy(new_file.filename, filename, MAX_FILENAME_DEN);
    new_file.start_block = start_block;
    new_file.num_blocks = num_needed_blocks;
    files[num_files++] = new_file;

    printf("File created: %s\n", filename);
}
void delete_file(char* filename) {
    int file_index = -1;
    for (int i = 0; i < num_files; i++) {
        if (strcmp(files[i].filename, filename) == 0) {
            file_index = i;
            break;
        }
    }

    if (file_index == -1) {
        printf("File not found!\n");
        return;
// Free blocks
    for (int i = files[file_index].start_block; i < files[file_index].start_block + files[file_index].num_blocks; i++) {
        allocated_blocks[i] = 0;
    }

    // Remove file from files array
    for (int i = file_index; i < num_files - 1; i++) {
        files[i] = files[i + 1];
    }
    num_files--;

    printf("File deleted: %s\n", filename);
}
int main() {
    // Initialize file system
    for (int i = 0; i < NUM_BLOCKS; i++) {
        allocated_blocks[i] = 0;
    }

    // Create files
    create_file("document1.docx", 1024);
    create_file("image1.png", 2048);
    create_file("spreadsheet1.xlsx", 1536);

    // Print initial files
    print_files();

    // Delete a file
    delete_file("image1.png");

    // Print remaining files
    print_files();

    // Create a new file
    create_file("text1.txt", 512);

    // Print updated files
    print_files();
 return 0;
}