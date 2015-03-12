#ifndef CELLS_H
#define CELLS_H

#include cell.h

class Cells
{
	Cell[] cellsIJ;
	int rows;
	int cols;
	int[] occArray;
	int[] navArray;
	int[] interArray;
public:
	Cells();
	Cell* getCellsIJ();
	updateOccArray(cell_type,int rows,int cols,int* intArray)
	updateNavArray();
	compute();
};

#endif
