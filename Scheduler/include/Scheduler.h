#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <vector>

using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace recommender {
                    class Scheduler {
                    public:

                        /**
                         * Construtor.
                         * Devolve uma instância de Scheduler.
                         */
                        Scheduler();

                        /**
                         * Destrutor.
                         */
                        ~Scheduler();

                        /**
                         * Agenda a execução de uma aplicação.
                         * @param lib nome da biblioteca que contém a aplicação a ser executada.
                         * @param constructor construtor da aplicação que devolve uma instância para a interface SchedulerItem.
                         * @param destructor destrutor da aplicação.
                         * @param params parâmetros para a aplicação.
                         * @param begin data e horário para iniciar a execução da aplicação (formato: dd/MM/aaaa-hh:mm:ss).
                         * @param end data e horário para o fim da execução da aplicação (formato: dd/MM/aaaa-hh:mm:ss).
                         * @param period periodicidade do agendamento: 1 - uma única vez; 2 - uma vez por dia; 3 - indefinidamente; 4 - uma vez por semana; 5 - uma vez por mês.
                         * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                         */
                        bool schedule(const char* lib, const char* constructor, const char* destructor, vector<char*>* params, const char* begin, const char* end, int period);

                        /**
                         * Carrega para a memória as informações de agendamentos armazenadas em arquivo.
                         * @param file arquivo XML com informações de agendamentos.
                         * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                         */
                        bool load(const char* file);

                        /**
                         * Salva em arquivo as informações de agendamentos em memória.
                         * @param file arquivo XML para destino das informações de agendamentos.
                         * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                         */
                        bool store(const char* file);

                    };
                }
            }
        }
    }
}

#endif

