#ifndef AUTOMATON_H
#define AUTOMATON_H

#include "cell.h"
#include <set>
#include <algorithm>

class Automaton{
	private:
		Cell** cells;
		int lig;
		int col;
		bool betaEnabled;
		bool deltaEnabled;
		bool epsilonEnabled;

	public:

		static const int bis_thick = 0;
		static const int bis_thin = 1;
		static const int aid_min = 2;
		static const int avd_min = 3;
		static const int consensus_id = 4;
		static const int cvd = 5;
		static const int pr = 6;

		Automaton();
		Automaton(int lig,int col);
		Automaton(std::string filename);
		void init(int lig,int col);
		~Automaton(){
			for (int i=0;i<lig;i++){
				delete[] this->cells[i];
			}
			delete[] this->cells;
		}

		//getters
		bool getBeta();
		bool getDelta();
		bool getEpsilon();
		int getLig();
		int getCol();
		Cell * getCellIJ(int i, int j);
		Cell * getCellIJ(std::pair<int,int> coords);

		//setters
		void setBeta(bool beta);
		void setDelta(bool delta);
		void setEpsilon(bool epsilon);
		void setLig(int lig);
		void setCol(int col);
		void setCells();
		void detectPattern(Cell *cell);//to put in cell class in the end with communication with other cells

		//update
		void synchronous_update(int function);
		void asynchronous_update(int function);
		void update_bis_thick(int i, int j);
		void update_bis_thin(int i, int j);
		void update_aid_min(int i, int j);
		void update_avd_min(int i, int j);
		void update_consensus_id(int i, int j);
		void update_cvd(int i, int j);
		void update_pr(int i, int j);
		int cantor(int a, int b);

		//printing
		void printAutomaton(int function);
		void drawAutomaton();
};
#endif
