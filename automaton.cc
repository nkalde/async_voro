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

//TODO
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
	Cell * d= this->getCellIJ(i,j);
	if (d->isSite()){
		d->setD1(0);
	}
	else{
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
			if (this->getCellIJ(fst,snd)->isSite() or (! this->getCellIJ(fst,snd)->isSite() and this->getCellIJ(fst,snd)->getIDI() != this->getCellIJ(fst,snd)->getID()))
			{
				beta->push_back(alpha->at(i));
				betaN->push_back(alphaN->at(i));
			}
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
				idDelta->insert(this->getCellIJ(fst,snd)->getID()->begin(),this->getCellIJ(fst,snd)->getID()->end());
				lenIdDelta->push_back(this->getCellIJ(fst,snd)->getID()->size());
				if (minIdDelta==-1 or minIdDelta >lenIdDelta->at(i)){
					minIdDelta = lenIdDelta->at(i);
				}
			}
			//print('min id delta'+str(minIdDelta))
			std::vector<std::pair<int,int> >* epsilon;
			for (int i=0;i<delta->size();i++){
				int fst = delta->at(i).first;
				int snd = delta->at(i).second;
				if(this->getCellIJ(fst,snd)->getID()->size() == minIdDelta){
					epsilon->push_back(delta->at(i));
					epsilonN->push_back(deltaN->at(i));	
				}
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
			d->setID(idS);
			d->setD1(minD1+1);
			//print('\t\t'+str(n)+' '+str(cellN.getID()))
		}	
		//if (epsilon->size() > 0)
		//print('after',d.getID())
	}
}


//bisector
void Automaton::update_bis_thick(int i,int j){//,automatonCS,automatonNS){
	Cell * d= this->getCellIJ(i,j);
	//C0 von Neumann neighbors	
	std::vector<std::pair<int,int> >* dVNN = d->getVonNeumannN();
	std::vector<std::pair<int,int> > *c0;
	for (int i=0; i<dVNN->size();i++)
	{
		if (dVNN->at(i).first != -1){
			c0->push_back(dVNN->at(i));
		}
	}

	//C1 id different
	std::vector<std::pair<int,int> >* inter;
	for (int i=0; i<c0->size();i++){
		int fst = c0->at(i).first;
		int snd = c0->at(i).second;
		if (this->getCellIJ(fst,snd)->getID() != d->getID()){
			inter->push_back(c0->at(i));
		}
	}
	bool c1 = inter->size() > 0 and not d->isSite();

	d->setBisector(0);
	if (c1)
		d->setBisector(1);
}

void Automaton::update_bis_thin(int i, int j){//,automatonCS,automatonNS){
	Cell * d= this->getCellIJ(i,j);

	//C0 von Neumann neighbors
	std::vector<std::pair<int,int> >* dVNN = d->getVonNeumannN();
	std::vector<std::pair<int,int> > *c0;
	for (int i=0; i<dVNN->size();i++)
	{
		if (dVNN->at(i).first != -1){
			c0->push_back(dVNN->at(i));
		}
	}

	//C1 multiple ids
	std::unordered_set<int>* dID = d->getID();
	bool c1 = dID->size() > 1;

	//C2 one id
	std::vector<std::pair<int,int> >* siteNeighbors;
	for (int i=0; i< c0->size();i++){
		int fst = c0->at(i).first;
		int snd = c0->at(i).second;
		if (this->getCellIJ(fst,snd)->isSite()){
			siteNeighbors->push_back(c0->at(i));
		}
	}

	std::vector<std::pair<int,int> >* inter;
	for (int i=0; i< c0->size();i++){
		int fst = c0->at(i).first;
		int snd = c0->at(i).second;
		if ((this->getCellIJ(fst,snd)->getID()->size() == 1) and (dID != this->getCellIJ(fst,snd)->getID())){
			inter->push_back(c0->at(i));	
		}
	}
	bool c2 = dID->size() <= 1 and inter->size() > 0 and not d->isSite();
	bool c2p = c2 and siteNeighbors->size() < 1;

	c2=c2p;
	d->setBisector(0);
	if (c1)
		d->setBisector(1);
	if (c2)
		d->setBisector(2);
}

