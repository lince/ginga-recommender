#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <fstream>
#include "Apriori.hpp"
using namespace std;


/// This procedure displays the usage of the program.

void usage()
{
   cerr << "\nUsage: apriori basketfile outcomefile min_supp [min_conf]\n";
   cerr << "\n basketfile\t    file, that contains the baskets of itemcodes";
   cerr << "\n outcomefile\t    file to write the outcome";
   cerr << "\n min_supp\t    support threshold";
   cerr << "\n min_conf\t    confidence threshold";

   cerr << "\n\nFile formats:";
   cerr << "\n\nThe basket file is a plan text file. Each row represents a basket. ";
   cerr << "A basket is a set of items seperated by a nonnumeric character (for example white space, comma, colon, etc.). ";
   cerr << "An item is represented by its code which is an integer number greater than or equal to 0.";
   cerr << "\nFor more options please check the configuration file: .apriori_config.";
   cerr << "\n\nHave a succesful mining ;-)";
   cerr << "\n\n\n\t\t\t\t\tFerenc Bodon\n\n";
}
void process_config_file(bool& quiet, bool& store_input, unsigned long& size_threshold)
{
   ifstream config_file(".apriori_config");
   quiet=false;
   store_input=true;

   if( !config_file ) 
   {
      cerr << "Warning: There is no configuration file (.apriori_config)!\n";
      cerr << "Default values are used!"<<flush;
   }
   else
   {
      char temp_string[256];
      config_file.getline(temp_string, 256);
      config_file.getline(temp_string, 256);
      config_file>>quiet;
      if( config_file.fail() )
      { 
	 cerr<<"\nWarning: Failed to read in quiet value from the configuration file!";
	 cerr<<"\nDefault value (false) is used.";
      }
      config_file.getline(temp_string, 256);
      config_file>>store_input;
      if( config_file.fail() )
      { 
	 cerr<<"\nWarning: Failed to read in store_input value from the configuration file!";
	 cerr<<"\nDefault value (true) is used.";
      }
      config_file.getline(temp_string, 256);
      config_file>>size_threshold;
      if( config_file.fail() )
      { 
	 cerr<<"\nWarning: Failed to read in size_threshold value from the configuration file!";
	 cerr<<"\nDefault value (0) is used.";
      }
      config_file.close();
   }
}

/// This procedure processes the arguments.
int process_arguments( int argc, char *argv[], ifstream& basket_file, 
		       double& min_supp, double& min_conf )
{
   if ( argc < 4 )
   {
     usage();
     cerr<<"\nError! There are 3 mandatory arguments!"<<flush;
     return 1;
   }
   basket_file.open(argv[1]);
   if( !basket_file )
   {
      usage();
      cerr << "\nError! The basket file can not be read!"<< flush;
      return 1;
   }
   
   min_supp = atof(argv[3]);
   if ( min_supp <= 0 || min_supp > 1 )
   {
      usage();
      cerr<<"\nError!\n min_supp should be in the interval (0,1].\n"<<flush;
      return 1;
   }
       
   if ( argc == 5 )
   {
      min_conf = atof(argv[4]);
      if ( min_conf <= 0 || min_conf > 1 )
      {
	 usage();
         cerr << "\nError!\n min_conf should be in the interval (0,1].\n"<<flush;
         return 1;
      }
   }
   else min_conf = 0;
   return 0;
}

int main( int argc, char *argv[] )
{
   double min_supp,
          min_conf;
   bool store_input = true,
              quiet = false;
   unsigned long size_threshold;
   ifstream basket_file;
   
   process_config_file(quiet, store_input, size_threshold);
   if( process_arguments( argc, argv, basket_file, min_supp, min_conf ) ) return 1;
      
   if( !quiet )
   {
      cout << "\n********************************************************************";
      cout << "\n***                                                              ***";
      cout << "\n***                Trie based APRIORI algorithm                  ***";
      cout << "\n***                       version: 2.3.2                         ***";
      cout << "\n***                                                              ***";
      cout << "\n***       Implemented by: Ferenc Bodon (bodon@cs.bme.hu)         ***";
      cout << "\n***                                                              ***";
      cout << "\n********************************************************************\n\n";
   }
  
   //criou o objeto
   Apriori apriori( basket_file, argv[2], store_input );

   apriori.APRIORI_alg( min_supp, min_conf, quiet, size_threshold );
   basket_file.close();
   return 0;
}

