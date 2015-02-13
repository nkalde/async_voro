#include "automaton.h"

Automaton::Automaton(int lig, int col){
	this->init(lig,col);
}

Automaton::Automaton() : Automaton(0,0){}


//create first map
Automaton::Automaton(std::string filename){
	cv::Mat img = cv::imread(filename,cv::IMREAD_GRAYSCALE);
	this->init(img.rows,img.cols);
	std::cerr << " filename " << filename << " rowsxcols " << img.rows << "x" << img.cols;
	for(int row = 0; row < img.rows; ++row) {
		for(int col = 0; col < img.cols; ++col) {
			Cell *c = this->getCellIJ(row,col);
			c->setSite(img.at<uchar>(row,col)<100);
		}
	}
}

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
			std::vector<std::pair<int,int> > neighbors(4);
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
			//std::cerr << i << "/"<< j << std::endl;
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
Cell * Automaton::getCellIJ(std::pair<int,int> coords){ return this->getCellIJ(coords.first,coords.second);}

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
		switch (function){
			case Automaton::bis_thick:{
							  this->update_bis_thick(randI,randJ);
							  break;
						  }
			case Automaton::bis_thin:{
							 this->update_bis_thin(randI,randJ);
							 break;
						 }
			case Automaton::aid_min:{
							this->update_aid_min(randI,randJ);
							break;
						}
			case Automaton::avd_min:{
							this->update_avd_min(randI,randJ);
							break;
						}
			case Automaton::consensus_id:{
							     this->update_consensus_id(randI,randJ);
							     break;
						     }
			case Automaton::cvd:{
						    this->update_cvd(randI,randJ);
						    break;
					    }
			case Automaton::pr:{
						   this->update_pr(randI,randJ);
						   break;
					   }
		}
		cnt++;
		if (cnt%(3*this->lig*this->col*this->lig*this->col) == 0)
			return;
	}
}

void Automaton::detectPattern(Cell *cell){//to put in cell class in the end with communication with other cells
	std::vector<std::pair<int,int> > vN = cell->getVonNeumannN();
	std::vector<bool> localPat = {false,false,false,false,false};

	//if (cell->isSite()){
	localPat[0] = cell->isSite();

	for (int k=0; k<vN.size();k++){
		if (vN[k].first != -1){
			std::pair<int,int> vNK = vN[k];
			localPat[k+1] = this->getCellIJ(vN[k])->isSite();//core dumped segfault
		}
	}
	std::vector<int> pat_Cl_Nb = Cell::patternsMap[localPat];
	cell->setPattern(&pat_Cl_Nb);
	//}
}

