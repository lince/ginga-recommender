#include "../include/Iteraction.h"

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {
				
					int Iteraction::idIteraction = 1;

					Iteraction::Iteraction(Program* program, Volume* volume, Key* key, string time, string type) {
						this->program = program;
						this->volume = volume;
						this->key = key;
						this->time = time;
						this->type = type;
					}
					
					string Iteraction::generateSql() {
						Utilitario* util = Utilitario::getInstance();
						return "insert into iteraction (time, type, idProgram, idVolume, idKey) values ('" + time + "', '" + type + "', " + util->IntToString(program->getIdProgram()) + ", " + util->IntToString(volume->getIdVolume()) + ", " + util->IntToString(key->getIdKey()) + ");";
					}
					
					int Iteraction::getIdIteraction(){return idIteraction;}
					void Iteraction::setIdIteraction(int id){idIteraction = id;}
					void Iteraction::incIdIteraction(){idIteraction++;}
					
					Program* Iteraction::getProgram(){return program;}
					void Iteraction::setProgram(Program* program){this->program = program;}
					
					Volume* Iteraction::getVolume(){return volume;}
					void Iteraction::setVolume(Volume* volume){this->volume = volume;}
					
					Key* Iteraction::getKey(){return key;}
					void Iteraction::setKey(Key* key){this->key = key;}
					
					string Iteraction::getTime(){return time;}
					void Iteraction::setTime(string time){this->time = time;}
					
					string Iteraction::getType(){return type;}
					void Iteraction::setType(string type){this->type = type;}

				}
			}
		}
	}
}

