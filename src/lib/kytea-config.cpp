/*
* Copyright 2009, KyTea Development Team
* 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*     http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <kytea/kytea-config.h>
#include "config.h"

using namespace kytea;
using namespace std;

// set the type of the input corpus
void KyteaConfig::setIOFormat(const char* str, CorpForm & cf) {
    if(!strcmp(str, "full"))      { cf = CORP_FORMAT_FULL; }
    else if(!strcmp(str, "part")) { cf = CORP_FORMAT_PART; }
    else if(!strcmp(str, "conf")) { cf = CORP_FORMAT_PROB; }
    else if(!strcmp(str, "prob")) { cf = CORP_FORMAT_PROB; }
    else if(!strcmp(str, "raw"))  { cf = CORP_FORMAT_RAW;  }
    else
        THROW_ERROR("Unsupported corpus IO format '" << str << "'");
}


void KyteaConfig::parseTrainCommandLine(int argc, char ** argv) {
    for(int i = 1; i < argc; i++)
        i += parseTrainArg(argv[i], (i == argc-1?NULL:argv[i+1]));
}


void KyteaConfig::parseRunCommandLine(int argc, char ** argv) {
    for(int i = 1; i < argc; i++)
        i += parseRunArg(argv[i], (i == argc-1?NULL:argv[i+1]));
    // load the model file if it has not been specified at the command line
    if(model_.length() == 0) {
        if(getenv("KYTEA_MODEL"))
            model_ = getenv("KYTEA_MODEL");
        else {
            model_ = PKGDATADIR;
            model_ += "/model.bin";
        }
    }
}


void KyteaConfig::printUsage() {
    if(onTraining_) {
        // print the training usage
        cerr << 
"train-kytea:" << endl << 
"  A program to train models for KyTea" << endl <<
"" << endl <<
"Input/Output Options: " << endl <<
"  -encode  The text encoding to be used (utf8/euc/sjis; default: utf8)" << endl <<
"  -full    A fully annotated training corpus (can be used multiple times)" << endl <<
"  -part    A partially annotated training corpus (can be used multiple times)" << endl <<
"  -conf    A confidence annotated training corpus (can be used multiple times)" << endl <<
"  -dict    A dictionary file (one 'word/pron' entry per line, multiple possible)" << endl <<
"  -subword A file of subword units. This will enable unknown word PE." << endl <<
"  -model   The file to write the trained model to" << endl <<
"  -modtext Print a text model (instead of the default binary)" << endl <<
"Model Training Options (basic)" << endl <<
"  -nows    Don't train a word segmentation model" << endl <<
"  -notags  Skip the training of tagging, do only word segmentation" << endl <<
"  -global  Train the nth tag with a global model (good for POS, bad for PE)" << endl <<
"  -debug   The debugging level during training (0=silent, 1=normal, 2=detailed)" << endl <<
"Model Training Options (for advanced users): " << endl <<
"  -charw   The character window to use for WS (3)" << endl <<
"  -charn   The character n-gram length to use for WS for WS (3)" << endl <<
"  -typew   The character type window to use for WS (3)" << endl <<
"  -typen   The character type n-gram length to use for WS for WS (3)" << endl <<
"  -dictn   Dictionary words greater than -dictn will be grouped together (4)" << endl <<
"  -unkn    Language model n-gram order for unknown words (3)" << endl <<
"  -eps     The epsilon stopping criterion for classifier training" << endl <<
"  -cost    The cost hyperparameter for classifier training" << endl <<
"  -nobias  Don't use a bias value in classifier training" << endl <<
"  -solver  The solver (1=SVM, 7=logistic regression, etc.; default 1,"<<endl<<
"           see LIBLINEAR documentation for more details)" << endl <<
"Format Options (for advanced users): " << endl <<
"  -wordbound The separator for words in full annotation (\" \")" << endl <<
"  -tagbound  The separator for tags in full/partial annotation (\"/\")" << endl <<
"  -elembound The separator for candidates in full/partial annotation (\"&\")" << endl <<
"  -unkbound  Indicates unannotated boundaries in partial annotation (\" \")" << endl <<
"  -skipbound Indicates skipped boundaries in partial annotation (\"?\")" << endl <<
"  -nobound   Indicates non-existence of boundaries in partial annotation (\"-\")" << endl <<
"  -hasbound  Indicates existence of boundaries in partial annotation (\"|\")" << endl << endl;
    }
    else {
        // print the testing usage
        cerr << 
"kytea:" << endl << 
"  KyTea, a word segmentation/pronunciation estimation tool" << endl <<
"" << endl <<
"Analysis Options: " << endl <<
"  -model   The model file to use when analyzing text" << endl <<
"  -nows    Don't do word segmentation (raw input cannot be accepted)" << endl <<
"  -notags  Do only word segmentation, no tagging" << endl <<
"  -notag   Skip the tag of the nth tag (n starts at 1)" << endl <<
"  -nounk   Don't estimate the pronunciation of unkown words" << endl <<
"  -unkbeam The width of the beam to use in beam search for unknown words " <<endl<<
"           (default 50, 0 for full search)" << endl <<
"  -debug   The debugging level (0=silent, 1=simple, 2=detailed)" << endl <<
"Format Options: " << endl <<
"  -in      The formatting of the input  (raw/full/part/conf, default raw)" << endl <<
"  -out     The formatting of the output (full/part/conf, default full)" << endl <<
"  -tagmax  The maximum number of tags to print for one word (default 3," << endl <<
"            0 implies no limit)" << endl << 
"  -deftag  A tag for words that cannot be given any tag (for example, "<<endl<<
"           unknown words that contain a character not in the subword dictionary)" << endl << 
"  -unktag  A tag to append to indicate words not in the dictionary" << endl <<
"Format Options (for advanced users): " << endl <<
"  -wordbound The separator for words in full annotation (\" \")" << endl <<
"  -tagbound  The separator for tags in full/partial annotation (\"/\")" << endl <<
"  -elembound The separator for candidates in full/partial annotation (\"&\")" << endl <<
"  -unkbound  Indicates unannotated boundaries in partial annotation (\" \")" << endl <<
"  -skipbound Indicates skipped boundaries in partial annotation (\"?\")" << endl <<
"  -nobound   Indicates non-existence of boundaries in partial annotation (\"-\")" << endl <<
"  -hasbound  Indicates existence of boundaries in partial annotation (\"|\")" << endl << endl;
    }
    exit(1);
}

void KyteaConfig::printVersion() {
    cerr << "KyTea version "<<VERSION<<endl;
    exit(0);
}

void KyteaConfig::ch(const char * n, const char* v) {
    if(!v) {
        cerr << "Stray '" << n << "' argument" << endl << endl;
        printUsage();
    }
}       
// parse a single argument
//  the value argument can be null
//  return 1 if the value was used 0 if not
unsigned KyteaConfig::parseTrainArg(const char * n, const char * v) {
    unsigned r=1;

    if(!strcmp(n, "--help") || !strcmp(n,"-help")) { printUsage(); }
    else if(!strcmp(n, "--version") || !strcmp(n,"-version")) { printVersion(); }

    // general input/output option
    else if(!strcmp(n, "-encode"))        { ch(n,v); setEncoding(v); }
    else if(!strcmp(n, "-debug"))    { ch(n,v); setDebug(util_->parseInt(v)); }

    // input options for training
    else if(!strcmp(n, "-full"))     { ch(n,v); addCorpus(v, CORP_FORMAT_FULL); }
    else if(!strcmp(n, "-part"))     { ch(n,v); addCorpus(v, CORP_FORMAT_PART); }
    else if(!strcmp(n, "-conf"))     { ch(n,v); addCorpus(v, CORP_FORMAT_PROB); }
    else if(!strcmp(n, "-dict"))     { ch(n,v); addDictionary(v); }
    else if(!strcmp(n, "-subword"))  { ch(n,v); addSubwordDict(v); }
    else if(!strcmp(n, "-global"))    { ch(n,v); setGlobal(util_->parseInt(v)-1); }

    // output option for training
    else if(!strcmp(n, "-model"))    { ch(n,v); setModelFile(v); }
    else if(!strcmp(n, "-modtext"))  { setModelFormat('T'); r=0; }
    else if(!strcmp(n, "-numtags"))  { ch(n,v); setNumTags(util_->parseInt(v)); }

    // liblinear options
    else if(!strcmp(n, "-eps"))      { ch(n,v); setEpsilon(util_->parseFloat(v)); }
    else if(!strcmp(n, "-cost"))      { ch(n,v); setCost(util_->parseFloat(v)); }
    else if(!strcmp(n, "-solver"))   { ch(n,v); setSolverType(util_->parseInt(v)); }

    // feature options
    else if(!strcmp(n, "-charw"))    { ch(n,v); setCharWindow(util_->parseInt(v)); }
    else if(!strcmp(n, "-charn"))    { ch(n,v); setCharN(util_->parseInt(v)); }
    else if(!strcmp(n, "-typew"))    { ch(n,v); setTypeWindow(util_->parseInt(v)); }
    else if(!strcmp(n, "-typen"))    { ch(n,v); setTypeN(util_->parseInt(v)); }
    else if(!strcmp(n, "-dictn"))    { ch(n,v); setDictionaryN(util_->parseInt(v)); }
    else if(!strcmp(n, "-unkn"))     { ch(n,v); setUnkN(util_->parseInt(v)); }

    // formatting options
    else if(!strcmp(n, "-wordbound"))     { ch(n,v); setWordBound(v); }
    else if(!strcmp(n, "-tagbound"))      { ch(n,v); setTagBound(v); }
    else if(!strcmp(n, "-elembound"))     { ch(n,v); setElemBound(v); }
    else if(!strcmp(n, "-unkbound"))      { ch(n,v); setUnkBound(v); }
    else if(!strcmp(n, "-nobound"))       { ch(n,v); setNoBound(v); }
    else if(!strcmp(n, "-hasbound"))      { ch(n,v); setHasBound(v); }
    else if(!strcmp(n, "-skipbound"))     { ch(n,v); setSkipBound(v); }

    // whether or not to perform word segmentation, pronunciation estimation
    else if(!strcmp(n, "-nows"))     { setDoWS(false); r=0; }
    else if(!strcmp(n, "-notags"))   { setDoTags(false); r=0; }
    else if(!strcmp(n, "-nobias"))   { setBias(false); r=0; }

    // --- DEPRECATED ---
    // do not use these undocumented options, as they may disappear in the future
    else if(!strcmp(n, "-prob"))     { ch(n,v); addCorpus(v, CORP_FORMAT_PROB); }
    else if(!strcmp(n, "-dicn"))    { ch(n,v); setDictionaryN(util_->parseInt(v)); }
    
    else if(n[0] == '-') {
        cerr << "Invalid argument '" << n << "'" << endl << endl;
        printUsage();
    }

    else { r=0; args_.push_back(n); }
    return r;
}

unsigned KyteaConfig::parseRunArg(const char * n, const char * v) {
    unsigned r=1;

    if(!strcmp(n, "--help") || !strcmp(n,"-help")) { printUsage(); }
    else if(!strcmp(n, "--version") || !strcmp(n,"-version")) { printVersion(); }

    // general input/output option
    else if(!strcmp(n, "-in"))       { ch(n,v); setIOFormat(v, inputForm_);  }
    else if(!strcmp(n, "-out"))      { ch(n,v); setIOFormat(v, outputForm_); }

    // output option for training
    else if(!strcmp(n, "-model"))    { ch(n,v); setModelFile(v); }

    // whether or not to perform word segmentation, pronunciation estimation
    else if(!strcmp(n, "-nows"))     { setDoWS(false); r=0; }
    else if(!strcmp(n, "-notags"))   { setDoTags(false); r=0; }
    else if(!strcmp(n, "-notag"))    { 
        ch(n,v); 
        if(util_->parseInt(v) < 1) THROW_ERROR("Illegal setting "<<v<<" for -notag (must be 1 or greater)");
        setDoTag(util_->parseInt(v)-1,false);
    }
    else if(!strcmp(n, "-numtags"))  { ch(n,v); setNumTags(util_->parseInt(v)); }
    else if(!strcmp(n, "-tagmax"))   { ch(n,v); setTagMax(util_->parseInt(v)); }

    // the limit on the number of unknown words to output
    else if(!strcmp(n, "-unktag"))   { ch(n,v); setUnkTag(v); }
    else if(!strcmp(n, "-deftag"))   { ch(n,v); setDefaultTag(v); }
    else if(!strcmp(n, "-unkbeam"))  { ch(n,v); setUnkBeam(util_->parseInt(v)); }
    else if(!strcmp(n, "-debug"))    { ch(n,v); setDebug(util_->parseInt(v)); }

    // formatting options
    else if(!strcmp(n, "-wordbound"))     { ch(n,v); setWordBound(v); }
    else if(!strcmp(n, "-tagbound"))      { ch(n,v); setTagBound(v); }
    else if(!strcmp(n, "-elembound"))     { ch(n,v); setElemBound(v); }
    else if(!strcmp(n, "-unkbound"))      { ch(n,v); setUnkBound(v); }
    else if(!strcmp(n, "-nobound"))       { ch(n,v); setNoBound(v); }
    else if(!strcmp(n, "-hasbound"))      { ch(n,v); setHasBound(v); }
    else if(!strcmp(n, "-skipbound"))     { ch(n,v); setSkipBound(v); }

    else if(n[0] == '-') {
        cerr << "Invalid argument '" << n << "'" << endl << endl;
        printUsage();
    }

    else { r=0; args_.push_back(n); }
    return r;
}
