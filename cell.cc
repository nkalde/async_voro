#include "cell.h"

//CLASS ATTRIBUTES
const std::vector<std::vector<bool> >	Cell::class1 = {{true,false,false,false,false}};
const std::vector<std::vector<bool> > Cell::class2 = {{true,true,true,false,false},{true,true,false,false,true},{true,false,false,true,true},{true,false,true,true,false}};
const std::vector<std::vector<bool> > Cell::class3 = {{true,true,false,true,false},{true,false,true,false,true}};
const std::vector<std::vector<bool> > Cell::class4 = {{true,true,false,false,false},{true,false,true,false,false},{true,false,false,true,false},{true,false,false,false,true}};
const std::vector<std::vector<bool> > Cell::class5 = {{true,true,true,true,false},{true,true,true,false,true},{true,false,true,true,true},{true,true,false,true,true}};
const std::vector<std::vector<bool> > Cell::class6 = {{true,true,true,true,true}};
const std::vector<std::vector<bool> > Cell::class7 = {{false,false,false,false,false}};
const std::vector<std::vector<std::vector<bool> > > Cell::classes = {Cell::class1, Cell::class2, Cell::class3, Cell::class4, Cell::class5, Cell::class6, Cell::class7};

std::map<std::vector<bool>,std::vector<int> > Cell::computePatternMap(){
	int cnt=0;
	std::map<std::vector<bool>,std::vector<int> > mapPattern;
	for (int i=0; i< (Cell::classes).size(); i++){
		for (int j=0;j<Cell::classes[i].size();j++){
			mapPattern[Cell::classes[i][j]] = {i+1,j+1,-cnt};
			cnt++;
		}
	}
	return mapPattern;
}
std::map<std::vector<bool>,std::vector<int> > Cell::patternsMap = Cell::computePatternMap();

//CONSTRUCTORS
Cell::Cell(int idInit, int i, int j){this->init(idInit,i,j);}

Cell::Cell() : Cell(-1,-1,-1){}//remove

void Cell::init(int idInit, int i, int j){
	this->idInit = idInit;
	this->i =i;
	this->j =j;
	this->site=false;
	this->d1=std::numeric_limits<int>::max();

	this->neighbors.assign(this->size_neighbors,std::make_pair(-1,-1));
	this->pattern.assign(this->size_pattern,-1);
	this->conflict.assign(this->size_conflict,-1);
	this->idI.insert(idInit);
	this->idC.insert(idInit);
	this->idA.insert(idInit);

	this->ids.reserve(this->size_ids);
	this->ids.resize(5);//otherwise segfaults
	this->ids.at(0) = {idInit};
	this->ids.at(1) = {idInit};
	this->ids.at(2) = {idInit};
	this->ids.at(3) = {idInit};
	this->ids.at(4) = {idInit};
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

std::unordered_set<int>* Cell::getID(int default_value){
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
	std::cerr << "setPattern" << std::endl;
	this->pattern[0] = (*pattern_p)[0];
	this->pattern[1] = (*pattern_p)[1];
	this->pattern[2] = (*pattern_p)[2];
}
void Cell::setConflict(std::vector<int> * conflict_p){}
void Cell::setID(std::unordered_set<int> * idI_p, int default_value/*=5*/){}
void Cell::setIDA(std::vector<int> * idA_p){}
void Cell::setIDInit(int idInit){this->idInit = idInit;}
void Cell::update(int function){
	switch(function){
		case(0):
			break;

		default:
			break;
	}
}

//PRINT
void Cell::printNeighbors(){
	std::cerr << "[";
	for (std::vector<std::pair<int,int> >::iterator it= this->getVonNeumannN()->begin(); it != this->getVonNeumannN()->end(); it++){
		std::cerr << (*it).first << "," << (*it).second << '/';	
	}
	std::cerr << ']';
	//std::cerr << std::endl;
}

void Cell::printPattern(){
	std::cerr << "[";
	for (std::vector<int>::iterator it= this->getPattern()->begin(); it != this->getPattern()->end(); it++){
		std::cerr << (*it) << "," << "/";	
	}
	std::cerr << "]";
}


void Cell::printPatternsMap(){
	for(std::map<std::vector<bool>,std::vector<int> >::const_iterator it = Cell::patternsMap.begin(); it != Cell::patternsMap.end(); ++it){
		for(std::vector<bool>::const_iterator it_key = (it->first).begin(); it_key != (it->first).end(); ++it_key){
			std::cerr << (*it_key);
		}
		std::cerr << std::endl;

		for(std::vector<int>::const_iterator it_value = (it->second).begin(); it_value != (it->second).end(); ++it_value){
			std::cerr << (*it_value);
		}
		std::cerr << std::endl;
	}
}

void Cell::printCell(){
	std::cerr << "\nCell printing function" << std::endl;

	std::cerr << " \ti: " << this->getI() << std::endl;

	std::cerr << " \tj: " << this->getJ() << std::endl;

	std::cerr << " \tneighbors: " << std::endl;
	this->printNeighbors();

	std::cerr << " \tsite: " << this->isSite() << std::endl;

	std::cerr << " \td1: " << this->getD1() << std::endl;

	std::cerr << " \tbis: "<< this->getBisector() << std::endl;

	std::cerr << " \tpattern: " << std::endl;
	this->printPattern();

	std::cerr << " \tidInit: " << this->getIDInit() << std::endl;

	std::cerr << " \tids: " << std::endl;

	for (std::vector<std::unordered_set<int> >::iterator it= this->ids.begin(); it != this->ids.end(); it++){
		for (std::unordered_set<int>::iterator it2 = it->begin(); it2 != it->end(); it2++){
			std::cerr << "\t\t" << (*it2) << std::endl;
		}	
	}

	std::cerr << " \tidI: " << std::endl;	
	for (std::unordered_set<int>::iterator it = this->getIDI()->begin(); it != this->getIDI()->end(); it++){
		std::cerr << "\t\t" << (*it) << std::endl;
	}	

	std::cerr << " \tidC: " << std::endl;
	for (std::unordered_set<int>::iterator it = this->getID()->begin(); it != this->getID()->end(); it++){
		std::cerr << "\t\t" << (*it) << std::endl;
	}	

	std::cerr << " \tidA: " << std::endl;
	for (std::unordered_set<int>::iterator it = this->getIDA()->begin(); it != this->getIDA()->end(); it++){
		std::cerr << "\t\t" << (*it) << std::endl;
	}	
}