//Area Identification
void Automaton::update_aid_min(int i,int j){
	Cell * d= this->getCellIJ(i,j);
	//C0 von Neumann neighbors
	std::vector<std::pair<int,int> >* dVNN = d->getVonNeumannN();
	std::vector<std::pair<int,int> > *c0;
	for (int i=0; i<dVNN->size();i++)
	{
		if (dVNN->at(i).first != -1){
			c0->push_back(dVNN->at(i));
		}
	}

	std::vector<std::pair<int,int> > *sites;
	for (int i=0; i<c0->size();i++){
		int fst= c0->at(i).first;
		int snd= c0->at(i).second;
		if (this->getCellIJ(fst,snd)->isSite()){
			sites->push_back(c0->at(i));
		}
	}

	//C1 multiple ids
	bool c1 = d->isSite() and sites->size() > 0;
	if (c1){
		std::unordered_set<int>* idS;
		idS->insert(d->getID()->begin(),d->getID()->end());
		for (int i=0;i<sites->size();i++){
			int fst = sites->at(i).first;
			int snd = sites->at(i).second;
			idS->insert(this->getCellIJ(fst,snd)->getID()->begin(),this->getCellIJ(fst,snd)->getID()->end());
		}
		std::unordered_set<int>::iterator iter = std::min_element(idS->begin(),idS->end());
		int idSMin = *iter;
		idS->clear();
		idS->insert(idSMin);
		d->setID(idS);
	}
}

