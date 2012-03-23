#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include "argparse.hpp"
#include "datadefs.hpp"
#include "treedata.hpp"
#include "utils.hpp"

using namespace std;
using datadefs::num_t;

namespace options {

  void printHeader(ostream& output) {

    output << endl;
    output << "-----------------------------------------------------" << endl;
    output << "|  RF-ACE version:  1.0.4, March 20th, 2012         |" << endl;
    output << "|    Project page:  http://code.google.com/p/rf-ace |" << endl;
    output << "|     Report bugs:  timo.p.erkkila@tut.fi           |" << endl;
    output << "-----------------------------------------------------" << endl;
    output << endl;
  }

  void printHelpHint() {
    cout << endl;
    cout << "To get started, type \"-h\" or \"--help\"" << endl;
  }
  
  void printFilterOverview() {
    cout << "PROGRAM: RF-ACE-FILTER" << endl << endl;
    cout << " Given target feature and input data, applies decision tree ensemble " << endl;
    cout << " learning with RF-ACE to identify statistically significant predictors." << endl << endl;
  }
  
  void printPredictorBuilderOverview() {
    cout << "PROGRAM: RF-ACE PREDICTOR BUILDER" << endl << endl;
    cout << " Given target feature and input data, builds a Random Forest (RF) or "
	 << " Gradient Boosting Tree (GBT) predictor" << endl << endl;
  }
  
  void printPredictorOverview() {
    cout << "PROGRAM: RF-ACE PREDICTOR" << endl << endl;
    cout << " Makes predictions given a model and novel data." << endl << endl;
  }
  
  // Default general configuration
  const bool   GENERAL_DEFAULT_PRINT_HELP = false;
  const char   GENERAL_DEFAULT_DATA_DELIMITER = '\t';
  const char   GENERAL_DEFAULT_HEADER_DELIMITER = ':';
  const size_t GENERAL_DEFAULT_MIN_SAMPLES = 5;
  const int    GENERAL_DEFAULT_SEED = -1;

  // Statistical test default configuration
  const size_t ST_DEFAULT_N_PERMS = 20;
  const num_t  ST_DEFAULT_P_VALUE_THRESHOLD = 0.05;

  // Random Forest default configuration
  const size_t RF_DEFAULT_N_TREES = 100;
  const size_t RF_DEFAULT_M_TRY = 0;
  const size_t RF_DEFAULT_N_MAX_LEAVES = 100;
  const size_t RF_DEFAULT_NODE_SIZE = 5;
  const num_t  RF_DEFAULT_IN_BOX_FRACTION = 1.0;
  const num_t  RF_DEFAULT_SAMPLE_WITH_REPLACEMENT = true;
  const bool   RF_DEFAULT_USE_CONTRASTS = true;
  const bool   RF_DEFAULT_IS_RANDOM_SPLIT = true;
  const num_t  RF_DEFAULT_SHRINKAGE = 0.0;

  // Gradient Boosting Trees default configuration
  const size_t GBT_DEFAULT_N_TREES = 100;
  const size_t GBT_DEFAULT_M_TRY = 0;
  const size_t GBT_DEFAULT_N_MAX_LEAVES = 6;
  const size_t GBT_DEFAULT_NODE_SIZE = 5;
  const num_t  GBT_DEFAULT_IN_BOX_FRACTION = 0.5;
  const num_t  GBT_DEFAULT_SAMPLE_WITH_REPLACEMENT = false;
  const bool   GBT_DEFAULT_USE_CONTRASTS = false;
  const bool   GBT_DEFAULT_IS_RANDOM_SPLIT = false;
  const num_t  GBT_DEFAULT_SHRINKAGE = 0.1;

  // Determines the amount of indentation in help print-outs
  const size_t maxWidth = 20;
  
  struct General_options {

    bool printHelp; const string printHelp_s; const string printHelp_l;
    string input; const string input_s; const string input_l;
    string output; const string output_s; const string output_l;
    string targetStr; const string targetStr_s; const string targetStr_l;
    string whiteList; const string whiteList_s; const string whiteList_l;
    string blackList; const string blackList_s; const string blackList_l;
    string log; const string log_s; const string log_l;
    char dataDelimiter; const string dataDelimiter_s; const string dataDelimiter_l;
    char headerDelimiter; const string headerDelimiter_s; const string headerDelimiter_l;
    size_t pruneFeatures; const string pruneFeatures_s; const string pruneFeatures_l;
    int seed; string seed_s; string seed_l;

