#ifndef _IMININGALGORITHMAPRIORI_H_
#define _IMININGALGORITHMAPRIORI_H_

#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <cmath>

#include <ginga/recommender/database/Database.h>
#include <ginga/recommender/utils/Utils.h>
#include <ginga/recommender/miningalgorithm/MiningAlgorithm.h>
#include <ginga/recommender/miningalgorithm/Apriori_Trie.hpp>
#include <ginga/recommender/miningalgorithm/Apriori.hpp>
#include <ginga/recommender/miningalgorithm/common.hpp>
#include <ginga/recommender/miningalgorithm/Input_Output_Manager.hpp>
#include <ginga/recommender/miningalgorithm/Trie.hpp>
#include <ginga/recommender/miningalgorithm/SI.h>
#include <ginga/recommender/miningalgorithm/User.h>

/*#include <ginga/recommender/database/Database.h>
#include <ginga/recommender/utils/Utils.h>
#include "MiningAlgorithm.h"
#include "apriori23/Apriori_Trie.hpp"
#include "apriori23/Apriori.hpp"
#include "apriori23/common.hpp"
#include "apriori23/Input_Output_Manager.hpp"
#include "apriori23/Trie.hpp"
#include "SI.h"
#include "User.h"*/

using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace recommender {

                    class IMiningAlgorithmApriori : public MiningAlgorithm {
                    public:

                        ///define o tipo itemtype utilizado pelo algoritmo apriori para contagem dos elementos
                        typedef unsigned long itemtype;

                        public:
                        /**
                         * Construtor.
                         */
                        IMiningAlgorithmApriori();

                        /**
                         * Destrutor.
                         */
                        ~IMiningAlgorithmApriori();

                        public:
                        /**
                         * Inicializa o algoritmo com os parâmetros especificados.
                         * @param params parâmetros para o algoritmo:
                         *        params[0]: min_sup parametros do algoritmo. Utilizado para desconsiderar poucas interações do usuário.
                         *        params[1]: min_confi Confiabilidade dos dados. Quanto maior esse parâmetro, menos regras são produzidas.
                         *        params[2]: banco de dados tabela contendo a tabela SI (uma abstração para as tabelas EIT e SDT).
                         * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                         */
                        bool setParams(vector<char*> params);

                        /**
                         * Prepara os dados para serem processados pelo algoritmo.
                         * @param data dados a serem pre-processados.
                         * @param delimiter delimitador dos dados.
                         * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                         */
                        bool prepare(const char* data, const char* delimiter);

                        /**
                         * Prepara os dados para serem processados pelo algoritmo.
                         * @param db banco de dados que contém a tabela a ser pre-processada.
                         * @param table tabela com os dados a serem pre-processados.
                         * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                         */
                        bool prepare(Database* db, const char* table);

                        /**
                         * Executa o algoritmo.
                         * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                         */
                        bool start();

                        /**
                         * Pára a execução do algoritmo.
                         * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                         */
                        bool stop();

                        /**
                         * Define a saída dos dados processados.
                         * @param data variável para armazenamento dos dados processados.
                         * @param size tamanho (em bytes) da variável para armazenamento dos dados.
                         * @param delimiter delimitador dos dados.
                         */
                        void setOutput(char* data, long size, const char* delimiter);

                        /**
                         * Define a saída dos dados processados.
                         * @param db banco de dados que contém a tabela onde serão armazenados os dados processados.
                         * @param table tabela onde serão armazenados os dados processados.
                         */
                        void setOutput(Database* db, const char* table);

                        /**
                         * Define a saída dos dados processados.
                         * @param file caminho do arquivo para armazenamento dos dados processados.
                         */
                        void setOutput(const char* file);

                        /**
                         * Executa o algoritmo apriori
                         * @param file arquivo que contém as interações do usuário (comportamento do usuário)
                         * @param min_sup parametros do algoritmo. Utilizado para desconsiderar poucas interações do usuário.
                         * @param min_confi Confiabilidade dos dados. Quanto maior esse parâmetro, menos regras são produzidas.
                         */
                        void execute(string file, double min_sup, double min_confi);
                        /**
                         * O algoritmo apriori produz a saída toda em formato numérico. Esse método mapea a saída do algoritmo
                         * numérico para strings.
                         * @param fileCodeTable tabela usada para o mapeamento string --> numérico
                         * @param fileOutMining arquivo gerado pelo algoritmo apriori (numérico)
                         * @param fileOut arquivo de saída mapeado numérico --> string
                         */
                        void encode(char * fileCodeTable, char * fileOutMining, char * fileOut);

                        void code_file_process(char* code_file_name);
                        void code_file_process_out_Algorithm(char* outAlgorithm);
                        void code_file_process_out(char* outAlgorithm);

                        /**
                         * Adiciona o item decodificado à string que será impressa ao final da execução.
                         * @param item elemento que será escrito.
                         */
                        void write_decoded_item(itemtype item);

                        /**
                         * Decodifica o arquivo que contém os dados da interação do usuário com o middleware.
                         */
                        void decode();

                        /**
                         * Recupera as informações das tabelas EIT e SDT
                         * @param nomeCache o nome do cache que contém as informações das tabelas EIT & SDT
                         * @return um ponteiro para um vector contendo os registros recuperados.
                         */
                        vector<si*> * restoreCacheInformation(string nomeCache);

                        /**
                         * Converte informações relacionais do banco de dados em objetos
                         * da classe User.
                         * @return um ponteiro de vector <User*>
                         */
                        vector<User*> * getUserInformation();

                        /**
                         * Converte objetos da classe User em strings para serem
                         * submetidas aos algoritmos de mineração
                         * @param user um ponteiro para um vector<User *> *user
                         * @return <b> string </b> com os objetos da classe User convertidos no padrão basket_file do
                         * algoritmo apriori.
                         */
                        vector<std::string> prepareDataUser(vector<User *> *user);

                        /**
                         * Realiza a persistência das informações processadas pelo
                         * método prepareDataUser
                         * @param user recebe uma string com os dados a serem persistidos na
                         * @param file recebe uma string que informa o path e nome do arquivo a ser
                         *        criado em disco.
                         * em disco.
                         */
                        void saveUserInformation(vector<string> user, string file);

                        /**
                         * Monta objetos da classe User a partir das tuplas passadas por referência.
                         * @param retornaConsulta : dados contidos nas tuplas.
                         * @param retornaHead : nome das tabelas.
                         * @return vetor com os usuários.
                         */
                        vector<User *> * mountObject(vector<string> retornaConsulta, vector<string> retornaHead);

                        /**
                         * Calcular o período do dia em que um programa foi sintonizado.
                         * Exemplo: Programa sintonizado das 09:30hs até 11:00hs.
                         * retorna: manhã.
                         * @param dataColectedByUser string contendo as informações do programa, horário, data.
                         * @return string informando em qual horário o programa foi sintonizado
                         */
                        string getValueByTime(string dataColectedByUser);

                        /**
                         * Calcular o percentual de tempo que o usuário sintonizou
                         * determinando programa.
                         * @param dataColectedByUser string contendo as informações do programa, horário, data.
                         * @param dataCacheByTable vector <si*> * um ponteiro para os dados restaurados da tabela EIT & SDT.
                         * @return string informando: pouco, medio,alto
                         */
                        string getPercentualByTime(const string dataColectedByUser, vector <si*> * dataCacheByTable);

                        /**
                         * Calcular o percentual de tempo que o usuário sintonizou determinando programa.
                         * @param nomePrograma string contendo as informações do programa para comparação com as informações da EIT no cache.
                         * @param tempoUsuario tempoUsuario double contendo o tempo em que o usuário ficou sintonizado no programa
                         * @param dataInicio string contendo a data atual para recuperar os programas da tabela EIT & SDT
                         * @param dataCacheByTable vector si* um ponteiro para os dados restaurados da tabela EIT & SDT.
                         * @return vector contendo o tempo que o usuário assitiu cada programa.
                         */
                        vector<string> getPercentualByTime(const string nomePrograma, double tempoUsuario, string dataInicio, vector <si*> * dataCacheByTable);


                        /**
                         * Quando não existirem dados suficiente para o algoritmo apriori processar
                         * é necessário preencher o arquivo de saída lua com informações randômicas.
                         * @param min informa o menor valor gerado
                         * @param max informa o máximo valor gerado
                         * @return <b> int </b> onde esse valor é maior igual ao minimo e menor igual ao máximo.
                         */
                        int getRandom(int min, int max);

                        /**
                         * Retorna uma string representando o tempo que usuário
                         * permaneceu conectado em determinado programa.
                         * @param duration double que representa o percentual de tempo em
                         * que o usuário ficou conectado em um determinado programa.
                         *
                         * @return <b> string </b> que pode ser: pouco, medio, alto
                         */
                        static std::string verifyDuration(double duration);

                        /**
                         * Leitura de arquivo texto do disco
                         * @param arquivo nome do arquivo a ser lido
                         * @return número de bytes lidos do arquivo
                         */
                        int ReadFile(char *arquivo);

                        vector<string> getLine();
                        void setLine(vector<string> line);

                        /**
                         * Busca na tabela EIT o programa recomendado
                         * @param genero string que informa o genero do programa
                         * @param subgenero string que informa o sub-genero do programa
                         * @param per_dia string que informa o dia para procurar na tabela EIT
                         * @param programacao a tabela EIT
                         * @return
                         */
                        vector<string> searchProgrambyId(string genero, string subgenero, string per_dia, vector<string> *programacao);
                        //static int count;
                        int count;
                        vector<string> line;

                        /**
                         * retorna o Genero do Programa a partir do codigo
                         * @param gen string com o genero
                         * @return o genero, por exemplo, Jornalismo
                         **/

                        string getGenProgram(string gen, string subgen);

                        /**
                         * Evita que ocorram duplicações nas recomedações
                         */
                        //static void controlDuplicate();
                        void controlDuplicate();

                        /**
                         * Salva a recomendação no arquivo de formato lua.
                         * Exemplo de arquivo salvo:
                         * NOITE{
                         * Genero Cod = "0x0",
                         * SubGenero Cod = "0x02",
                         * Inicio = "2008-03-12 23:32:00",
                         * Fim = "2008-03-13 00:29:00",
                         * Programa = "REPORTER RECORD SG",
                         * Emissora = "006",
                         * }
                         *
                         * @param arquivo nome do arquivo para salvar as recomendações
                         * @param user recomendações ofertadas para o usuário
                         * @param horario que pode ser: madrugada, manha, tarde e noite
                         */
                        void saveRecommender(string arquivo, vector<string> *user, string horario);

                        /**
                         * Prepara o banco para receber as novas recomendacoes
                         */
                        void prepareToSaveRecommender();

                        //Constantes com os valores para as Durações.
                        static const int DESPREZIVEL = 10;
                        static const int POUCO = 30;
                        static const int MEDIO = 65;
                        static const int ALTO = 100;

                        //Atributos para o método prepare (usando variável).
                        string* inData;
                        string* inDataDelimiter;

                        //Atributos para o método prepare (usando banco de dados).
                        Database* inDB;
                        string* inTableName;

                        //Atributos para o método setOutput (usando variável).
                        char* outDataBuffer;
                        long* outDataSize;
                        string* outDataDelimiter;

                        //Atributos para o método setOutput (usando banco de dados).
                        Database* outDB;
                        string* outTableName;

                        //Atributos para o método setOutput (usando arquivo).
                        string* outFileName;

                        //Atributos para o método setParams, usados pelo algoritmo Apriori.
                        double minSup;
                        double minConfi;
                        string siCacheFileName;

                        //Armazena os dados do usuário que serão minerados pelo algoritmo.
                        vector<User*>* userInfo;

                        ///abstração para o arquivo que contém os dados da interação do usuário
                        FILE *code_file_algorithm;
                        /**
                         * abstração para o arquivo de saída produzido pelo algoritmo apriori. O arquivo de saída ainda não está
                         * pronto, necessitando ser submetido aos algoritmos do módulo FilterAgent
                         * @see br.ufscar.lince.ginga.recommender.filteragent
                         */
                        ofstream code_file_out;

                        ///conta a quantidade de itens (programas) de acordo com a base de dados do usuário
                        map<itemtype, string> codearray;
                        string imprimir;
                        ofstream outfile;
                    };

                }
            }
        }
    }
}

#endif

