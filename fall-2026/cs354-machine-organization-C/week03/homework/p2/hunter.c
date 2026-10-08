/*======================================================================
 * PROJECT: Hitori Hunter
 *----------------------------------------------------------------------
 * AUTHOR: Max Hanson
 * EMAIL: mthanson5@wisc.edu
 * ADDITIONAL SOURCES: None
 * FILE: hunter.c
 * COURSE: COMP SCI 354 - Fall 2026
 * INSTRUCTOR: Dahl
 * COPYRIGHT: 2026, Dahl
 * Posting or sharing this file with anyone outside of course staff prohibited.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "hunter.h"

/*
 * Entry point for this Hitori Hunter application.  This program can be used to
 * solve hitori puzzles that are encoded as plain text files.
 *
 * This program takes 1 command line arguments specifying the filename of the
 * puzzle that you would like this program to solve.
 */
int main(int argc, char** argv) {

	// use command line arguments to load board from plain text file
	if( argc != 2)
		error("Hitori Hunter usage: hunter PUZZLEFILE\n");
	char* filename = argv[1];
	int** puzzle;
	int size = loadPuzzle(filename,&puzzle);

	// transfer loaded board into a 1D array of Cells
	Cell solution[size * size];
	transferGrid(puzzle,solution,size);

	// make as much progress as possible toward solving this puzzle
	solvePuzzle(solution,size);

	// output progress toward solution to <file>.sln (length: + ".sln" + '\0')
	char solutionFilename[strlen(filename)+4+1];
	strcpy(solutionFilename,filename);
	strcat(solutionFilename,".sln");
	saveSolution(solution,size,solutionFilename);

	return 0;
}

/*
 * Helper function to more concisely report error messages through stderr
 * before ending the program with a return status of 1 (rather than 0).
 *
 * Arguments: Similar to printf, you can pass this method a format string
 * followed by a list of values to fill that string's placeholders.
 */
void error(char* format, ...) {
	va_list args;
	va_start(args,format);
	fprintf(stderr,"ERROR: ");
	vfprintf(stderr,format,args);
	exit(1);
}

/*
 * Loads Hitori puzzle from plain text file, and stores its contents into
 * a heap-allocated 2d int array.
 * 
 * The first line of this plain text file must contains a single number
 * defining the size of the square puzzle (between 1 and MAX_BOARD_SIZE).
 * Each subsequent line contains one row of comma separate numbers (as
 * many as are dedfined by the previously read size).
 */
int loadPuzzle(char* filename, int*** puzzle) {
	FILE* fin = fopen(filename,"r");
	if(fin == NULL)
		error("Unable to open file for reading: %s\n",filename);

	// reads board size
	char buffer[MAX_LINE_WIDTH];
	if(fgets(buffer,MAX_LINE_WIDTH,fin) == NULL)
		error("Unable to read first line of file: %s\n",filename);
	int size = atoi(buffer);
	if(size < 1 || size > MAX_BOARD_SIZE)
		error("Unable to read valid board size from file: %s\n",buffer);

	// allocate memory for entire puzzle grid
	int** grid = malloc(sizeof(int*)*size);
	if(grid == NULL)
		error("Unable to allocate memory for grid rows.\n");
	for(int i=0;i<size;i++) {
		grid[i] = malloc(sizeof(int)*size);
		if(grid[i] == NULL)
			error("Unable to allocate memory for grid columns.\n");
	}
	
	// read board contents
	int row = 0;
	while(fgets(buffer,MAX_LINE_WIDTH,fin) != NULL && row < size) {
		char* token;
		int col = 0;
		for(token = strtok(buffer,",");token != NULL;token = strtok(NULL,",")){
			grid[row][col] = atoi(token);
			if(grid[row][col] < 1 || grid[row][col] > MAX_BOARD_SIZE)
				error("Unable to read column number: %s\n",token);
			col++;
		}
		if(col != size) error("Not enough columns in row: %s\n",buffer);
		row++;
	}
	if(row != size) error("Not enough rows in board: %i/%i\n",row,size);

	fclose(fin);
	*puzzle = grid;
	return size;
}

/*
 * Transfers 2D heap-allocated int array into a 1D stack-allocated Cell
 * array.  The heap memory is then freed, after it has been copied..
 */
void transferGrid(int** puzzle, Cell* solution, int size) {
	for(int i=0;i<size;i++) {
		for(int j=0;j<size;j++) {
			// flatten index
			int idx = (i * size) + j;
			solution[idx] = (Cell)puzzle[i][j];
		}
	}

	// free memory allocated for puzzle
	for(int i=0;i<size;i++) {
    	free(puzzle[i]);
	}
	free(puzzle);
}

/*
 * Saves solution progress to the specified file.  Circled cells are saved
 * as the circled number, blacked out cells are saved as #'s, and all other
 * cells are saved as ?'s.
 */
void saveSolution(Cell* solution, int size, char* filename) {
	FILE* fout = fopen(filename,"w");
	if(fout == NULL)
		error ("Unable to open file for writing: %s\n",filename);

	fprintf(fout,"%i\n",size);
	for(int i=0;i<size;i++) {
		for(int j=0;j<size;j++) {
			Cell value = solution[(i * size) + j];
			
			int isCircle = value & CIRCLE;
			int isBlackedOut = value & BLACK;
			int cellNumber = NUMBER(value);
			if(isCircle)
				fprintf(fout,"%i",cellNumber);
			else if(isBlackedOut)
				fprintf(fout,"#");
			else
				fprintf(fout,"?");
			if(j < size-1) fprintf(fout,",");
		}
		fprintf(fout,"\n");
	}
	fclose(fout);
}