    General_options():
      printHelp(GENERAL_DEFAULT_PRINT_HELP),printHelp_s("h"),printHelp_l("help"),
      input(""),input_s("I"),input_l("input"),
      output(""),output_s("O"),output_l("output"),
      targetStr(""),targetStr_s("i"),targetStr_l("target"),
      whiteList(""),whiteList_s("W"),whiteList_l("whitelist"),
      blackList(""),blackList_s("B"),blackList_l("blacklist"),
      log(""),log_s("L"),log_l("log"),
      dataDelimiter(GENERAL_DEFAULT_DATA_DELIMITER),dataDelimiter_s("D"),dataDelimiter_l("data_delim"),
      headerDelimiter(GENERAL_DEFAULT_HEADER_DELIMITER),headerDelimiter_s("H"),headerDelimiter_l("head_delim"),
      pruneFeatures(GENERAL_DEFAULT_MIN_SAMPLES),pruneFeatures_s("X"),pruneFeatures_l("prune_features"),
      seed(GENERAL_DEFAULT_SEED),seed_s("S"),seed_l("seed") {}
    
    void loadUserParams(const int argc, char* const argv[]) {
      
      // Read the user parameters ...
      ArgParse parser(argc,argv);
      
      parser.getFlag(printHelp_s, printHelp_l, printHelp);
      parser.getArgument<string>(input_s, input_l, input);
      parser.getArgument<string>(targetStr_s, targetStr_l, targetStr);
      parser.getArgument<string>(output_s, output_l, output);
      parser.getArgument<string>(whiteList_s, whiteList_l, whiteList);
      parser.getArgument<string>(blackList_s, blackList_l, blackList);
      parser.getArgument<string>(log_s,log_l,log);
      string dataDelimiter,headerDelimiter;
      parser.getArgument<string>(dataDelimiter_s, dataDelimiter_l, dataDelimiter);
      parser.getArgument<string>(headerDelimiter_s, headerDelimiter_l, headerDelimiter);
      parser.getArgument<size_t>(pruneFeatures_s, pruneFeatures_l, pruneFeatures);
      stringstream ss(dataDelimiter);
      ss >> dataDelimiter;

      ss.clear();
      ss.str("");
      ss << headerDelimiter;
      ss >> headerDelimiter;

      parser.getArgument<int>(seed_s, seed_l, seed);
      
      // If no seed was provided, generate one
      if ( seed == GENERAL_DEFAULT_SEED ) {
	seed = utils::generateSeed();
      }
      
    }

    void help() {

      cout << "REQUIRED ARGUMENTS:" << endl;
      cout << " -" << input_s << " / --" << input_l << setw( maxWidth - input_l.size() )
           << " " << "Input data file, either AFM or ARFF" << endl;
      cout << " -" << targetStr_s << " / --" << targetStr_l << setw( maxWidth - targetStr_l.size() )
           << " " << "Target, specified as integer or string that is to be matched with the content of input" << endl;
      cout << " -" << output_s << " / --" << output_l << setw( maxWidth - output_l.size() )
           << " " << "Output file" << endl;
      cout << endl;
      
      cout << "OPTIONAL ARGUMENTS:" << endl;
      cout << " -" << log_s << " / --" << log_l << setw( maxWidth - log_l.size() )
           << " " << "Log output file" << endl;
      cout << " -" << whiteList_s << " / --" << whiteList_l << setw( maxWidth - whiteList_l.size() )
           << " " << "White list of features to be included in the input file(s)." << endl;
      cout << " -" << blackList_s << " / --" << blackList_l << setw( maxWidth - blackList_l.size() )
           << " " << "Black list of features to be excluded from the input file(s)." << endl;
      cout << " -" << dataDelimiter_s << " / --" << dataDelimiter_l << setw( maxWidth - dataDelimiter_l.size() )
           << " " << "Data delimiter (default \\t)" << endl;
      cout << " -" << headerDelimiter_s << " / --" << headerDelimiter_l << setw( maxWidth - headerDelimiter_l.size() )
           << " " << "Header delimiter that separates the N and C symbols in feature headers from the rest (default " << GENERAL_DEFAULT_HEADER_DELIMITER << ")" << endl;
      cout << " -" << pruneFeatures_s << " / --" << pruneFeatures_l << setw( maxWidth - pruneFeatures_l.size() )
           << " " << "Features with less than n ( default " << GENERAL_DEFAULT_MIN_SAMPLES << " ) samples will be removed" << endl;
      cout << " -" << seed_s << " / --" << seed_l << setw( maxWidth - seed_l.size() ) 
	   << " " << "Seed (positive integer) for the Mersenne Twister random number generator" << endl;
      cout << endl;

    }
    
  };

  struct StochasticForest_options {