void Automaton::update_pr(int i,int j){
	//Distance and Semantic layers
	//std::cerr << "i,j" << i << "," << j << std::endl;
	Cell * d= this->getCellIJ(i,j);
	if (d->isSite()){
		//distance update
		d->setD1(0);
	}
	else{
		std::vector<std::pair<int,int> > dVNN = d->getVonNeumannN();

		//ALPHA CONDITION neighbors
		std::vector<std::pair<int,int> > alpha;
		std::vector<int> alphaN;
		for (int i=0; i<dVNN.size();i++)
		{
			if (dVNN[i].first != -1){
				alpha.push_back(dVNN[i]);
				alphaN.push_back(i);
			}
		}
		//std::cerr << "alpha size: " << alphaN.size() << std::endl;

		//BETA CONDITION sites and non-sites modified
		std::vector<std::pair<int,int> > beta;
		std::vector<int> betaN;
		for (int i=0;i<alpha.size();i++){
			Cell * cell_a = this->getCellIJ(alpha[i]);
			if (cell_a->isSite() or (! cell_a->isSite() and cell_a->getIDI() != cell_a->getID()))
			{
				beta.push_back(alpha[i]);
				betaN.push_back(alphaN[i]);
			}
		}
		if (betaEnabled==false){
			beta = alpha;
			betaN = alphaN;
		}
		//std::cerr << "beta size: " << betaN.size() << std::endl;

		//DELTA CONDITION minDist
		int minD1 = -1;
		for (int i=0;i<beta.size();i++){
			Cell * cell_b = this->getCellIJ(beta[i]);
			if (minD1==-1 or minD1>cell_b->getD1()){
				minD1 = cell_b->getD1();
			}
		}
		//std::cerr << "min d1 " << minD1 << std::endl;

		std::vector<std::pair<int,int> > delta;
		std::vector<int> deltaN;
		for (int i=0;i<beta.size();i++){
			Cell * cell_b = this->getCellIJ(beta[i]);
			if (cell_b->getD1() == minD1){
				delta.push_back(beta[i]);
				deltaN.push_back(betaN[i]);
			}
		}
		if (deltaEnabled==false){
			delta=beta;
			deltaN=betaN;
		}
		//std::cerr << "delta size: " << deltaN.size() << std::endl;

		//EPSILON CONDITION minContribution
		std::vector<int> lenIdDelta;
		int minIdDelta = -1;
		for (int i=0;i<delta.size();i++){
			Cell * cell_d = this->getCellIJ(delta[i]);
			lenIdDelta.push_back(cell_d->getID().size());
			if (minIdDelta==-1 or minIdDelta >lenIdDelta[i]){
				minIdDelta = lenIdDelta[i];
			}
		}
		//std::cerr << "min id delta " << minIdDelta << std::endl;

		std::vector<std::pair<int,int> > epsilon;
		std::vector<int> epsilonN;
		for (int i=0;i<delta.size();i++){
			Cell * cell_d = this->getCellIJ(delta[i]);
			if(cell_d->getID().size() == minIdDelta){
				epsilon.push_back(delta[i]);
				epsilonN.push_back(deltaN[i]);
			}
		}
		if (epsilonEnabled==false){
			epsilon=delta;
			epsilonN=deltaN;
		}
		//if (epsilon.size() > 0)
		//    std::cerr << "before" << d->getID() << std::endl;
		//std::cerr << "epsilon size: " << epsilonN.size() << std::endl;

		std::unordered_set<int> idS;
		for(int i=0; i< epsilonN.size();i++){
			Cell* cellN = this->getCellIJ(dVNN[epsilonN[i]]);
			//id update
			idS.insert(cellN->getID((i+2)%4).begin(),cellN->getID((i+2)%4).end());
			// std::cerr << cellN->getID() << std::endl;
		}
		//if (epsilon.size() > 0)
		//std::cerr << "after" << d->getID() << std::endl;

		if (!idS.empty()){

			//distance update
			if (minD1 == std::numeric_limits<int>::max())
				d->setD1(minD1);
			else
				d->setD1(minD1+1);

			//identifier update
			d->setID(&idS);
		}
	}
}

//bisector
void Automaton::update_bis_thick(int i,int j){//,automatonCS,automatonNS){
	Cell * d= this->getCellIJ(i,j);
	const std::vector<std::pair<int,int> > dVNN = d->getVonNeumannN();

	//C0 von Neumann neighbors
	std::vector<std::pair<int,int> > c0;
	for (int i=0; i<dVNN.size();i++){
		if (dVNN[i].first != -1){
			c0.push_back(dVNN[i]);
		}
	}

	//C1 id different
	int nb_inter = 0;
	for (int i=0; i<c0.size();i++){
		Cell * cell_c0 = this->getCellIJ(c0[i]);
		if (cell_c0->getID() != d->getID()){
			nb_inter++;
		}
	}
	bool c1 = nb_inter > 0 and not d->isSite();

	d->setBisector(0);
	if (c1)
		d->setBisector(1);
}

