#include "automaton.h"

Automaton::Automaton(int lig, int col){
	this->init(lig,col);
}

Automaton::Automaton() : Automaton(0,0){}

void Automaton::init(int lig, int col){
	this->setLig(lig);
	this->setCol(col);
	this->setCells();
}

void Automaton::setCells(){
	this->cells = new Cell*[lig];
	for (int i=0;i<lig;i++){
		this->cells[i] = new Cell[col];	
		for (int j=0; j<col;j++){

			//add neighbors
			std::vector<std::pair<int,int>> neighbors(4);
			neighbors.assign(4,std::make_pair(-1,-1));
			if (i>0)//north
				neighbors[0] = std::make_pair(i-1,j);
			if (j>0)//west
				neighbors[3] = std::make_pair(i,j-1);
			if (j<col-1)//east
				neighbors[1] = std::make_pair(i,j+1);
			if (i<lig-1)//south
				neighbors[2] = std::make_pair(i+1,j);
			this->cells[i][j].init(i*col+j+1,i,j);
			this->cells[i][j].setVonNeumannN(&neighbors);	
			
			//add pattern
			//this->cells[i][j].setPattern();
		}
	}
}

//getters
int Automaton::getLig(){return this->lig;}
int Automaton::getCol(){return this->col;}
Cell * Automaton::getCellIJ(int i, int j){return &this->cells[i][j];}

//setters
void Automaton::synchronous_update(int function){}

void Automaton::asynchronous_update(int function){
	int cnt = 0;
	while(true){
		//random execution
		int randI = rand() % this->getLig();
		int randJ = rand() % this->getCol();
		Cell *randCell = getCellIJ(randI,randJ);
		randCell->update(function);
		cnt++;
		if (cnt%(this->lig*this->col) == 0)
			return;
	}
}

void Automaton::printAutomaton(int function){
	std::cerr << "\nAutomaton" << std::endl;
	std::cerr << "\tlig" << this->getLig() << "\tcol" << this->getCol() << std::endl;

	for (int j =0; j<this->getCol();j++){
		std::cerr << "\t" << j;
	}
	std::cerr << std::endl;
	for (int i=0; i<this->getLig();i++){
		std::cerr << i;
		for (int j=0; j<this->getCol();j++){	
			std::cerr << "\t";
			switch(function){
				case(0)://distance
					std::cerr << this->getCellIJ(i,j)->getD1();
					break;
				case(1)://site
					std::cerr << this->getCellIJ(i,j)->isSite();
					break;
				case(2)://id init	
					std::cerr << this->getCellIJ(i,j)->getIDInit();
					break;
				case(3)://bisector
					std::cerr << this->getCellIJ(i,j)->getBisector();
					break;
				case(4)://neighbors
					this->getCellIJ(i,j)->printNeighbors();
				default:
					break;
			}
		}
		std::cerr << std::endl;
	}
}
