#ifndef CELL_H
#define CELL_H

#include <limits>
#include <utility>
#include <unordered_set>
#include <string.h>
#include <vector>
#include <iostream>
#include <map>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class Cell
{
	private:
		//sizes
		static const size_t size_neighbors = 4;
		static const size_t size_pattern = 3;
		static const size_t size_ids = 5;
		static const size_t size_conflict = 1;

		//row i, column j
		int i;
		int j;

		//neighbors
		std::vector< std::pair<int,int> > neighbors;

		//site
		bool site;

		//distance
		int d1;

		//bis	
		int bis;

		//pattern
		std::vector<int> pattern;

		//direction dis
		std::vector<std::unordered_set<int> > ids;
		int conflict;

		//identifiers
		int idInit; //intrinsic
		//initial
		std::unordered_set<int> idI;
		//computed
		std::unordered_set<int> idC;
		//area
		std::unordered_set<int> idA;

	public:
		//print constant
		static const int print_distance = 0;
		static const int print_site = 1;
		static const int print_identifier_init = 2;
		static const int print_identifier_comp = 6;
		static const int print_identifier_area = 7;
		static const int print_identifier_line = 8;
		static const int print_bisector = 3;
		static const int print_neighbors = 4;
		static const int print_pattern = 5;

		//classes
		static const std::vector<std::vector<bool> > class1;
		static const std::vector<std::vector<bool> > class2;
		static const std::vector<std::vector<bool> > class3;
		static const std::vector<std::vector<bool> > class4;
		static const std::vector<std::vector<bool> > class5;
		static const std::vector<std::vector<bool> > class6;
		static const std::vector<std::vector<bool> > class7;
		static const std::vector<std::vector<std::vector<bool> > > classes;
		static std::map<std::vector<bool>,std::vector<int> > patternsMap;
		static std::map<std::vector<bool>,std::vector<int> > computePatternMap();

		Cell();
		Cell(int idInit, int i, int j);		
		void init(int idInit, int i, int j);
		void nothing();

		/////////
		//getters
		/////////

		//integer : row i on the grid
		int getI();

		//integer : column j on the grid
		int getJ();

		//integer 2-tuple list : von neumann *neighbors (i,j)
		const std::vector< std::pair<int,int> >& getVonNeumannN();

		//boolean : site state
		bool isSite();

		//integer : distance
		int getD1();

		//integer : bisector state
		int getBisector();

		//integer 3 list :  class number, pattern number, pattern count
		const std::vector<int>& getPattern();

		int getConflict();

		//integer set : set of cell id
		const std::unordered_set<int>& getID(int default_value=4);
		const std::unordered_set<int>& getIDI();
		const std::unordered_set<int>& getIDA();
		int getIDInit();

		/////////
		//setters
		/////////
		void setI(int i);
		void setJ(int j);
		void setNeighborI(int i, std::pair<int,int> neighbor);
		void setVonNeumannN(std::vector< std::pair<int,int> >* neighbors);
		void setSite(bool site);
		void setD1(int d1);
		void setBisector(int bis);
		void setPattern(std::vector<int>* pattern);
		void setConflict(int conflict);
		void setID(std::unordered_set<int>* idI_p, int default_value=5);
		void setIDA(std::vector<int>* idA_p);
		void setIDInit(int idInit);
		void detectPattern();
		void update(int function);

		///////
		//print
		///////
		void printNeighbors();
		void printCell();
		void printPatternsMap();
		void printPattern();
		void printID();
		void printID_line();
};

#endif