void Automaton::update_bis_thin(int i, int j){//,automatonCS,automatonNS){
	Cell * d= this->getCellIJ(i,j);
	const std::vector<std::pair<int,int> > dVNN = d->getVonNeumannN();

	//C0 von Neumann neighbors
	std::vector<std::pair<int,int> > c0;
	for (int i=0; i<dVNN.size();i++){
		if (dVNN[i].first != -1){
			c0.push_back(dVNN[i]);
		}
	}

	//C1 multiple ids
	int dID_size = d->getID().size();
	bool c1 = dID_size > 1;

	//C2 one id
	int siteNeighbors_size = 0;
	for (int i=0; i< c0.size();i++){
		if (this->getCellIJ(c0[i])->isSite()){
			siteNeighbors_size++;
		}
	}

	bool inter_size = 0;
	for (int i=0; i< c0.size();i++){
		if ((this->getCellIJ(c0[i])->getID().size() == 1) and (d->getID() != this->getCellIJ(c0[i])->getID())){
			inter_size++;
		}
	}
	bool c2 = dID_size <= 1 and inter_size > 0 and not d->isSite();
	bool c2p = c2 and siteNeighbors_size < 1;

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
	const std::vector<std::pair<int,int> > dVNN = d->getVonNeumannN();

	//C0 von Neumann neighbors
	std::vector<std::pair<int,int> > c0;
	for (int i=0; i<dVNN.size();i++){
		if (dVNN[i].first != -1){
			c0.push_back(dVNN[i]);
		}
	}

	std::vector<std::pair<int,int> > sites;
	for (int i=0; i<c0.size();i++){
		if (this->getCellIJ(c0[i])->isSite()){
			sites.push_back(c0[i]);
		}
	}

	//C1 multiple ids
	bool c1 = d->isSite() and sites.size() > 0;
	if (c1){
		std::unordered_set<int> idS;
		idS.insert(d->getID().begin(),d->getID().end());
		for (int i=0;i<sites.size();i++){
			idS.insert(this->getCellIJ(sites[i])->getID().begin(),this->getCellIJ(sites[i])->getID().end());
		}
		std::unordered_set<int>::iterator iter = std::min_element(idS.begin(),idS.end());
		int idSMin = *iter;
		idS.clear();
		idS.insert(idSMin);
		d->setID(&idS);
	}
}