/*
 * Black out a cell.  Circled cells are saved
 * as the circled number, blacked out cells are saved as #'s, and all other
 * cells are saved as ?'s.
 */
void blackout(Cell* solution, int size, int row, int column) {
	int idx = (row * size) + column;
	Cell currentCell = solution[idx];
	// return if cell is blacked out
	if (currentCell & BLACK) {
		return;
	}
	// error if cell is circled
	if (currentCell & CIRCLE) {
		error("Error: Cannot black out circled cell at row %d, column &d.\n", row, column);
	}
	// bitwise OR to blackout cell
	solution[idx] |= BLACK;
	// invloke circle on valid orthogonal neighbors
	if (row - 1 >= 0) {
		circle(solution, size, row - 1, column);
	}
	if (row + 1 < size) {
		circle(solution, size, row + 1, column);
	}
	if (column - 1 >= 0) {
		circle(solution, size, row, column - 1);
	}
	if (column + 1 < size) {
		circle(solution, size, row, column + 1);
	}
}

/*
 * Circle a cell.  Circled cells are saved
 * as the circled number, blacked out cells are saved as #'s, and all other
 * cells are saved as ?'s.
 */
void circle(Cell* solution, int size, int row, int column) {
	int idx = (row * size) + column;
	Cell currentCell = solution[idx];
	// return if cell is circled
	if (currentCell & CIRCLE) {
		return;
	}
	// error if cell is blacked out
	if (currentCell & BLACK) {
		error("Error: Cannot circle blacked out cell at row %d, column &d.\n", row, column);
	}
	// bitwise OR to circle cell
	solution[idx] |= CIRCLE;
	// get target number
	int targetNumber = NUMBER(currentCell);

	for (int c=0;c<size;c++) {
        // skip cell we circled
        if (c == column) continue;

        int rowIndex = (row * size) + c;
        Cell rowCell = solution[rowIndex];

        // if it shares the same number and isn't already blacked out, black it out
        if (NUMBER(rowCell) == targetNumber && !(rowCell & BLACK)) {
            blackout(solution, size, row, c);
        }
    }

	for (int r=0;r<size;r++) {
        // skip cell we circled
        if (r == row) continue;

        int colIndex = (r * size) + column;
        Cell colCell = solution[colIndex];

        // If it shares the same number and isn't already blacked out, black it out
        if (NUMBER(colCell) == targetNumber && !(colCell & BLACK)) {
            blackout(solution, size, r, column);
    	}
	}
}

/*
 * Calls helper methods to solve the Hitori.
 */
void solvePuzzle(Cell* solution, int size) {
	makeSandwichDeductions(solution, size);
	makeDoubletDeductions(solution, size);

}

/*
 * If a cell has two neighbors which sandwhich the cell,
 * and the neighbors have the same number, then one of the
 * neighbors must be black, and therefor the sandwhiched
 * cell cannot itself be black. 
 */
void makeSandwichDeductions(Cell* solution, int size) {
	// return if size not at least three
	if (size < 3) return;

	for (int r=0;r<size;r++) {
		for (int c=0;c<size;c++) {
			
			// check horizontal neighbors
			if (c > 0 && c + 1 < size) {
				int leftIndex = (r * size) + (c - 1);
				int rightIndex = (r * size) + (c + 1);

				if (NUMBER(solution[leftIndex]) == NUMBER(solution[rightIndex])) {
					circle(solution, size, r, c);
				}
			}

			// check vertical neighbors
			if (r > 0 && r + 1 < size) {
				int topIndex = ((r - 1) * size) + c;
				int bottomIndex = ((r + 1) * size) + c;

				if (NUMBER(solution[topIndex]) == NUMBER(solution[bottomIndex])) {
					circle(solution, size, r, c);
				}
			}
		}
	}
}

/*
 * If the puzzle has two adjacent cells that are equal,
 * and if there are other cells in the same row or column
 * that have the same number, then the other cells must be
 * black, otherwise the two adjacent cells would result
 * in neighboring black cells.
 */
void makeDoubletDeductions(Cell* solution, int size) {
	// horizontal doublets
	for (int r=0;r<size;r++) {
        for (int c=0;c<size-1;c++) {
            int leftIndex  = (r * size) + c;
            int rightIndex = (r * size) + (c + 1);

            int leftVal  = NUMBER(solution[leftIndex]);
            int rightVal = NUMBER(solution[rightIndex]);

            // if doublets
            if (leftVal == rightVal) {
                // black out every other cell in row with the same number
                for (int scanC=0;scanC<size;scanC++) {
                    // skip the two doublet cells
                    if (scanC == c || scanC == c + 1) continue;

                    int scanIndex = (r * size) + scanC;
                    if (NUMBER(solution[scanIndex]) == leftVal) {
                        blackout(solution, size, r, scanC);
                    }
                }
            }
        }
    }

	// vertical doublets
    for (int c=0;c<size;c++) {
        for (int r=0;r<size-1;r++) {
            int topIndex    = (r * size) + c;
            int botttomIndex = ((r + 1) * size) + c;

            int topVal    = NUMBER(solution[topIndex]);
            int bottomVal = NUMBER(solution[botttomIndex]);

            // if doublets
            if (topVal == bottomVal) {
                // black out every other cell in column with the same number
                for (int scanR=0;scanR<size;scanR++) {
                    // skip the two doublet cells
                    if (scanR == r || scanR == r + 1) continue;

                    int scanIndex = (scanR * size) + c;
                    if (NUMBER(solution[scanIndex]) == topVal) {
                        blackout(solution, size, scanR, c);
                    }
                }
            }
        }
    }
}

// Step 2.7. Optional: For Extra Credit
// Implement function that makes gate deductions here.

// EOF -----------------------------------------------------------------
