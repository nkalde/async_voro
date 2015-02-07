#ifndef AUTOMATON_H
#define AUTOMATON_H

#include "cell.h"

class Automaton{
	private:
		Cell** cells;
		int lig;
		int col;
	public:
		Automaton();
		Automaton(int lig,int col);
		void init(int lig,int col);
		~Automaton(){
			for (int i=0;i<lig;i++){
				delete[] this->cells[i];
			}
			delete[] this->cells;
		}

		//getters
		int getLig();
		int getCol();
		Cell * getCellIJ(int i, int j);

		//setters
		void setLig(int lig){this->lig=lig;}
		void setCol(int col){this->col=col;}
		void setCells();

		//update
		void synchronous_update(int function);
		void asynchronous_update(int function);

		//printing
		void printAutomaton(int function);	
};
#endif