//tzionas
void Automaton::update_consensus_id(int i, int j){
	Cell *d = this->getCellIJ(i,j);
	Cell *d2 = this->getCellIJ(i,j);//asynchronous case the same

	int directionID = -1;
	if (d->isSite()){
		//std::cerr << "site" << std::endl;
		std::vector<bool> localPat = {false, false, false, false, false};
		localPat[0] = d->isSite();
		const std::vector<std::pair<int,int> > vN = d->getVonNeumannN();
		for (int k=0;k<vN.size();k++){
			if (vN[k].first != -1){
				localPat[k+1] = this->getCellIJ(vN[k])->isSite();
			}
			else{
				localPat[k+1] = true;
			}
		}

		std::vector<int> pat_Cl_Nb=Cell::patternsMap[localPat];
		int patCl = pat_Cl_Nb[0]; //class number
		int patClN = pat_Cl_Nb[1]; //inner class number
		int patNum = pat_Cl_Nb[2]; //pattern number
		std::vector<bool> pattern=Cell::classes[patCl-1][patClN-1];

		//default edge id
		std::unordered_set<int> newID;
		newID.insert(d->getPattern()[2]);
		d2->setPattern(&pat_Cl_Nb);
		d2->setID(&newID);
		d2->setConflict(-1);

		//ISOLATED
		if (patCl==1){
			std::cerr << "isolated" << std::endl;
			//CASE 0
			newID.clear();
			newID.insert(d->getIDInit());
			d2->setID(&newID);
		}
		//CORNERS
		if (patCl==2){	
			//std::cerr << "corners" << std::endl;
			//std::cerr << "CORNERS" << std::endl;
			//CASE 2
			std::vector<int> sN;
			for (int i=0;i<pattern.size();i++){
				if (pattern.at(i) and i!=0){
					sN.push_back(i-1);
				}
			}
			std::vector<std::pair<int,int> > sNeighbors;
			for (int i=0;i<sN.size();i++){
				if (d->getVonNeumannN()[i].first != -1){
					sNeighbors.push_back(d->getVonNeumannN()[i]);
				}
			}
			for (int i=0;i<sN.size();i++){
				std::pair<int,int> a = d->getVonNeumannN()[i];
				if (a.first != -1){
					Cell * neigh = this->getCellIJ(a.first,a.second);
					const std::vector<int> patNeigh = neigh->getPattern();
					int patNeighCl = patNeigh[0]; //class number
					int patNeighClN = patNeigh[1]; //inner class number
					int patNeighNum = patNeigh[2]; //pattern number
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

							newID.clear();
							newID.insert(this->cantor(mina,maxa));
							d2->setID(&newID,directionID);
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
						newID.clear();
						newID.insert(patNeighNum);
						d2->setID(&newID,directionID);
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
							newID.clear();
							newID.insert(patNeighNum);
							d2->setID(&newID,directionID);
						}
					}
				}
			}
			//EXT
			if (patCl==4){
				//std::cerr << "ext" << std::endl;
				std::vector<int> *vN;
				for (int i=0;i<pattern.size();i++){
					if (pattern.at(i)==true and i !=0){
						vN->push_back(i-1);
					}
				}
				std::vector<std::pair<int,int> >* vNeighbors;
				for (int i=0;i<vN->size();i++){
					int vn = vN->at(i);
					if (d->getVonNeumannN()[vn].first != -1){
						vNeighbors->push_back(d->getVonNeumannN()[vn]);
					}
				}

				for (int i=0;i<vN->size();i++){
					int direction = vN->at(i);
					std::pair<int,int> a=d->getVonNeumannN()[direction];
					if (a.first != -1){
						Cell * neigh = this->getCellIJ(a.first,a.second);
						const std::vector<int> patNeigh = neigh->getPattern();
						int patNeighCl = patNeigh[0]; //class number
						int patNeighClN = patNeigh[1]; //inner class number
						int patNeighNum = patNeigh[2]; //pattern number
						std::vector<bool> patternN = Cell::classes[patNeighCl-1][patNeighClN-1];
						//CASE5
						if (patNeighCl==3){//LINE
							//print("CASE5")
							newID.clear();
							newID.insert(patNeighNum);
							d2->setID(&newID);
						}
						//CASE4
						if (patNeighCl == 4){//END
							//print("CASE4")
							int mina = std::min(d->getIDInit(),neigh->getIDInit());
							int maxa = std::max(d->getIDInit(),neigh->getIDInit());
							newID.clear();
							newID.insert(this->cantor(mina,maxa));
							d2->setID(&newID,directionID);
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

								newID.clear();
								newID.insert(patNeighNum);
								d2->setID(&newID,directionID);
								if (neigh->getConflict() != -1){ //#PB
									//print("CONFLICT")
									int indexFalse = -1;
									std::vector<bool>::iterator it = patternN.begin();
									it=find(patternN.begin(),patternN.end(),false);
									if (it != patternN.end())
									{
										indexFalse = distance(patternN.begin(),it);
									}
									int directionID=(indexFalse-1+2)%4;
									newID.insert(neigh->getConflict());
									d2->setID(&newID,directionID);
									int directionID2=(direction+2)%4;
									d2->setID(&newID,directionID2);
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
			//std::cerr << "directions" << std::endl;
			//print("DIRECTIONS")
			std::vector<int> sN;
			for (int i=0; i<pattern.size() ; i++){
				if (pattern[i] and i !=0){
					sN.push_back(i-1);
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
			std::pair<int,int> a = d->getVonNeumannN()[indexOppositeFalse];

			if (a.first != -1){
				Cell * neigh = this->getCellIJ(a.first,a.second);
				std::vector<int> patNeigh = neigh->getPattern();
				int patNeighCl = patNeigh[0]; //class number
				int patNeighNum = patNeigh[2]; //pattern number
				if (patNeighCl==2){ //CORNER
					std::vector<int> tab = {(indexOppositeFalse+1)%4,(indexOppositeFalse+3)%4};
					for (int j=0;j<tab.size();j++){
						//print("DIRECTIONS ICI 2")
						std::pair<int,int> e=d->getVonNeumannN()[j];
						if (e.first != -1){
							Cell * en = this->getCellIJ(e.first,e.second);
							std::vector<int> patE = en->getPattern();
							int patECl = patE[0];
							int patENum = patE[2];
							if (patECl == 4){ //EXTREMITY
								//PROBLEM ICI A CORRIGER
								//print(patNum,patNeighNum,patENum)
								d2->setConflict(patNeighNum);
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
				case(Cell::print_distance)://distance
					std::cerr << this->getCellIJ(i,j)->getD1();
					break;
				case(Cell::print_site)://site
					std::cerr << this->getCellIJ(i,j)->isSite();
					break;
				case(Cell::print_identifier_init)://id init
					std::cerr << this->getCellIJ(i,j)->getIDInit();
					break;
				case(Cell::print_identifier_comp)://id comp
					this->getCellIJ(i,j)->printID();
					break;
				case(Cell::print_identifier_area)://id area
					this->getCellIJ(i,j)->printID();
					break;
				case(Cell::print_identifier_line)://id line
					this->getCellIJ(i,j)->printID_line();
					break;
				case(Cell::print_bisector)://bisector
					std::cerr << this->getCellIJ(i,j)->getBisector();
					break;
				case(Cell::print_neighbors)://neighbors
					this->getCellIJ(i,j)->printNeighbors();
					break;
				case(Cell::print_pattern)://pattern
					this->getCellIJ(i,j)->printPattern();
				default:
					break;
			}
		}
		std::cerr << std::endl;
	}
}

void Automaton::drawAutomaton(){
	cv::Mat sites_img(this->getLig(),this->getCol(),CV_8U);
	cv::Mat d1_img(this->getLig(),this->getCol(),CV_8U);
	cv::Mat id_img(this->getLig(),this->getCol(),CV_8U);
	cv::Mat bis_img(this->getLig(),this->getCol(),CV_8U);
	
	int max_d1 = -1;
	for(int i=0;i<this->getLig();i++){
		for (int j=0;j<this->getCol();j++){
			if (max_d1 < this->getCellIJ(i,j)->getD1())
				max_d1 = this->getCellIJ(i,j)->getD1();
		}
	}	
	
	for(int i=0;i<this->getLig();i++){
		for (int j=0;j<this->getCol();j++){
			sites_img.at<uchar>(i,j) = this->getCellIJ(i,j)->isSite()?0:255;
			d1_img.at<uchar>(i,j) = this->getCellIJ(i,j)->getD1()*255/(max_d1);
			bis_img.at<uchar>(i,j) = this->getCellIJ(i,j)->getBisector()*255/2;;
		}
	}

	std::string sites_winname = "sites";
	std::string d1_winname = "distance_map";
	std::string id_winname = "region";
	std::string bis_winname = "bisector";

	cv::namedWindow(sites_winname, cv::WINDOW_NORMAL);
	cv::namedWindow(d1_winname, cv::WINDOW_NORMAL);
	cv::namedWindow(id_winname, cv::WINDOW_NORMAL);
	cv::namedWindow(bis_winname, cv::WINDOW_NORMAL);	

	cv::imshow(sites_winname, sites_img);
	cv::imshow(d1_winname, d1_img);
	cv::imshow(id_winname, id_img);
	cv::imshow(bis_winname, bis_img);

	cv::waitKey(0);
}