    size_t  nTrees; string nTrees_s; string nTrees_l;
    size_t  mTry; string mTry_s; string mTry_l;
    size_t  nMaxLeaves; string nMaxLeaves_s; string nMaxLeaves_l;
    size_t  nodeSize; string nodeSize_s; string nodeSize_l;
    //num_t   inBoxFraction; string inBoxFraction_s; string inBoxFraction_l;
    //bool    sampleWithReplacement; string sampleWithReplacement_s; string sampleWithReplacement_l;
    //bool    useContrasts; string useContrasts_s; string useContrasts_l;
    //bool    isRandomSplit; string isRandomSplit_s; string isRandomSplit_l;
    num_t   shrinkage; string shrinkage_s; string shrinkage_l;

    StochasticForest_options():
      nTrees_s("n"),nTrees_l("ntrees"),
      mTry_s("m"),mTry_l("mtry"),
      nMaxLeaves_s("a"),nMaxLeaves_l("nmaxleaves"),
      nodeSize_s("s"),nodeSize_l("nodesize"),
      //inBoxFraction_s("f"),inBoxFraction_l("inboxfraction"),
      //sampleWithReplacement_s("w"),sampleWithReplacement_l("samplewithreplacement"),
      //useContrasts_s("c"),useContrasts_l("usecontrasts"),
      //isRandomSplit_s("r"),isRandomSplit_l("randomsplit"),
      shrinkage_s("k"),shrinkage_l("shrinkage") { setRFDefaults(); }

    void loadUserParams(const int argc, char* const argv[]) {

      ArgParse parser(argc,argv);

      parser.getArgument<size_t>( "n", "ntrees",                nTrees );
      parser.getArgument<size_t>( "m", "mtry",                  mTry );
      parser.getArgument<size_t>( "a", "nmaxleaves",            nMaxLeaves );
      parser.getArgument<size_t>( "s", "nodesize",              nodeSize );
      //parser.getArgument<num_t>(  "f", "inboxfraction",         inBoxFraction );
      //parser.getFlag(             "w", "samplewithreplacement", sampleWithReplacement );
      //parser.getFlag(             "c", "useContrasts",          useContrasts );
      //parser.getFlag(             "r", "randomsplit",           isRandomSplit );
      parser.getArgument<num_t>(  "k", "shrinkage",             shrinkage );
      
    }
    
    void setRFDefaults() {

      nTrees                = RF_DEFAULT_N_TREES;
      mTry                  = RF_DEFAULT_M_TRY;
      nMaxLeaves            = RF_DEFAULT_N_MAX_LEAVES;
      nodeSize              = RF_DEFAULT_NODE_SIZE;
      //inBoxFraction         = RF_DEFAULT_IN_BOX_FRACTION;
      //sampleWithReplacement = RF_DEFAULT_SAMPLE_WITH_REPLACEMENT;
      //useContrasts          = RF_DEFAULT_USE_CONTRASTS;
      //isRandomSplit         = RF_DEFAULT_IS_RANDOM_SPLIT;
      shrinkage             = RF_DEFAULT_SHRINKAGE;

    }

    void setGBTDefaults() {

      nTrees                = GBT_DEFAULT_N_TREES;
      mTry                  = GBT_DEFAULT_M_TRY;
      nMaxLeaves            = GBT_DEFAULT_N_MAX_LEAVES;
      nodeSize              = GBT_DEFAULT_NODE_SIZE;
      //inBoxFraction         = GBT_DEFAULT_IN_BOX_FRACTION;
      //sampleWithReplacement = GBT_DEFAULT_SAMPLE_WITH_REPLACEMENT;
      //useContrasts          = GBT_DEFAULT_USE_CONTRASTS;
      //isRandomSplit         = GBT_DEFAULT_IS_RANDOM_SPLIT;
      shrinkage             = GBT_DEFAULT_SHRINKAGE;

    }

    void help() {
      cout << "OPTIONAL ARGUMENTS -- STOCHASTIC FOREST:" << endl;
      
      cout << " -" << nTrees_s << " / --" << nTrees_l << setw( maxWidth - nTrees_l.size() )
           << " " << "Number of trees in the forest" << endl;
      cout << " -" << mTry_s << " / --" << mTry_l << setw( maxWidth - mTry_l.size() )
           << " " << "Fraction of randomly drawn features per node split" << endl;
      cout << " -" << nMaxLeaves_s << " / --" << nMaxLeaves_l << setw( maxWidth - nMaxLeaves_l.size() )
           << " " << "Maximum number of leaves per tree" << endl;
      cout << " -" << nodeSize_s << " / --" << nodeSize_l << setw( maxWidth - nodeSize_l.size() )
           << " " << "Minimum number of train samples per node, affects tree depth" << endl;
      //cout << " -" << inBoxFraction_s << " / --" << inBoxFraction_l << setw( maxWidth - inBoxFraction_l.size() )
      //     << " " << "Fraction of bootstrap samples drawn for training" << endl;
      cout << " -" << shrinkage_s << " / --" << shrinkage_l << setw( maxWidth - shrinkage_l.size() )
           << " " << "[GBT only] Shrinkage applied to evolving the residual" << endl;
      cout << endl;
    }
    
  };
  
