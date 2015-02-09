#include "automaton.h"

Automaton::Automaton(int lig, int col){
	this->init(lig,col);
}

Automaton::Automaton() : Automaton(0,0){}

void Automaton::init(int lig, int col){
	this->setLig(lig);
	this->setCol(col);
	this->setCells();
	this->setBeta(true);
	this->setDelta(true);
	this->setEpsilon(true);
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
			this->getCellIJ(i,j)->init(i*col+j+1,i,j);
			this->getCellIJ(i,j)->setVonNeumannN(&neighbors);
			std::cerr << i << "/"<< j << std::endl;	
		}
	}

	for (int i=0;i<lig;i++){
		for (int j=0; j<col;j++){
			//this->getCellIJ(i,j)->setPattern(...);
			this->detectPattern(this->getCellIJ(i,j));			
		}
	}
}

//getters
bool Automaton::getBeta(){return this->betaEnabled;}
bool Automaton::getDelta(){return this->deltaEnabled;}
bool Automaton::getEpsilon(){return this->epsilonEnabled;}
int Automaton::getLig(){return this->lig;}
int Automaton::getCol(){return this->col;}
Cell * Automaton::getCellIJ(int i, int j){return &this->cells[i][j];}

//setters
void Automaton::setBeta(bool beta){this->betaEnabled=beta;}
void Automaton::setDelta(bool delta){this->deltaEnabled=delta;}
void Automaton::setEpsilon(bool epsilon){this->epsilonEnabled=epsilon;}
void Automaton::setLig(int lig){this->lig=lig;}
void Automaton::setCol(int col){this->col=col;}
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

void Automaton::detectPattern(Cell *cell){//to put in cell class in the end with communication with other cells
	bool patternFound=false;
	std::vector<std::pair<int,int> > *vN = cell->getVonNeumannN();
	std::vector<bool> localPat = {false,false,false,false,false};

	//if (cell->isSite()){
	localPat[0] = cell->isSite();

	for (int k=0; k<vN->size();k++){	
		if (vN->at(k).first != -1){
			std::pair<int,int> vNK = vN->at(k);
			localPat[k+1] = (this->getCellIJ(vNK.first,vNK.second))->isSite();//core dumped segfault
		}

	}
	std::vector<int> pat_Cl_Nb = Cell::patternsMap[localPat];
	cell->setPattern(&pat_Cl_Nb);
	//}
}

void Automaton::update_pr(int i,int j){
//Distance and Semantic layers
//def Pr(i,j,automatonCS,automatonNS,idPr=Cell.getIDInit):
	Cell * d= this->getCellIJ(i,j);
	if (d->isSite()){
		//TODO NEXT STATE AUTOMATON
		//d=automatonNS[i][j]
		Cell *d = this->getCellIJ(i,j);
		d->setD1(0);
	}
	else{
		//if (monoUpdate and d->getIDInit() == d->getID() or not monoUpdate)
		//ALPHA CONDITION neighbors
		std::vector<std::pair<int,int> >* dVNN = d->getVonNeumannN();
		std::vector<std::pair<int,int> >* alpha;
		std::vector<int>* alphaN;
		for (int i=0; i<dVNN->size();i++)
		{
			if (dVNN->at(i).first != -1){
				alpha->push_back(dVNN->at(i));
				alphaN->push_back(i);
			}
		}

		//BETA CONDITION sites and non-sites modified
		std::vector<std::pair<int,int> >* beta;
		std::vector<int>* betaN;
		for (int i=0; i<alpha->size();i++){
			int fst = alpha->at(i).first;
			int snd = alpha->at(i).second;
			//if (this->getCellIJ(fst,snd)->isSite() or (! this->getCellIJ(fst,snd)->isSite() and this->getCellIJ(fst,snd)->getIDInit() != this->getCellIJ(fst,snd)->getID()))
			//{
			//	beta->push_back(alpha->at(i));
			//	betaN->push_back(alphaN->at(i));
			//}
		}
		if (betaEnabled==false){
			beta = alpha;
			betaN = alphaN;
		}
	
		//DELTA CONDITION minDist
		std::vector<std::pair<int,int> >* delta;
		std::vector<int>* deltaN;
		std::vector<int>* d1N;
		int minD1 = -1;
		for (int i=0; i<beta->size();i++){
			int fst = beta->at(i).first;
			int snd = beta->at(i).second;
			d1N->push_back(this->getCellIJ(fst,snd)->getD1());
			if (minD1==-1 or minD1>d1N->at(i)){
				minD1 = d1N->at(i);
			}
		}
		if (d1N->size() > 0){
			for (int i=0;i<beta->size();i++){
				int fst = beta->at(i).first;
				int snd = beta->at(i).second;
				if (this->getCellIJ(fst,snd)->getD1() == minD1){
					delta->push_back(beta->at(i));
					deltaN->push_back(betaN->at(i));
				}
			}
		}
		if (deltaEnabled==false){
			delta=beta;
			deltaN=betaN;
		}
		
		//EPSILON CONDITION minContribution
		std::vector<std::pair<int,int> >* epsilon;
		std::vector<int>* epsilonN;
		if (delta->size() > 0){
			std::unordered_set<int>* idDelta;
			std::vector<int>* lenIdDelta;
			int minIdDelta = -1;
			for (int i=0;i<delta->size();i++){
				int fst = delta->at(i).first;
				int snd = delta->at(i).second;
				//idDelta->insert(this->getCellIJ(fst,snd)->getID());
				//lenIdDelta->push_back(this->getCellIJ(fst,snd)->getID()->size());
				if (minIdDelta==-1 or minIdDelta >lenIdDelta->at(i)){
					minIdDelta = lenIdDelta->at(i);
				}
			}
			//print('min id delta'+str(minIdDelta))
			std::vector<std::pair<int,int> >* epsilon;
			for (int i=0;i<delta->size();i++){
				int fst = delta->at(i).first;
				int snd = delta->at(i).second;
				//if(this->getCellIJ(fst,snd)->getID()->size() == minIdDelta){
				//	epsilon->push_back(delta->at(i));
				//	epsilonN->push_back(deltaN->at(i));	
				//}
			}
		}
		if (epsilonEnabled==false){
			epsilon=delta;
			epsilonN=deltaN;
		
		}
		//if epsilon->size() > 0)
			//print('before',d.getID())
		
		std::unordered_set<int>* idS;

		for(int i=0; i< epsilonN->size();i++){
			// k in epsilonN:
			int fst = dVNN->at(i).first;
			int snd = dVNN->at(i).second;
			Cell* cellN = this->getCellIJ(fst,snd);	
			idS->insert(cellN->getID((i+2)%4)->begin(),cellN->getID((i+2)%4)->end());
			
			//TODO NEXT STATE AUTOMATON
			//d=automatonNS[i][j]
			//d.setID(idS)
			//d.setD1(minD1+1)
			//this->getCellIJ(i,j)->setID(idS);
			this->getCellIJ(i,j)->setD1(minD1+1);
			//print('\t\t'+str(n)+' '+str(cellN.getID()))
		}	
		//if (epsilon->size() > 0)
			//print('after',d.getID())
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
					break;
				case(5)://pattern
					this->getCellIJ(i,j)->printPattern();
				default:
					break;
			}
		}
		std::cerr << std::endl;
	}
}