//tzionas
void Automaton::update_consensus_id(int i, int j){
	Cell *d = this->getCellIJ(i,j);
	Cell *d2 = this->getCellIJ(i,j);
	int directionID = -1;
	if (d->isSite()){
		std::vector<bool> localPat = {false, false, false, false, false};
		localPat[0] = d->isSite();
		std::vector<std::pair<int,int> >* vN = d->getVonNeumannN();
		for (int k=0; k< vN->size();k++){
			if (vN->at(k).first != -1){
				std::pair<int,int> vNK = vN->at(k);
				localPat[k+1] = this->getCellIJ(vNK.first,vNK.second)->isSite();
			}
			else{
				std::pair<int,int> vNK = vN->at(k);
				localPat[k+1] = true;
			}
		}
		std::vector<int> pat_Cl_Nb=Cell::patternsMap[localPat];
		int patCl = pat_Cl_Nb[0]; //class number
		int patClN = pat_Cl_Nb[1]; //inner class number
		int patNum = pat_Cl_Nb[2]; //pattern number
		std::vector<bool> pattern=Cell::classes[patCl-1][patClN-1];
		//default edge id
		std::unordered_set<int>* newID;
		newID->insert(d->getPattern()->at(2));
		d2->setPattern(&pat_Cl_Nb);
		d2->setID(newID);
		d2->setConflict(NULL);

		//ISOLATED
		if (patCl==1){
			//CASE 0
			newID->erase(newID->begin(),newID->end());
			newID->insert(d->getIDInit());
			d2->setID(newID);
		}
		//CORNERS
		if (patCl==2){
			//print('CORNERS')
			//CASE 2
			std::vector<int>* sN;
			for (int i=0;i<pattern.size();i++){
				if (pattern.at(i) and i!=0){
					sN->push_back(i-1);
				}
			}
			std::vector<std::pair<int,int> >* sNeighbors;
			for (int i=0;i<sN->size();i++){
				if (d->getVonNeumannN()->at(i).first != -1){
					sNeighbors->push_back(d->getVonNeumannN()->at(i));
				}
			}
			for (int i=0;i<sN->size();i++){
				std::pair<int,int> a = d->getVonNeumannN()->at(i);
				if (a.first != -1){
					Cell * neigh = this->getCellIJ(a.first,a.second);
					std::vector<int> * patNeigh = neigh->getPattern();
					int patNeighCl = patNeigh->at(0); //class number
					int patNeighClN = patNeigh->at(1); //inner class number
					int patNeighNum = patNeigh->at(2); //pattern number
					std::vector<bool> patternN = Cell::classes[patNeighCl-1][patNeighClN-1];

					if (patNeighCl == 2){ //CORNERS
						//print('CASE1')
						directionID = -1;
						if (((patNum == -1 or patNum == -3) and (patNeighNum==-2 or patNeighNum==-4)) or ((patNum==-2 or patNum==-4) and (patNeighNum==-1 or patNeighNum==-3))){
							//print(patNum,patNeighNum)
							//compute direction
							if ((patNum == -1 and patNeighNum==-2) or (patNum == -2 and patNeighNum==-1)){
								directionID = 2;
							}
							if ((patNum == -1 and patNeighNum==-4) or (patNum == -4 and patNeighNum==-1)){
								directionID = 3;
							}
							if ((patNum == -2 and patNeighNum==-3) or (patNum == -3 and patNeighNum==-2)){
								directionID = 1;
							}
							if ((patNum == -3 and patNeighNum==-4) or (patNum == -4 and patNeighNum==-3)){
								directionID = 0;
							}
							int mina = std::min(d->getIDInit(),neigh->getIDInit());
							int maxa = std::max(d->getIDInit(),neigh->getIDInit());
							newID->erase(newID->begin(),newID->end());
							newID->insert(this->cantor(mina,maxa));
							d2->setID(newID,directionID);
						}
					}

					if (patNeighCl == 3 or patNeighCl == 4){// #CORNER TAKES EXTREMITY and LINES PATTERN TO CORRECT
						//print('CASE2')
						//always
						//compute direction
						if (patNeighCl == 3){//#Lines
							//print('CASE2 LINES')
							directionID = -1;
							//SOUTH
							if ((patNum ==-1 or patNum==-2) and patNeighNum==-6){
								directionID = 2;
							}
							//WEST
							if ((patNum ==-1 or patNum==-4) and patNeighNum== -5){
								directionID = 3;
							}
							//NORTH
							if ((patNum ==-3 or patNum==-4) and patNeighNum==-6){
								directionID = 0;
							}
							//EAST
							if ((patNum ==-2 or patNum==-3) and patNeighNum ==-5){
								directionID = 1;
							}
						}

						if (patNeighCl == 4){//#extremities
							//print('CASE 2 EXTREMITIES')
							directionID = -1;
							if ((patNum==-1 and patNeighNum==-9)){
								directionID = 3;
							}
							if ((patNum==-1 and patNeighNum==-10)){
								directionID = 2;
							}
							if ((patNum ==-2 and patNeighNum==-9)){
								directionID = 1;
							}
							if ((patNum ==-2 and patNeighNum==-8)){
								directionID = 2;
							}
							if ((patNum ==-3 and patNeighNum==-7)){
								directionID = 1;
							}
							if ((patNum ==-3 and patNeighNum==-8)){
								directionID = 0;
							}
							if ((patNum ==-4 and patNeighNum==-7)){
								directionID = 3;
							}
							if ((patNum ==-4 and patNeighNum==-10)){
								directionID = 0;
							}
						}
						//d.mj toward free
						//print(patNum,patNeighNum,directionID)
						newID->erase(newID->begin(),newID->end());
						newID->insert(patNeighNum);
						d2->setID(newID,directionID);
					}
					if (patNeighCl == 5){
						//print('CASE7') #CORNER TAKES DIRECTION Pattern
						if (((patNum == -1 and (patNeighNum==-11 or patNeighNum==-12)) or (patNum==-2 and (patNeighNum==-12 or patNeighNum==-14)) or (patNum==-3 and (patNeighNum==-13 or patNeighNum==-14)) or (patNum==-4 and (patNeighNum==-11 or patNeighNum==-13)))){
							//compute direction
							//d.mj toward free

							int indexFalse = -1;
							std::vector<bool>::iterator it = patternN.begin();	
							it=find(patternN.begin(),patternN.end(),false);
							if (it != patternN.end())
							{
								indexFalse = distance(patternN.begin(),it);
							}
							directionID=indexFalse-1;
							newID->erase(newID->begin(),newID->end());
							newID->insert(patNeighNum);
							d2->setID(newID,directionID);
						}
					}
				}
			}
			//EXT
			if (patCl==4){
				std::vector<int> *vN;
				for (int i=0;i<pattern.size();i++){
					if (pattern.at(i)==true and i !=0){
						vN->push_back(i-1);
					}
				}
				std::vector<std::pair<int,int> >* vNeighbors;
				for (int i=0;i<vN->size();i++){
					int vn = vN->at(i);
					if (d->getVonNeumannN()->at(vn).first != -1){
						vNeighbors->push_back(d->getVonNeumannN()->at(vn));
					}
				}

				for (int i=0;i<vN->size();i++){
					int direction = vN->at(i);
					std::pair<int,int> a=d->getVonNeumannN()->at(direction);
					if (a.first != -1){	
						Cell * neigh = this->getCellIJ(a.first,a.second); 
						std::vector<int> *patNeigh = neigh->getPattern();
						int patNeighCl = patNeigh->at(0); //class number
						int patNeighClN = patNeigh->at(1); //inner class number
						int patNeighNum = patNeigh->at(2); //pattern number
						std::vector<bool> patternN = Cell::classes[patNeighCl-1][patNeighClN-1];
						//CASE5
						if (patNeighCl==3){//LINE
							//print("CASE5")
							newID->erase(newID->begin(),newID->end());
							newID->insert(patNeighNum);
							d2->setID(newID);
						}
						//CASE4
						if (patNeighCl == 4){//END
							//print("CASE4")
							//int mina = std::min(d->getIDI(),neigh->getIDI());
							//int maxa = std::max(d->getIDI(),neigh->getIDI());
							//newID = [cantor(mina,maxa)];
							d2->setID(newID);
						}
						if (patNeighCl == 5){//BORDER
							//conflict 1->1,4, 3->1,4, 2->2,3, 4->2,3
							if (((patNum == -7 or patNum == -9) and (patNeighNum==-11 or patNeighNum==-14)) or ((patNum==-8 or patNum==-10) and (patNeighNum==-12 or patNeighNum==-13))){
								//d.mj toward d.free

								int indexFalse = -1;
								std::vector<bool>::iterator it = patternN.begin();	
								it=find(pattern.begin(),patternN.end(),false);
								if (it != patternN.end())
								{
									indexFalse = distance(patternN.begin(),it);
								}
								directionID=indexFalse-1;

								newID->erase(newID->begin(),newID->end());
								newID->insert(patNeighNum);
								d2->setID(newID,directionID);
								if (neigh->getConflict() != NULL){ //#PB
									//print("CONFLICT")
									int indexFalse = -1;
									std::vector<bool>::iterator it = patternN.begin();	
									it=find(patternN.begin(),patternN.end(),false);
									if (it != patternN.end())
									{
										indexFalse = distance(patternN.begin(),it);
									}
									int directionID=(indexFalse-1+2)%4;
									//newID = neigh->getConflict();
									d2->setID(newID,directionID);
									int directionID2=(direction+2)%4;
									d2->setID(newID,directionID2);
									//print(d.i,d.j,newID,directionID,directionID2);
									//direction id was false
								}
							}
						}
					}
				}
			}	
		}

		//direction id was false
		//DIRECTIONS
		if (patCl==5){
			//print("DIRECTIONS")
			std::vector<int>* sN;
			for (int i=0; i<pattern.size() ; i++){
				if (pattern[i] and i !=0){
					sN->push_back(i-1);
				}
			}
			int indexFalse = -1;
			std::vector<bool>::iterator it = pattern.begin();	
			it=find(pattern.begin(),pattern.end(),false);
			if (it != pattern.end())
			{
				indexFalse = distance(pattern.begin(),it);
			}
			int indexOppositeFalse=((indexFalse-1)+2)%4;
			std::pair<int,int> a = d->getVonNeumannN()->at(indexOppositeFalse);

			if (a.first != -1){
				Cell * neigh = this->getCellIJ(a.first,a.second);
				std::vector<int>* patNeigh = neigh->getPattern();
				int patNeighCl = patNeigh->at(0); //class number
				int patNeighNum = patNeigh->at(2); //pattern number
				if (patNeighCl==2){ //CORNER
					std::vector<int> tab = {(indexOppositeFalse+1)%4,(indexOppositeFalse+3)%4};
					for (int j=0;j<tab.size();j++){
						//print("DIRECTIONS ICI 2")
						std::pair<int,int> e=d->getVonNeumannN()->at(j);
						if (e.first != -1){
							Cell * en = this->getCellIJ(e.first,e.second);
							std::vector<int>* patE = en->getPattern();
							int patECl = patE->at(0);
							int patENum = patE->at(2);
							if (patECl == 4){ //EXTREMITY
								//PROBLEM ICI A CORRIGER
								//print(patNum,patNeighNum,patENum)
								std::vector<int>* conflict;
								conflict->push_back(patNeighNum);
								d2->setConflict(conflict);
							}
						}
					}
				}
			}
		}
	}
}

//AREA VORONOI DIAGRAM
void Automaton::update_avd_min(int i,int j){
	this->update_aid_min(i,j);
	this->update_pr(i,j);
	this->update_bis_thin(i,j);
}

//Consensus version
void Automaton::update_cvd(int i, int j){
	this->update_consensus_id(i,j);
	this->update_pr(i,j);
	this->update_bis_thin(i,j);
}

int Automaton::cantor(int a, int b){
	return (a+b)*(a+b+1)+b;
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
