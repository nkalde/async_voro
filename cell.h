#ifndef CELL_H
#define CELL_H

#include <limits>
#include <utility>
#include <unordered_set>
#include <string.h>
#include <vector>

class Cell
{
	private:
		//row i, column j
		int i;
		int j;

		//neighbors
		std::vector< std::pair<int,int> > neighbors;//[4];

		//site
		bool site;

		//distance
		int d1;

		//bis	
		int bis;

		//pattern
		std::vector<int> pattern;//[3];

		//direction dis
		std::vector< std::unordered_set<int> > ids;//[5];//5
		std::vector<int> conflict;//[1];

		//identifiers
		int idInit; //intrinsic
//initial
		std::unordered_set<int> idI;
//computed
		std::unordered_set<int> idC;
//area
		std::unordered_set<int> idA;

	public:
		Cell();
		Cell(int idI, int i, int j);		
		void nothing();

		/////////
		//getters
		/////////

		//integer : row i on the grid
		int getI();

		//integer : column j on the grid
		int getJ();

		//integer 2-tuple list : von neumann *neighbors (i,j)
		std::vector< std::pair<int,int> >* getVonNeumannN();

		//boolean : site state
		bool isSite();

		//integer : distance
		int getD1();

		//integer : bisector state
		int getBisector();

		//integer 3 list :  class number, pattern number, pattern count
		std::vector<int>* getPattern();

		std::vector<int>* getConflict();

		//integer set : set of cell id
		std::unordered_set<int>* getID(int default_value);
		std::unordered_set<int>* getIDI();
		std::unordered_set<int>* getIDA();
		int getIDInit();

		/////////
		//setters
		/////////

		void setI(int i);
		void setJ(int j);
		void setVonNeumannN(std::vector< std::pair<int,int> >* neighbors);
		void setSite(bool site);
		void setD1(int d1);
		void setBisector(int bis);
		void setPattern(std::vector<int>* pattern);
		void setConflict(std::vector<int>* conflict);
		void setID(std::vector<int>* idI_p, int default_value);
		void setIDA(std::vector<int>* idA_p);
};

#endif