  struct Predictor_options {
    
    string forest;
    const string forest_s;
    const string forest_l;
    
    Predictor_options():
      forest(""),forest_s("F"),forest_l("forest") {}
    
    void loadUserParams(const int argc, char* const argv[]) {
      
      // Read the user parameters ...
      ArgParse parser(argc,argv);
      
      parser.getArgument<string>(forest_s, forest_l, forest);
      
    }
    
    void help() {
      
      cout << "REQUIRED ARGUMENTS -- PREDICTOR:" << endl;
      cout << " -" << forest_s << " / --" << forest_l << setw( maxWidth - forest_l.size() )
           << " " << "Forest predictor stored in a .sf file" << endl;
      cout << endl; 
    }
    
  };
  
  struct PredictorBuilder_options {
    
    bool isGBT; string isGBT_s; string isGBT_l;
    bool isRF; string isRF_s; string isRF_l;
    
    PredictorBuilder_options():
      isGBT(false), isGBT_s("G"), isGBT_l("GBT"),
      isRF(false), isRF_s("R"), isRF_l("RF") {}
    
    void loadUserParams(const int argc, char* const argv[]) {
      
      ArgParse parser(argc,argv);
      
      parser.getFlag(isGBT_s, isGBT_l, isGBT);
      parser.getFlag(isRF_s,  isRF_l,  isRF);

      // If neither flags were raised, set default to RF
      if ( ! ( isGBT || isRF ) ) {
        isRF = true;
      }

      // Raise error if both flags were raised
      if ( isRF && isGBT ) {
        cerr << "You cannot choose both RF and GBT for predictor building" << endl;
        exit(1);
      }

    }

    void help() {

      cout << "OPTIONAL ARGUMENTS -- PREDICTOR BUILDER:" << endl;
      cout << " -" << isGBT_s << " / --" << isGBT_l << setw( maxWidth - isGBT_l.size() )
           << " " << "Set this flag if you prefer GBT as the predictor model" << endl;
      cout << " -" << isRF_s << " / --" << isRF_l << setw( maxWidth - isRF_l.size() )
           << " " << "Set this flag if you prefer RF as the predictor model (default)" << endl;
      cout << endl;
    }

  };

  struct StatisticalTest_options {

    size_t nPerms; string nPerms_s; string nPerms_l;
    num_t pValueThreshold; string pValueThreshold_s; string pValueThreshold_l;

    StatisticalTest_options():
    nPerms(ST_DEFAULT_N_PERMS), nPerms_s("p"), nPerms_l("nperms"),
    pValueThreshold(ST_DEFAULT_P_VALUE_THRESHOLD), pValueThreshold_s("t"), pValueThreshold_l("pthreshold") {
      
    }

    void loadUserParams(const int argc, char* const argv[]) {

      ArgParse parser(argc,argv);

      parser.getArgument<size_t>(nPerms_s, nPerms_l, nPerms);
      parser.getArgument<num_t>(pValueThreshold_s,  pValueThreshold_l,  pValueThreshold);

      if ( nPerms < 6 ) {
	cerr << "Use more than 5 permutations in statistical test!" << endl;
	exit(1);
      }

      if ( pValueThreshold < 0.0 || pValueThreshold > 1.0 ) {
	cerr << "P-value threshold in statistical test must be within 0...1" << endl;
	exit(1);
      }

    }
    
    void help() {
      
      cout << "OPTIONAL ARGUMENTS -- STATISTICAL TEST:" << endl;
      cout << " -" << nPerms_s << " / --" << nPerms_l << setw( maxWidth - nPerms_l.size() )
	   << " " << "Number of permutations in statistical test (default " << ST_DEFAULT_N_PERMS << ")" << endl;
      cout << " -" << pValueThreshold_s << " / --" << pValueThreshold_l << setw( maxWidth - pValueThreshold_l.size() ) 
	   << " " << "P-value threshold in statistical test (default " << ST_DEFAULT_P_VALUE_THRESHOLD << ")" << endl;
      cout << endl;
      
    }
    
  };
  
}

#endif