#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

// Define a file struct to represent files in the file system
struct File {
    string name;
    int size;
    int firstBlock;
};

// Define a block struct to represent a block of disk space
struct Block {
    bool used;
    int nextBlock;
};

class FileSystem {
public:
    // Constructor to initialize the disk space and allocation algorithm
    FileSystem(int diskSize, string allocationAlgorithm) {
        this->diskSize = diskSize;
        this->allocationAlgorithm = allocationAlgorithm;
        this->disk = vector<Block>(diskSize, Block{false, -1});
    }

    // Function to create a new file and allocate disk space for it
    void createFile(string name, int size) {
        if (fileMap.find(name) != fileMap.end()) {
            cout << "File already exists" << endl;
            return;
        }

        // Calculate the number of blocks needed for the file
        int numBlocks = (size + blockSize - 1) / blockSize;

        // Find the first block in the free list and allocate it to the file
        int firstBlock = findFreeBlocks(numBlocks);
        if (firstBlock == -1) {
            cout << "Not enough free space" << endl;
            return;
        }

        // Mark the allocated blocks as used and update the free list
        for (int i = 0; i < numBlocks; i++) {
            disk[firstBlock + i].used = true;
            if (i < numBlocks - 1) {
                disk[firstBlock + i].nextBlock = firstBlock + i + 1;
            } else {
                disk[firstBlock + i].nextBlock = -1;
            }
        }
        updateFreeList();

        // Create a new file object and add it to the file map
        File file = {name, size, firstBlock};
        fileMap[name] = file;

        cout << "File created: " << name << endl;
    }

    // Function to delete a file and deallocate its disk space
    void deleteFile(string name) {
        if (fileMap.find(name) == fileMap.end()) {
            cout << "File does not exist" << endl;
            return;
        }

        // Get the file object and free its allocated blocks
        File file = fileMap[name];
        int block = file.firstBlock;
        while (block != -1) {
            disk[block].used = false;
            int nextBlock = disk[block].nextBlock;
            disk[block].nextBlock = freeList;
            freeList = block;
            block = nextBlock;
        }

        // Remove the file from the file map
        fileMap.erase(name);

        cout << "File deleted: " << name << endl;
    }

    // Function to rename a file
    void renameFile(string oldName, string newName) {
        if (fileMap.find(oldName) == fileMap.end()) {
            cout << "File does not exist" << endl;
            return;
        }
        if (fileMap.find(newName) != fileMap.end()) {
            cout << "File already exists" << endl;
            return;
        }

        // Update the file map with the new name
        File file = fileMap[oldName];
        file.name = newName;
        fileMap.erase(oldName);
        fileMap[newName] = file;

        cout << "File renamed from " << oldName << " to " << newName << endl;
    }

    // Function to move a file to a
