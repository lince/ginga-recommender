#include "../include/LocalAgent.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace recommender {
                
                	LocalAgent* LocalAgent::_instance = 0;

                    /**
                     * Construtor
                     */
                    LocalAgent::LocalAgent() {
						setEstado(true); // O módulo LocalAgent começa ativo
                    	db = new Database("","","/usr/local/etc/ginga/files/recommender/iteraction.db");
                    	tuner = NULL;
                    	nclStateMachine = NULL;
                    	document = NULL;
                    	media = NULL;
                    	context = NULL;
                    	stateManager = NULL;
                    	ps = NULL;
                    	if(db->createDatabase()) {
							setPrimaryKey();
							cm = IComponentManager::getCMInstance();
							stateManager = ((StateManagerCreator*)(cm->getObject("StateManager")))();
                    		start();
                    	}
                    	else {
                    		setEstado(false);
                    	}
                    }

                    /**
                     * Destrutor.
                     */
                    LocalAgent::~LocalAgent() {
                    	estado = false;
                        db->closeDatabase();
                    }

                    /**
                     * Retorna a instancia única da classe
                     */
                    LocalAgent* LocalAgent::getInstance() {
                    	if(LocalAgent::_instance == NULL) {
                    		LocalAgent::_instance = new LocalAgent();
                    	}
                    	return LocalAgent::_instance;
                    }

                    /**
                     * Retorna o banco de dados onde são armazendas as informações coletadas.
                     * @return banco de dados onde são armazendas as informações coletadas.
                     */
                    Database* LocalAgent::getDatabase() {
                        return db;
                    }

					/**
                     * Pára a execução do LocalAgent.
                     * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                     */
                    bool LocalAgent::stop() {
                    	setEstado(false);
                    }

                    /**
                     * Adiciona um AgentListener ao LocalAgent.
                     * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                     */
                    bool LocalAgent::addAgentListener(AgentListener* listener) {
                    	if (listener != NULL) {
                        	listeners->push_back(listener);
                        	return true;
                        }
                        return false;
                    }

                    /**
                     * Remove um AgentListener do LocalAgent.
                     * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                     */
                    bool LocalAgent::removeAgentListener(AgentListener* listener) {
                    	vector<AgentListener*>::iterator it;
                        for (it = listeners->begin(); it < listeners->end(); ++it) {
                            if ((AgentListener*)(*it) == listener) {
                                listeners->erase(it);
                                if(listeners != NULL && listeners->empty()) {
									delete listeners;
									listeners = NULL;
								}
								return true;
                            }
                        }
                        return false;
                    }
                    
                    void LocalAgent::run() {
                    	while(1) {
                    		while(!iters.empty()) {
			            		if(db != NULL) {
									Iteraction* iteraction = iters.front();
                                    if(iteraction != NULL) {
                                        lock();
									    // key
									    db->query(iteraction->getKey()->generateSql().c_str());
									
									    if(iteraction->getKey()->getType() == "interaction") {
										    nclStateMachine = new NclStateMachine(iteraction);
										    
									        db->query(nclStateMachine->generateSql().c_str());

								            string docName = "";

							                ps = stateManager->getPresentationState();

							                if(ps != NULL) {
							                
							                    docName = ps->getDocumentName();

								                document = new Document(docName, nclStateMachine);
								                
								                if(document != NULL) {
								                     db->query(document->generateSql().c_str());
								                }

										        set<string> conjunto;
										        pair<set<string>::iterator,bool> ret;
										        vector<string>* names;
										        string str;
										        int found;
										        names = ps->getPlayersNames();
										        vector<string>::iterator it;
										        Context* anterior = NULL;
										        
                                                if(!names->empty()) {
										            for(it = names->begin(); it < names->end(); it++) {
											            str = (string)(*it);
											            IElementaryState* iEL = ps->getElementaryState(str);
											            if(iEL == NULL) {
											                continue;
											            }

											            int status = iEL->getStatus();
											            
											            string sStatus;
											            if (status == 2) {
												            sStatus = "paused";
											            } else if (status == 1) {
												            sStatus = "occoring";
											            } else if (status == 3) {
												            sStatus = "sleeping";
											            } else {
												            sStatus = "none";
											            }
                                                        
											            found = str.find('/');
											            string strSub;
											            string strSub2;
                                                        if(found != string::npos) {
											                strSub = str.substr(0, found);
											                strSub2 = str.substr(found+1);
											                while(found != string::npos) {
												                ret = conjunto.insert(strSub);
												                set<string>::iterator ito;
												                if(ret.second == true) {
													                Context* context = new Context(strSub, anterior, document);
													                db->query(context->generateSql().c_str());
													                anterior = context;
													                Context::incIdContext();
												                }
												                found = strSub2.find('/');
												                strSub = strSub2.substr(0, found);
												                strSub2 = strSub2.substr(found+1);
											                }
                                                        }
                                                        
											            double time = ps->getElementaryState(str)->getMediaTime();
											            std::ostringstream oss;
											            oss << time;
											            string stime = oss.str();
											            found = str.find('/');
                                                        Media* media;
                                                        if(found != string::npos) {
            											    strSub = str.substr(found+1);
                                                        
											                while(found != string::npos) {
												                found = strSub.find('/');
												                strSub = strSub.substr(found+1);
											                }
                                                            media = new Media(strSub, sStatus, stime, anterior, document);
                                                        }
                                                        else {
                                                            media = new Media(str, sStatus, stime, anterior, document);
                                                        }
                                                        db->query(media->generateSql().c_str());
											            Media::incIdMedia();
										            }
                                                    names->clear();
                                                    names->~vector();
                                                }
										        NclStateMachine::incIdNclStateMachine();
										        Document::incIdDocument();
                                                if(nclStateMachine != NULL) {
                                                    delete nclStateMachine;
                                                    nclStateMachine = NULL;
                                                }
                                                if(document != NULL) {
                                                    delete document;
                                                    document = NULL;
                                                }
                                            }
                                            delete ps;
                                            ps = NULL;
									    }

									    // volume
									    db->query(iteraction->getVolume()->generateSql().c_str());
						
									    // channel
									    db->query(iteraction->getProgram()->getChannel()->generateSql().c_str());
						
									    // program
									    db->query(iteraction->getProgram()->generateSql().c_str());
						
									    // iteraction
									    db->query(iteraction->generateSql().c_str());
						
									    // incrementa chave primária
									    Key::incIdKey();
									    Volume::incIdVolume();
									    Channel::incIdChannel();
									    Program::incIdProgram();
									    Iteraction::incIdIteraction();
                                        unlock();
									    removeIteraction(iteraction);
                                    }
								}
							}
                    		usleep(500);
                    	}
                    }
                    
                    bool LocalAgent::addIteraction(Iteraction* iteraction) {
                    	if (iteraction != NULL) {
                            lock();
                        	iters.push_back(iteraction);
                            unlock();
                        	return true;
                        }
                        return false;
                    }
                    
                    bool LocalAgent::removeIteraction(Iteraction* iteraction) {
                    	vector<Iteraction*>::iterator it;
                        lock();
                        for (it = iters.begin(); it < iters.end(); ++it) {
                            if ((Iteraction*)(*it) == iteraction) {
                                iters.erase(it);
                                unlock();
								return true;
                            }
                        }
                        unlock();
                        return false;
                    }
                    
                    string LocalAgent::getKeyName(int keyCode) {
                    	CodeMap* map = CodeMap::getInstance();
                    	string value = "";
                    	if(keyCode == CodeMap::KEY_F12 || keyCode == CodeMap::KEY_PAUSE) {
							value = "PAUSE";
						}
						else if (keyCode == CodeMap::KEY_F11 || keyCode == CodeMap::KEY_STOP) {
							value = "STOP";
						}
						else if (keyCode == CodeMap::KEY_F1 || keyCode == CodeMap::KEY_RED) {
							value = "RED";
						}
						else if (keyCode == CodeMap::KEY_F2 || keyCode == CodeMap::KEY_GREEN) {
							value = "GREEN";
						}
						else if (keyCode == CodeMap::KEY_F3 || keyCode == CodeMap::KEY_YELLOW) {
							value = "YELLOW";
						}
						else if (keyCode == CodeMap::KEY_F4 || keyCode == CodeMap::KEY_BLUE) {
							value = "BLUE";
						}
						else if (keyCode == CodeMap::KEY_ENTER || keyCode == CodeMap::KEY_OK) {
							value = "OK";
						}
						else if (keyCode == CodeMap::KEY_PAGE_DOWN || keyCode == CodeMap::KEY_CHANNEL_DOWN) {
							value = "CHANNEL_DOWN";
						}
						else if (keyCode == CodeMap::KEY_PAGE_UP || keyCode == CodeMap::KEY_CHANNEL_UP) {
							value = "CHANNEL_UP";
						}
						else if (keyCode == CodeMap::KEY_VOLUME_DOWN) {
							value = "VOLUME_DOWN";
						}
						else if (keyCode == CodeMap::KEY_VOLUME_UP) {
							value = "VOLUME_UP";
						}
						else if (keyCode == CodeMap::KEY_CURSOR_DOWN) {
							value = "CURSOR_DOWN";
						}
						else if (keyCode == CodeMap::KEY_CURSOR_UP) {
							value = "CURSOR_UP";
						}
						else if (keyCode == CodeMap::KEY_CURSOR_LEFT) {
							value = "CURSOR_LEFT";
						}
						else if (keyCode == CodeMap::KEY_CURSOR_RIGHT) {
							value = "CURSOR_RIGHT";
						}
						else if (keyCode == CodeMap::KEY_BACKSPACE) {
							value = "BACKSPACE";
						}
						else if (keyCode == CodeMap::KEY_BACK) {
							value = "BACK";
						}
						else if (keyCode == CodeMap::KEY_ESCAPE) {
							value = "ESCAPE";
						}
						else if (keyCode == CodeMap::KEY_EXIT) {
							value = "EXIT";
						}
						else if (keyCode == CodeMap::KEY_POWER) {
							value = "POWER";
						}
						else if (keyCode == CodeMap::KEY_REWIND) {
							value = "REWIND";
						}
						else if (keyCode == CodeMap::KEY_EJECT) {
							value = "EJECT";
						}
						else if (keyCode == CodeMap::KEY_PLAY) {
							value = "PLAY";
						}
						else if (keyCode == CodeMap::KEY_RECORD) {
							value = "RECORD";
						}
						else if (keyCode == CodeMap::KEY_SUPER) {
							value = "SUPER";
						}
						else if (keyCode == CodeMap::KEY_PRINTSCREEN) {
							value = "PRINTSCREEN";
						}
						else if (keyCode == CodeMap::KEY_MENU) {
							value = "MENU";
						}
						else if (keyCode == CodeMap::KEY_INFO) {
							value = "INFO";
						}
						else if (keyCode == CodeMap::KEY_EPG) {
							value = "EPG";
						}
						else {
							if(CodeMap::getInstance()->getValue(keyCode).compare("") == 0)
								value = "OTHER";
							else
								value = CodeMap::getInstance()->getValue(keyCode);
						}
						return value;
                    }
                    
					bool LocalAgent::getEstado(){return estado;}

					void LocalAgent::setEstado(bool estado){this->estado = estado;}

					int LocalAgent::getPressedKey(){return pressedKey;}
					
					void LocalAgent::setTuner(ITuner* tuner){this->tuner = tuner;}
					
					void LocalAgent::setPressedKey(int pressedKey) {
						if(getEstado()) { // verifica se o módulo LocalAgent está ativo

							// Key
							Key* tecla = new Key(Utilitario::getInstance()->IntToString(pressedKey), getKeyName(pressedKey));
							
							// Volume
							Volume* volum = new Volume(0, false);

							// Channel
							Channel* canal;
							if(tuner != NULL) {
								IChannel* channel = tuner->getCurrentInterface()->getCurrentChannel();
								canal = new Channel(channel->getId(), channel->getName());
							}
							else {
								canal = new Channel(0,"NCL_LOCAL");
							}

							//Program Program(int age, int code, Channel* channel, string name, string genre, string subGenre);
							Program* programa = new Program(0, 0, canal, "---", "---", "---");

							// Iteraction
							Utilitario* util = Utilitario::getInstance();
							string time = util->getAno() + "/" + util->getMes() + "/" + util->getDia() + "T" + util->getHora() + ":" + util->getMinuto() + ":" + util->getSegundo();
							Iteraction* iteracao = new Iteraction(programa, volum, tecla, time, tecla->getType());
							addIteraction(iteracao);
						}
					}
					
					void LocalAgent::setPrimaryKey() {
		        		vector<string> datag;
		        		vector<string> headg;
		        		db->query("SELECT idIteraction from iteraction;", &headg, &datag);
		        		if(!datag.empty()) {
		            		int	maior = atoi(datag.back().c_str());
							Key::setIdKey(maior + 1);
							Volume::setIdVolume(maior + 1);
							Channel::setIdChannel(maior + 1);
							Program::setIdProgram(maior + 1);
							Iteraction::setIdIteraction(maior + 1);
							Meta::setIdMeta(maior + 1);
						}
		        		vector<string> datac;
		        		vector<string> headc;
						db->query("SELECT idContext from context;", &headc, &datac);
						if(!datac.empty()) {
							int maior = atoi(datac.back().c_str());
							Context::setIdContext(maior);
						}
						vector<string> datam;
		        		vector<string> headm;
						db->query("SELECT idMedia from media;", &headm, &datam);
						if(!datam.empty()) {
							int maior = atoi(datam.back().c_str());
							Media::setIdMedia(maior + 1);
						}
						vector<string> datao;
		        		vector<string> heado;
						db->query("SELECT idDocument from document;", &heado, &datao);
						if(!datao.empty()) {
							int maior = atoi(datao.back().c_str());
							NclStateMachine::setIdNclStateMachine(maior + 1);
							Document::setIdDocument(maior + 1);
						}
					}
					
                }
            }
        }
    }
}

