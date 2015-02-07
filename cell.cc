#include "cell.h"

//CONSTRUCTORS
Cell::Cell(){
	this->i = -1;
	this->j = -1;

	memset(&this->neighbors,0,sizeof(this->neighbors));

	this->site = false;
	this->d1 = std::numeric_limits<int>::max();
	this->bis = -1;

	memset(&this->pattern,0,sizeof(this->pattern));

	memset(&this->ids,0,sizeof(this->ids));
	memset(&this->conflict,0,sizeof(this->conflict));
	
	this->idInit = -1;
	this->idI = {};
	this->idC = {};
	this->idA = {};

}

Cell::Cell(int idI, int i, int j) : idI(idI), i(i), j(j){
	this->idI = {idI};
	this->idC = {idI};
	this->idA = {idI};
}

//GETTERS
int Cell::getI() {return this->i;}
int Cell::getJ() {return this->j;}
std::vector< std::pair<int,int> >* Cell::getVonNeumannN() {return &this->neighbors;}
bool Cell::isSite() {return this->site;}
int Cell::getD1() {return this->d1;}
int Cell::getBisector() {return this->bis;}
std::vector<int>* Cell::getPattern() {return &this->pattern;}
std::vector<int>* Cell::getConflict() {return &this->conflict;}

std::unordered_set<int>* Cell::getID(int default_value=4){
	if (default_value == 4)
		return &this->idC;
	else
		if (ids[default_value].empty())//-1 NULL
			return &this->ids[default_value];
		else
			return &this->idC;
}


std::unordered_set<int>* Cell::getIDI(){return &this->idI;}
std::unordered_set<int>* Cell::getIDA(){return &this->idA;}
int Cell::getIDInit(){return this->idInit;}

//SETTERS
void Cell::setI(int i){this->i=i;}
void Cell::setJ(int j){this->j=j;}
void Cell::setVonNeumannN(std::vector< std::pair<int,int> >* neighbors_p){
	this->neighbors[0] = (*neighbors_p)[0];
	this->neighbors[1] = (*neighbors_p)[1];
	this->neighbors[2] = (*neighbors_p)[2];
	this->neighbors[3] = (*neighbors_p)[3];
}
void Cell::setSite(bool site){this->site = site;}
void Cell::setD1(int d1){this->d1 = d1;}
void Cell::setBisector(int bis){this->bis = bis;}
void Cell::setPattern(std::vector<int>* pattern_p){
	this->pattern[0] = (*pattern_p)[0];
	this->pattern[1] = (*pattern_p)[1];
	this->pattern[2] = (*pattern_p)[2];
}
void Cell::setConflict(std::vector<int> * conflict_p){}
void Cell::setID(std::vector<int> * idI_p, int default_value=5){}
void Cell::setIDA(std::vector<int> * idA_p){}
