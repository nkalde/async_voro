#include "cell.h"

//CLASS ATTRIBUTES

const bool Cell::class1_array[1][5] = {{true,false,false,false,false}};
const bool Cell::class2_array[4][5] = {{true,true,true,false,false},{true,true,false,false,true},{true,false,false,true,true},{true,false,true,true,false}};
const bool Cell::class3_array[2][5] = {{true,true,false,true,false},{true,false,true,false,true}};
const bool Cell::class4_array[4][5] = {{true,true,false,false,false},{true,false,true,false,false},{true,false,false,true,false},{true,false,false,false,true}};
const bool Cell::class5_array[4][5] = {{true,true,true,true,false},{true,true,true,false,true},{true,false,true,true,true},{true,true,false,true,true}};
const bool Cell::class6_array[1][5] = {{true,true,true,true,true}};
const bool Cell::class7_array[1][5] = {{false,false,false,false,false}};

std::vector<std::vector<std::vector<bool> > > Cell::classes;

std::map<std::vector<bool>,std::vector<int> > Cell::computePatternMap(){


	std::vector<std::vector<bool> > class1;
	std::vector<std::vector<bool> > class2;
	std::vector<std::vector<bool> > class3;
	std::vector<std::vector<bool> > class4;
	std::vector<std::vector<bool> > class5;
	std::vector<std::vector<bool> > class6;
	std::vector<std::vector<bool> > class7;
	
	for (int i=0;i<1;++i){class1.push_back(std::vector<bool>(Cell::class1_array[i],Cell::class1_array[i]+5));}	
	for (int i=0;i<4;++i){class2.push_back(std::vector<bool>(Cell::class2_array[i],Cell::class2_array[i]+5));}
	for (int i=0;i<2;++i){class3.push_back(std::vector<bool>(Cell::class3_array[i],Cell::class3_array[i]+5));}
	for (int i=0;i<4;++i){class4.push_back(std::vector<bool>(Cell::class4_array[i],Cell::class4_array[i]+5));}
	for (int i=0;i<4;++i){class5.push_back(std::vector<bool>(Cell::class5_array[i],Cell::class5_array[i]+5));}
	for (int i=0;i<1;++i){class6.push_back(std::vector<bool>(Cell::class6_array[i],Cell::class6_array[i]+5));}
	for (int i=0;i<1;++i){class7.push_back(std::vector<bool>(Cell::class7_array[i],Cell::class7_array[i]+5));}
	
	Cell::classes.push_back(class1);
	Cell::classes.push_back(class2);
	Cell::classes.push_back(class3);
	Cell::classes.push_back(class4);
	Cell::classes.push_back(class5);
	Cell::classes.push_back(class6);
	Cell::classes.push_back(class7);

	int cnt=0;
	std::map<std::vector<bool>,std::vector<int> > mapPattern;
	for (int i=0; i< (Cell::classes).size(); i++){
		for (int j=0;j<Cell::classes[i].size();j++){
			mapPattern[Cell::classes[i][j]].push_back(i+1);	
			mapPattern[Cell::classes[i][j]].push_back(j+1);
			mapPattern[Cell::classes[i][j]].push_back(-cnt);
			cnt++;
		}
	}
	return mapPattern;
}

std::map<std::vector<bool>,std::vector<int> > Cell::patternsMap = Cell::computePatternMap();

//CONSTRUCTORS
Cell::Cell(int idInit, int i, int j){this->init(idInit,i,j);}

Cell::Cell(){this->init(-1,-1,-1);};//remove

void Cell::init(int idInit, int i, int j){
	this->idInit = idInit;
	this->i =i;
	this->j =j;
	this->site=false;
	this->d1=std::numeric_limits<int>::max();

	this->neighbors.assign(this->size_neighbors,std::make_pair(-1,-1));
	this->pattern.assign(this->size_pattern,-1);
	this->conflict = -1;

	this->idI.clear();
	this->idC.clear();
	this->idA.clear();
	this->idI.insert(idInit);
	this->idC.insert(idInit);
	this->idA.insert(idInit);

	this->ids.clear();
	this->ids.reserve(this->size_ids);
	this->ids.resize(5);//otherwise segfaults
	this->ids[0].insert(idInit);
	this->ids[1].insert(idInit);
	this->ids[2].insert(idInit);
	this->ids[3].insert(idInit);
	this->ids[4].insert(idInit);
}

//GETTERS
int Cell::getI() {return this->i;}
int Cell::getJ() {return this->j;}
const std::vector< std::pair<int,int> >& Cell::getVonNeumannN() {return this->neighbors;}
bool Cell::isSite() {return this->site;}
int Cell::getD1() {return this->d1;}
int Cell::getBisector() {return this->bis;}
const std::vector<int>& Cell::getPattern() {return this->pattern;}
int Cell::getConflict() {return this->conflict;}

const std::set<int>& Cell::getID(int default_value){
	if (default_value == 4)
		return this->idC;
	else
		if (!ids[default_value].empty())
			return this->ids[default_value];
		else
			return this->idC;
}

const std::set<int>& Cell::getIDI(){return this->idI;}

const std::set<int>& Cell::getIDA(){return this->idA;}

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
	//std::cerr << "setPattern" << std::endl;
	this->pattern[0] = (*pattern_p)[0];
	this->pattern[1] = (*pattern_p)[1];
	this->pattern[2] = (*pattern_p)[2];
}

void Cell::setConflict(int conflict_p){this->conflict = conflict_p;}

void Cell::setID(std::set<int> * idI_p, int default_value/*=5*/){
	if (default_value == 5){

		this->idC.clear();
		this->idC.insert(idI_p->begin(),idI_p->end());

		this->ids[default_value-1].clear();
		this->ids[default_value-1].insert(idI_p->begin(),idI_p->end());

		this->ids[default_value-2].clear();
		this->ids[default_value-2].insert(idI_p->begin(),idI_p->end());

		this->ids[default_value-3].clear();
		this->ids[default_value-3].insert(idI_p->begin(),idI_p->end());

		this->ids[default_value-4].clear();
		this->ids[default_value-4].insert(idI_p->begin(),idI_p->end());

		this->ids[default_value-5].clear();
		this->ids[default_value-5].insert(idI_p->begin(),idI_p->end());
	}
	else{
		this->ids[default_value].clear();
		this->ids[default_value].insert(idI_p->begin(),idI_p->end());
	}
}

void Cell::setIDA(std::vector<int> * idA_p){}//TODO

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
	for (std::vector<std::pair<int,int> >::const_iterator it= this->getVonNeumannN().begin(); it != this->getVonNeumannN().end(); it++){
		std::cerr << (*it).first << "," << (*it).second << '/';
	}
	std::cerr << ']';
	//std::cerr << std::endl;
}

void Cell::printPattern(){
	std::cerr << "[";
	for (std::vector<int>::const_iterator it= this->getPattern().begin(); it != this->getPattern().end(); it++){
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

void Cell::printID(){
	for (std::set<int>::const_iterator it = this->getID().begin(); it != this->getID().end(); it++){
		std::cerr << (*it);
	}
}


void Cell::printID_line(){
	long int intRep = 0;
	for (int i =0; i<4; i++){
		intRep+='#'+i+'#';	
		//std::cerr << "#" << i << "#";
		for (std::set<int>::const_iterator it = this->getID(i).begin(); it != this->getID().end(); it++){
			intRep+=(*it);
			//std::cerr << (*it);
		}
	}
	std::cerr << intRep;
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
	for (std::vector<std::set<int> >::iterator it= this->ids.begin(); it != this->ids.end(); it++){
		for (std::set<int>::iterator it2 = it->begin(); it2 != it->end(); it2++){
			std::cerr << "\t\t" << (*it2) << std::endl;
		}
	}

	std::cerr << " \tidI: " << std::endl;
	for (std::set<int>::const_iterator it = this->getIDI().begin(); it != this->getIDI().end(); it++){
		std::cerr << "\t\t" << (*it) << std::endl;
	}

	std::cerr << " \tidC: " << std::endl;
	this->printID();

	std::cerr << " \tidA: " << std::endl;
	for (std::set<int>::const_iterator it = this->getIDA().begin(); it != this->getIDA().end(); it++){
		std::cerr << "\t\t" << (*it) << std::endl;
	}
}
