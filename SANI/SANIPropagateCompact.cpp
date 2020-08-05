/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: SANIPropagateCompact.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1m3a 01-August-2020
 * Requirements: 
 * Description: SANI (Sequentially Activated Neuronal Input neural network) Propagate Compact - ~O(n)
 * /
 *******************************************************************************/


#include "SANIPropagateCompact.hpp"
#include "SHAREDvars.hpp"
#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
#include "GIAneuralNetworkOperations.hpp"
#include "ANNdisplay.hpp"
#endif

#ifdef SANI_COMPACT

//static int newNeuronIndex;

#ifdef SANI_PARSE_SAVE_PARSE_TREE
SANIGroupParseTree* topLevelParseTreeGroupLocalCompact;
#endif

#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
static int parseTreeMaxWeight;
#endif

#ifdef SANI_FREE_MEMORY
static vector<SANIGroupParseTree*> parseTreeGroupListCompact;
#endif

#ifdef SANI_FREE_MEMORY
vector<SANIGroupParseTree*>* SANIPropagateCompactClass::getParseTreeGroupList()
{
	return &parseTreeGroupListCompact;
}
#endif


#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
static int incrementalGenerationIndex;
void SANIPropagateCompactClass::initialiseIncrementalGeneration()
{
	incrementalGenerationIndex = 0;
}
#endif

//if SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY, first layer contains firstPOS neuron for each wordIndex only
bool SANIPropagateCompactClass::defineFirstLayer(GIAtranslatorVariablesClass* translatorVariables, SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool result = true;
		
	for(int i=0; i<forwardPropogationSentenceData->sentenceContents->size(); i++)
	{
		bool getFirstLayer = true;
		
		int w = i;

		SANIForwardPropogationSignalData forwardPropogationSignalDataNOTUSED;
	
		if(!propagateWordThroughNetworkIntro(translatorVariables, w, &forwardPropogationSignalDataNOTUSED, forwardPropogationSentenceData, getFirstLayer))
		{
			result = false;
		}
	}
	
	return result;
}	
	
bool SANIPropagateCompactClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents)
{
	SANIGroupParseTree* topLevelParseTreeGroup = NULL; 
	bool parseIsolatedSubreferenceSets = false;
	bool parserEnabled = false;
	int performance = 0;
	return executeTxtRelTranslatorNeuralNetwork(translatorVariables, SANIGroupTypes, sentenceContents, &topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, &performance);
}

bool SANIPropagateCompactClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
{
	bool result = false;

	SANIForwardPropogationSentenceData forwardPropogationSentenceData;
	forwardPropogationSentenceData.sentenceContents = sentenceContents;

	#ifndef SANI_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
	executeTxtRelTranslatorNeuralNetworkPart1(translatorVariables, SANIGroupTypes, &forwardPropogationSentenceData, topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, performance);
	#endif
	
	#ifndef SANI_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
	executeTxtRelTranslatorNeuralNetworkPart2(translatorVariables, SANIGroupTypes, &forwardPropogationSentenceData, topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, performance);
	#endif
	
	return result;
}


bool SANIPropagateCompactClass::executeTxtRelTranslatorNeuralNetworkPart1(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
{
	bool result = false;

	*performance = 0;
	
	bool sentenceValidActivationPath = false;

	#ifdef SANI_PARSE_SAVE_PARSE_TREE
	//cout << "SANI_PARSE_SAVE_PARSE_TREE: topLevelParseTreeGroupLocalCompact = NULL" << endl;
	topLevelParseTreeGroupLocalCompact = NULL;
	#endif
	
	//sentence specific variables:
	SANIPropagateOperations.setParseSentenceReverse(true, forwardPropogationSentenceData);
	//forwardPropogationSentenceData->toplevelGroupActivationFound = false;	//now done by performPropagation
	//forwardPropogationSentenceData->performance = performance;
	#ifdef SANI_PARSE_SIMULTANEOUS
	forwardPropogationSentenceData->parserEnabled = parserEnabled;
	#endif
	/*
	//not used by SANI_SEQUENCE_GRAMMAR
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_QUERIES
	forwardPropogationSentenceData->isQuery = SANIGroupClassObject.determineIsQuery(sentenceContents);
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
	forwardPropogationSentenceData->parseIsolatedSubreferenceSets = parseIsolatedSubreferenceSets;
	#endif
	forwardPropogationSentenceData->SANIGroupTypes = SANIGroupTypes;
	*/
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK_PROPAGATION
	forwardPropogationSentenceData->SANIGroupTypes = SANIGroupTypes;
	#endif
	
	for(int w=0; w<forwardPropogationSentenceData->sentenceContents->size(); w++)
	{	
		SANIForwardPropogationWordData* forwardPropogationWordData = new SANIForwardPropogationWordData();
		forwardPropogationSentenceData->forwardPropogationWordDataArray.push_back(forwardPropogationWordData);
	}
	
	//OLD: defineFirstLayer(translatorVariables, forwardPropogationSentenceData, &firstLayer)
	
	#ifndef SANI_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
	if(performPropagationTest(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData))
	{
		//cout << "SANIPropagateCompactClass::executeTxtRelTranslatorNeuralNetworkPart1: performPropagationTest pass" << endl;

		result = true;
		//printBackpropParseTree(topLevelParseTreeGroupLocalCompact, 3);
		
		#ifdef SANI_PARSE_SAVE_PARSE_TREE
		//cout << "SANI_PARSE_SAVE_PARSE_TREE: *topLevelParseTreeGroup = topLevelParseTreeGroupLocalCompact;" << endl;
		*topLevelParseTreeGroup = topLevelParseTreeGroupLocalCompact;
		#endif
	}
	else
	{
		//cout << "SANIPropagateCompactClass::executeTxtRelTranslatorNeuralNetworkPart1: performPropagationTest fail" << endl;	
	}
	#endif
	
	return result;
}

bool SANIPropagateCompactClass::executeTxtRelTranslatorNeuralNetworkPart2(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
{
	bool result = true;
	
	#ifdef SANI_FREE_MEMORY
	for(int w=0; w<forwardPropogationSentenceData->sentenceContents->size(); w++)
	{
		SANIForwardPropogationWordData* forwardPropogationWordData = forwardPropogationSentenceData->forwardPropogationWordDataArray[w];
		delete forwardPropogationWordData;
	}
	#endif
	
	if(result)
	{
		#ifdef SANI_PARSE_PERFORMANCE
		*performance = forwardPropogationSentenceData->sentenceContents->size();
		#else
		*performance = 1;
		#endif
	}
	
	return result;
}
	





bool SANIPropagateCompactClass::performPropagationTest(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool result = false;
	
	bool resetAllNeuronComponents = true;
	bool deinitialiseParseTreeGroupAfterFinish = true;
	int firstIndexInSequence = 0;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_TEST_VERIFICATION_PROPAGATION_IN_OPPOSITE_DIRECTION
	SANIPropagateOperations.setParseSentenceReverse(false, forwardPropogationSentenceData);
	#endif
	
	if(performPropagation(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, resetAllNeuronComponents, deinitialiseParseTreeGroupAfterFinish, firstIndexInSequence))
	{
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
		cout << "toplevelGroupActivationFound: groupIndexes = " << SANIPropagateOperations.printParseTreeGroupIndices(topLevelParseTreeGroupLocalCompact, 0) << endl;
		#endif
		result = true;
	}
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_TEST_VERIFICATION_PROPAGATION_IN_OPPOSITE_DIRECTION
	SANIPropagateOperations.setParseSentenceReverse(true, forwardPropogationSentenceData);
	#endif
	
	return result;
}


bool SANIPropagateCompactClass::performPropagation(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool resetAllNeuronComponents, const bool deinitialiseParseTreeGroupAfterFinish, const int firstIndexInSequence)
{	
	int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed;
	int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = forwardPropogationSentenceData->sentenceContents->size()-1 - firstIndexInSequence;
		activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = 0;
	}
	else
	{
		activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = firstIndexInSequence;
		activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = forwardPropogationSentenceData->sentenceContents->size()-1;
	}
	return performPropagation(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, resetAllNeuronComponents, deinitialiseParseTreeGroupAfterFinish, firstIndexInSequence, activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed);
}


bool SANIPropagateCompactClass::performPropagation(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool resetAllNeuronComponents, const bool deinitialiseParseTreeGroupAfterFinish, const int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, const int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed)
{
	int firstIndexInSequence;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		firstIndexInSequence = forwardPropogationSentenceData->sentenceContents->size()-1;
	}
	else
	{
		firstIndexInSequence = 0;
	}
	return performPropagation(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, resetAllNeuronComponents, deinitialiseParseTreeGroupAfterFinish, firstIndexInSequence, activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed);

}

bool SANIPropagateCompactClass::performPropagation(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool resetAllNeuronComponents, const bool deinitialiseParseTreeGroupAfterFinish, const int firstIndexInSequence, const int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, const int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed)
{
	bool result = false;
	
	forwardPropogationSentenceData->toplevelGroupActivationFound = false;
	
	if(resetAllNeuronComponents)
	{
		SANIPropagateOperations.resetAllNeuronComponents(SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_COMPONENT_CONNECTION_ACTIVE); //this is required for SANI_LIGHT_OPTIMISED
		SANIPropagateOperations.resetAllNeuronComponents(SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);	//this is required to initialise currentParseTreeGroup for every group (only required for first execution of SANIPropagateCompactClass::executeTxtRelTranslatorNeuralNetwork)
	}
	#ifdef SANI_FREE_MEMORY
	SANIPropagateOperations.initialiseParseTreeGroupList(SANIGroupTypes, &parseTreeGroupListCompact);
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
	int iStart = firstIndexInSequence;
	#else
	int iStart = 0;
	#endif
	for(int i=iStart; i<forwardPropogationSentenceData->sentenceContents->size(); i++)	//OLD: for(int i=currentFirstInputNeuronIndexInSequence; ...
	{
		int firstLayerNeuronIndex;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			firstLayerNeuronIndex = forwardPropogationSentenceData->sentenceContents->size()-1-i;
		}
		else
		{
			firstLayerNeuronIndex = i;
		}	
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR	
		cout << "\n\n \e[33m SANIPropagateCompactClass::performPropagation: i = " << i << ", firstLayerNeuronIndex = " << firstLayerNeuronIndex << " - " << (*(forwardPropogationSentenceData->sentenceContents))[firstLayerNeuronIndex]->tagName << " \e[0m" << endl;
		#endif
	
		//group specific variables:
		SANIForwardPropogationSignalData forwardPropogationSignalData;
		forwardPropogationSignalData.activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed;
		forwardPropogationSignalData.activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed;
		#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
		forwardPropogationSignalData.firstIndexInSequence = firstIndexInSequence;
		#endif
		
		const bool getFirstLayer = false;
		if(propagateWordThroughNetworkIntro(translatorVariables, firstLayerNeuronIndex, &forwardPropogationSignalData, forwardPropogationSentenceData, getFirstLayer))
		{

		}
	}

	if(deinitialiseParseTreeGroupAfterFinish)
	{
		#ifdef SANI_FREE_MEMORY
		SANIPropagateOperations.deinitialiseParseTreeGroupList(SANIGroupTypes, &parseTreeGroupListCompact);
		#endif
	}
	
	/*
	#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_ACTIVATION_INPUT_NEURONS
	//deactivate all input neurons
	SANIGroupNeuralNetwork* firstGroupInInputLayerSectionWordPOStype = SANIFormation.getFirstGroupInInputLayerSectionWordPOStype();
	SANIGroupNeuralNetwork* currentGroupInInputLayerSection = firstGroupInInputLayerSection;
	for(int i=0; i<groupIndexInSection; i++)
	{
		currentGroupInInputLayerSection->neuronActive = false;
		currentGroupInInputLayerSection = currentGroupInInputLayerSection->next;
	}
	#endif
	*/
	
	if(forwardPropogationSentenceData->toplevelGroupActivationFound)
	{
		result = true;
	}
	
	return result;
}

bool SANIPropagateCompactClass::propagateWordThroughNetworkIntro(GIAtranslatorVariablesClass* translatorVariables, const int w, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool getFirstLayer)
{
	bool result = true;
	
	GIApreprocessorPlainTextWord* currentWord = (*(forwardPropogationSentenceData->sentenceContents))[w];

	SANIForwardPropogationWordData* forwardPropogationWordData = (forwardPropogationSentenceData->forwardPropogationWordDataArray[w]);
	forwardPropogationWordData->wordReference = currentWord;
	#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	forwardPropogationWordData->wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	//set by propagateWordThroughNetworkIntro
	#endif
	forwardPropogationWordData->w = w;

	#ifdef SANI_DEBUG_PROPAGATE
	cout << "currentWord = " << currentWord->tagName << endl;
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
	int wordPOStypeNOTUSED = INT_DEFAULT_VALUE;
	if(!propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStypeNOTUSED, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, getFirstLayer))
	{
		result = false;
	}	
	#else
	if(!SANIPropagateOperations.currentWordAmbiguous(currentWord))
	{
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		int wordPOStype = currentWord->unambiguousPOSindex;
		#else
		#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		forwardPropogationSignalData->firstPOSval = false;
		#endif
		for(int wordPOStype=0; wordPOStype<GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; wordPOStype++)
		{
			if(SHAREDvars.getBitValue(currentWord->POSambiguityInfo, wordPOStype))
			{
		#endif	
				if(!propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, getFirstLayer))
				{
					result = false;
				}
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		#else
				#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
				forwardPropogationSignalData->firstPOSval = false;
				#endif
			}
		}
		#endif
	}
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES
	else
	{
		int wordPOStype = INT_DEFAULT_VALUE;
		bool pass = SANIPropagateOperations.getWordPOStypeFromAmbiguousWord(currentWord, &wordPOStype);
		if(pass)
		{
			if(!propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, getFirstLayer))
			{
				result = false;
			}		
		}
	}
	#endif
	#endif
	
	return result;
}

bool SANIPropagateCompactClass::propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool getFirstLayer)
{
	bool result = true;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
	GIApreprocessorPlainTextWord* currentWord = forwardPropogationWordData->wordReference;
	SANIGroupNeuralNetwork* inputLayerGroup;
	if(!SANIFormation.findInputNeuronLayerSectionWordOrig(currentWord, &inputLayerGroup))
	{
		SANIFormation.addInputNeuronLayerSectionWordOrig(currentWord, &inputLayerGroup);
	}
	#else
	SANIGroupNeuralNetwork* inputLayerGroup = SANIFormation.getInputGroupLayerSection(SANIFormation.getFirstGroupInInputLayerSectionWordPOStype(), wordPOStype);	
	#endif
	
	forwardPropogationWordData->wordPOStype = wordPOStype;
	inputLayerGroup->wordPOStype = wordPOStype;

	if(getFirstLayer)
	{
		#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		(forwardPropogationSentenceData->firstLayer->at(w))->push_back(inputLayerGroup);
		#else
		forwardPropogationSentenceData->firstLayer->push_back(inputLayerGroup);
		#endif
	}
	else
	{
		#ifdef SANI_DEBUG_PROPAGATE
		cout << "SANIPropagateCompactClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS, wordPOStype = " << wordPOStype << endl;
		#endif
		
		#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_ACTIVATION_INPUT_NEURONS
		inputLayerGroup->neuronActive = true;
		inputLayerGroup->neuronReference->activationLevel = ANN_ALGORITHM_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_ACTIVE_LEVEL_FULL;
		#endif
	
		SANIGroupParseTree* activationPathWordFirstParseTreeGroup = NULL;	//not properly used by SANI_SEQUENCE_GRAMMAR
		int layer = 0;
		propagateWordThroughNetworkGroup(translatorVariables, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup);
		
		#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_ACTIVATION_INPUT_NEURONS
		inputLayerGroup->neuronActive = false;
		inputLayerGroup->neuronReference->activationLevel = ANN_ALGORITHM_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_ACTIVE_LEVEL_INACTIVE;
		#endif
	}
			
	return result;
}	

		

bool SANIPropagateCompactClass::propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup)
{
	bool result = false;
	
	#ifdef SANI_DEBUG_PROPAGATE
	SANIPropagateOperations.printParseTreeDebugIndentation(layer);
	cout << "\e[32m 1: propagateWordThroughNetworkGroup: " << group->groupTypeName << ":" << "group->groupName << group->groupIndex = " << group->groupIndex << " \e[0m" << endl;
	#endif
			
	#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	if(!group->neuronPropagated)	//prevent circular loops	//only required for recursion (not yet supported by SANI_SEQUENCE_GRAMMAR)
	{
		group->neuronPropagated = true;
	#endif
		
		if(propagateWordThroughNetworkGroupSelect(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup))
		{
			result = true;
		}
		
	#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
		group->neuronPropagated = false;
	}
	#endif
	
	return result;
}
		
bool SANIPropagateCompactClass::propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup)
{
	bool result = false;

	//now search for existing sequence in network

	for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
	{
		#ifndef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
		if(!(forwardPropogationSentenceData->finishedPassingSentenceWords))
		{
		#endif
			SANIComponentNeuralNetwork* currentComponent = (group->ANNfrontComponentConnectionList)[i];
			SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;
	
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
			if((forwardPropogationSentenceData->findingCandidateComponent2))
			{
				if(!(forwardPropogationSentenceData->foundCandidateComponent2))
				{
					if(currentComponent == forwardPropogationSentenceData->candidateComponent2ToFind)
					{
						forwardPropogationSentenceData->foundCandidateComponent2 = true;
						forwardPropogationSentenceData->candidateComponent2sourceParseTreeGroup = activationPathWordCurrentParseTreeGroup;
					}
				}
			}
			#endif
		
			int componentIndex = currentComponent->componentIndex;	
			
			#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
			if(!ownerGroup->neuronPropagated)	//prevent circular loops
			{
			#endif	
				#ifdef SANI_DEBUG_PROPAGATE_EXTRA2
				SANIPropagateOperations.printParseTreeDebugIndentation(layer+1);	
				cout << "2a: propagateWordThroughNetworkGroup: ownerGroup->groupIndex = " <<  ownerGroup->groupIndex << ", currentComponent->componentIndex = " << currentComponent->componentIndex << endl;
				#endif
				
				if(currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
				{
					currentComponent->candidateStringMatch = forwardPropogationWordData->wordReference;
				}
				/*
				if(currentComponent->semanticRelationReturnEntity)
				{
					ownerGroup->semanticRelationReturnEntityForwardPropogationSignalDataProspective = *forwardPropogationSignalData;
				}
				*/
	
				bool sequentialActivationFound = false;
				bool existingActivationFoundStartComponent = false;
				//TODO: determine if new/SANI_SEQUENCE_GRAMMAR group reset method sufficient; see SANIPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady
				if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, i, &sequentialActivationFound, &existingActivationFoundStartComponent))
				{
					result = true;
				}

			#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
			}
			#endif
		#ifndef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
		}
		#endif
	}
	
	return result;
}	
	


bool SANIPropagateCompactClass::propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, bool* sequentialActivationFound, bool* existingActivationFoundStartComponent)
{
	bool result = false;
	
	bool activationSequenceCompleted = false;
	bool sequentialActivationFoundAndExistingActivationFound = false;
	
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA3
	SANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
	cout << "3_: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << ", ownerGroup->groupIndex: " << ownerGroup->groupIndex << endl;	
	#endif
	
	
	SANIComponentNeuralNetwork* previousActiveComponent = NULL;	
	SANIComponentNeuralNetwork* finalActiveComponent = NULL;	//NOTUSED
	bool firstActiveComponentInGroup = false;	//NOTUSED
	bool missingStartComponentFound = false;
	bool missingOrVariableStartComponentFound = false;
	//#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENTS
	bool missingOrVariableEndComponentFound = false;
        //#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENTS_REMEMBER_FIRST_COMPONENT_WORD_INDEX
	bool existingActivationFoundEndComponent = false;
	//#endif
	//#endif
	bool variableStartComponentFound = false;	//temp debug only
	int numberOfInactiveComponentsRemaining = 0;
        
	//cout << "\townerGroup->groupIndex = " << ownerGroup->groupIndex << endl;
	
	bool componentWordConnectivityTestsPrelimResult = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_FORCE_RESET_IF_NO_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
	if(SANIPropagateOperations.componentWordConnectivityTestsWrapper(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData, false))
	{
		componentWordConnectivityTestsPrelimResult = true;
	}
	#endif
	
	if(SANIPropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(currentComponent, &(ownerGroup->components), forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationPathWordCurrentParseTreeGroup, &activationSequenceCompleted, &firstActiveComponentInGroup, &previousActiveComponent, &finalActiveComponent, existingActivationFoundStartComponent, &existingActivationFoundEndComponent, &missingStartComponentFound, &missingOrVariableStartComponentFound, &missingOrVariableEndComponentFound, &numberOfInactiveComponentsRemaining, ownerGroup, componentWordConnectivityTestsPrelimResult))
	{	
		/*
		if(missingOrVariableEndComponentFound)
		{
			cout << "propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady" << endl;
		}
		*/
		
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA3_PRIMARY
		SANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
		cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << " - missingStartComponentFound = " << missingStartComponentFound << ", missingOrVariableStartComponentFound = " << missingOrVariableStartComponentFound << endl;
		SANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
		cout << "ownerGroup->currentParseTreeGroupTemp->components.size() = " << ownerGroup->currentParseTreeGroupTemp->components.size() << endl;
		#endif
		
		#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
		SANIGroupParseTree* currentParseTreeGroupBackup = ownerGroup->currentParseTreeGroupTemp;
		GIApreprocessorPlainTextWord* previousActiveComponentActiveWordRecordBackup = NULL;
		if(previousActiveComponent != NULL)
		{
			previousActiveComponentActiveWordRecordBackup = previousActiveComponent->neuronComponentConnectionActiveWordRecord;
		}
		#endif
		
		//execute sequentialActivationConnectivityTests at start to save multiple executions [OPTIONAL];
		bool componentWordConnectivityTests = false;
		#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_PRECHECKS
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_SIMPLIFIED	//or SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS
		bool skipWordConnectivityTests = missingOrVariableStartComponentFound;
		#else
		bool skipWordConnectivityTests = missingStartComponentFound;
		#endif
		#else
		bool skipWordConnectivityTests = false;
		#endif
		if(sequentialActivationConnectivityTests(translatorVariables, currentComponent, ownerGroup, ownerGroup->currentParseTreeGroupTemp, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, activationPathWordCurrentParseTreeGroup, *existingActivationFoundStartComponent, existingActivationFoundEndComponent, skipWordConnectivityTests))
		{
			componentWordConnectivityTests = true;
			*sequentialActivationFound = true;
			
			/*
			if(missingOrVariableEndComponentFound)
			{
				cout << "sequentialActivationConnectivityTests" << endl;
			}
			*/
		}
		
	
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_LAST_COMPONENTS
		propagateVariableEndComponentRemoveLastParseTreeComponent(translatorVariables, layer, ownerGroup, currentComponent, &(ownerGroup->components), forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, &activationSequenceCompleted, previousActiveComponent, finalActiveComponent, &existingActivationFoundEndComponent, componentWordConnectivityTests, &missingOrVariableEndComponentFound, sequentialActivationFound, activationPathWordCurrentParseTreeGroup);
		#endif		
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
		verifyMissingOrVariableStartComponentFound(translatorVariables, layer, ownerGroup, currentComponent, &(ownerGroup->components), forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, &activationSequenceCompleted, firstActiveComponentInGroup, previousActiveComponent, finalActiveComponent, existingActivationFoundStartComponent, &missingStartComponentFound, &variableStartComponentFound, componentWordConnectivityTests, &missingOrVariableStartComponentFound, numberOfInactiveComponentsRemaining, sequentialActivationFound, activationPathWordCurrentParseTreeGroup);
		verifyMissingOrVariableStartComponentFoundAllowedWordIndices(translatorVariables, layer, ownerGroup, currentComponent, &(ownerGroup->components), forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, &activationSequenceCompleted, firstActiveComponentInGroup, previousActiveComponent, finalActiveComponent, existingActivationFoundStartComponent, &missingStartComponentFound, &variableStartComponentFound, componentWordConnectivityTests, &missingOrVariableStartComponentFound, numberOfInactiveComponentsRemaining, sequentialActivationFound, activationPathWordCurrentParseTreeGroup);
		#endif
		
		if(*sequentialActivationFound)
		{	
			//cout << "sequentialActivationFound: missingOrVariableStartComponentFound = " << missingOrVariableStartComponentFound << endl;
					
			#ifdef SANI_DEBUG_PROPAGATE_EXTRA3
			SANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
			cout << "3c2: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << ", missingStartComponentFound = " << missingStartComponentFound << ", (ownerGroup->currentParseTreeGroupTemp->missingOrVariableStartComponentFound) = " << (ownerGroup->currentParseTreeGroupTemp->missingOrVariableStartComponentFound) << ", variableStartComponentFound = " << variableStartComponentFound << endl;	
			#else
			cout << "3c2: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << ", missingStartComponentFound = " << missingStartComponentFound << endl;				
			#endif
			#endif
		
			bool missingOrVariableStartComponentFoundStored = false;
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
			if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
			{
				missingOrVariableStartComponentFoundStored = ownerGroup->currentParseTreeGroupTemp->missingOrVariableStartComponentFound;
			}
			#endif
			bool variableStartComponentFoundStoredNOTUSED = false;	//= variableStartComponentFound //variableStartComponentFound = ownerGroup->currentParseTreeGroupTemp->variableStartComponentFound;
			if(propagateWordThroughNetworkGroupComponent(translatorVariables, group, currentComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, activationPathWordCurrentParseTreeGroup, groupFrontComponentConnectionListIndex, *existingActivationFoundStartComponent, missingStartComponentFound, missingOrVariableStartComponentFoundStored, variableStartComponentFoundStoredNOTUSED))
			{
				result = true;
			}
		}
		else
		{
			#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
			//cout << "restoring currentParseTreeGroupBackup" << endl;
			ownerGroup->currentParseTreeGroupTemp = currentParseTreeGroupBackup;
			if(previousActiveComponent != NULL)
			{
				previousActiveComponent->neuronComponentConnectionActiveWordRecord = previousActiveComponentActiveWordRecordBackup;
			}
			#endif
		}
	}

	/*
	if(missingOrVariableEndComponentFound)
	{
		if(activationSequenceCompleted)
		{
			cerr << "SANIPropagateCompactClass::propagateWordThroughNetworkGroupComponentWrapper error: missingOrVariableEndComponentFound & activationSequenceCompleted" << endl;
			exit(EXIT_ERROR);
		}
	}
	*/
	
	

	#ifndef SANI_SEQUENCE_GRAMMAR_IGNORE_PARTIAL_DETECTIONS
	if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
	{
		if(!activationSequenceCompleted)
		{
			if(*sequentialActivationFound)
			{
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENTS
				//rule #2;
				if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableEndComponent)
				{					
					if(missingOrVariableEndComponentFound)
					{						
						if(calculateNextIndexInSequenceProspective(forwardPropogationSentenceData, ownerGroup->currentParseTreeGroupTemp) < forwardPropogationSentenceData->sentenceContents->size()-1)
						{		
								//cout << "SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENTS: updateActivatedNeuronWithMaxWordIndexCoverage" << endl;
				#endif
							bool candidateCoveragePartial = true;
							updateActivatedNeuronWithMaxWordIndexCoverage(ownerGroup, ownerGroup->currentParseTreeGroupTemp, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, missingStartComponentFound, missingOrVariableStartComponentFound, variableStartComponentFound, candidateCoveragePartial);
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENTS
						}
					}
				}
				#endif	
			}
		}
	}
	#endif
		
	return result;
}




	
	
bool SANIPropagateCompactClass::propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, const bool existingActivationFoundStartComponent, const bool missingStartComponentFound, const bool missingOrVariableStartComponentFound, const bool variableStartComponentFound)
{
	bool result = true;
		
	if(existingActivationFoundStartComponent)
	{		
		#ifdef SANI_DEBUG_PROPAGATE	//SANI_DEBUG_SEQUENCE_GRAMMAR
		cout << "******** resetGroupParseTreeGroupRef, groupIndex = " << ownerGroup->groupIndex << endl;
		#endif
		ownerGroup->neuronActive = false;	//required for SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_ACTIVATION
		SANIPropagateOperations.resetGroupActivation(ownerGroup);	//OLD: component activations have already been reset by SANIPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady
		SANIPropagateOperations.resetGroupParseTreeGroupRef(ownerGroup, false);
		#ifdef SANI_FREE_MEMORY
		parseTreeGroupListCompact.push_back(ownerGroup->currentParseTreeGroupTemp);
		#endif	
		#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
		ownerGroup->parseTreeGroupMemory.push_back(ownerGroup->currentParseTreeGroupTemp);
		#endif
	}
	
	#ifdef SANI_SEQUENCE_GRAMMAR_WEIGHTS
	//if(!(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)) {
	currentComponent->componentStrength = currentComponent->componentStrength + 1;
	//}
	#endif
	currentComponent->neuronComponentConnectionActive = true;
	currentComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
	
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
	currentComponent->neuronComponentConnectionActiveWordRecordMemory.push_back(currentComponent->neuronComponentConnectionActiveWordRecord);	//note neuronComponentConnectionActiveWordRecordMemory is stored in currentComponent not newParseComponent such that it can be retained across executions of resetGroupParseTreeGroupRef(ownerGroup, false)
	#endif
	
	if(forwardPropogationWordData->wordReference->translatorSentenceWordIndex < 0)
	{
		cout << "SANIPropagateCompactClass::propagateWordThroughNetworkGroupComponent error: forwardPropogationWordData->wordReference->translatorSentenceWordIndex = " << forwardPropogationWordData->wordReference->translatorSentenceWordIndex << endl;
		exit(EXIT_ERROR);
	}
	
	#ifdef SANI_PARSE
	//create new parseTreeGroup
	//copied from SANIPropagateInverseClass::generateRulesGroupTreeComponents;	
	SANIComponentParseTree* newParseComponent = SANIPropagateOperations.convertNeuralNetworkComponentToParseTreeComponentNew(currentComponent);	//new SANIComponentParseTree(*SANIPropagateOperations.convertNeuralNetworkComponentToParseTreeComponent(currentComponent));	//copy rules component

	newParseComponent->componentRef = currentComponent;
	newParseComponent->neuronComponentConnectionActive = true;	//implied
	newParseComponent->neuronComponentConnectionActiveWordRecord = currentComponent->neuronComponentConnectionActiveWordRecord;
	#ifdef SANI_PARSE_GENERATE_PARSE_TREE
	newParseComponent->parseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
	#endif
	#ifdef SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
	newParseComponent->wordIndex = forwardPropogationWordData->w;
	#endif
	newParseComponent->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)
						
	SANIGroupParseTree* currentParseTreeGroupTemp = ownerGroup->currentParseTreeGroupTemp;

	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		//currentParseTreeGroupTemp->components.push_front(newParseComponent);
		currentParseTreeGroupTemp->components.insert(currentParseTreeGroupTemp->components.begin(), newParseComponent);
	}
	else
	{
		currentParseTreeGroupTemp->components.push_back(newParseComponent);
	}
	#endif

	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK_PROPAGATION
	if(!(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage))
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK_PROPAGATION_RESTRICT_TO_POST_GENERATION_PHASE
		if(translatorVariables->currentPreprocessorSentenceInList->sentenceIndexOriginal > translatorVariables->maxNumberSentences/2)
		{
		#endif
			printSANInetworkSVG(translatorVariables, forwardPropogationSentenceData->SANIGroupTypes);
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK_PROPAGATION_RESTRICT_TO_POST_GENERATION_PHASE
		}
		#endif
	}
	#endif
	
	#ifdef SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
	if(currentParseTreeGroupTemp->parseTreeMaxWordIndex == INT_DEFAULT_VALUE)
	{
		currentParseTreeGroupTemp->parseTreeMaxWordIndex = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
		currentParseTreeGroupTemp->parseTreeMinWordIndex = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
		#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
		currentParseTreeGroupTemp->parseTreeMaxWeight = ownerGroup->groupWeight;
		currentParseTreeGroupTemp->parseTreeMinWeight = ownerGroup->groupWeight;
		currentParseTreeGroupTemp->parseTreeTotalWeight = ownerGroup->groupWeight;
		#endif
	}
	updateParseTreeMaxMinWordIndexOfParent(currentParseTreeGroupTemp, newParseComponent, activationPathWordCurrentParseTreeGroup);
	#endif
	
	
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA4
	SANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroup: missingOrVariableStartComponentFound = " << missingOrVariableStartComponentFound << ", newParseComponent->componentIndex = " << newParseComponent->componentIndex << ", ownerGroup->components.size() = " << ownerGroup->components.size() << ", groupIndex = " << ownerGroup->groupIndex << ", existingActivationFoundStartComponent = " << existingActivationFoundStartComponent << ", activationSequenceCompleted = " << activationSequenceCompleted << endl; 
	#endif
		
	#ifdef SANI_PARSE
	SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner = ownerGroup->currentParseTreeGroupTemp;
	#endif
	

	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDCONNECTIVITY_VERIFICATION_CONTINUOUS
	SANIPropagateOperations.verifyWordIndexCoverageIntegrity(forwardPropogationSentenceData, activationPathWordCurrentParseTreeGroupOwner, forwardPropogationWordData);
	#endif

	if(activationSequenceCompleted)
	{			
		ownerGroup->neuronActive = true;	//required for SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_ACTIVATION
		
		#ifdef SANI_SEQUENCE_GRAMMAR_WEIGHTS
		//if(!(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)) {
		ownerGroup->groupStrength = ownerGroup->groupStrength + 1;
		//}
		#endif
	
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA
		SANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
		cout << "activationSequenceCompleted" << endl;
		#endif
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR
		cout << "topLevelGroup0" << endl;
		#endif
		
		SANIForwardPropogationSignalData* semanticRelationReturnEntityForwardPropogationSignalDataNew = forwardPropogationSignalData;
		
	
		bool candidateCoveragePartial = false;	
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_WORD_INDEX_COVERAGE
		cout << "activationSequenceCompleted" << endl;
		#endif
		if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
		{
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_WORD_INDEX_COVERAGE
			cout << "(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)" << endl;
			#endif
			//consider relying on: activationPathWordCurrentParseTreeGroupOwner->missingOrVariableStartComponentFound/missingStartComponentFound, rather than passing missingOrVariableStartComponentFound/missingStartComponentFound
			updateActivatedNeuronWithMaxWordIndexCoverage(ownerGroup, activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, missingStartComponentFound, missingOrVariableStartComponentFound, variableStartComponentFound, candidateCoveragePartial);
		}
		
			
		bool topLevelGroup = false;
		//if(verifyActivatedNeuronWithMaxWordIndexAllowed(forwardPropogationSentenceData, forwardPropogationSignalData, activationPathWordCurrentParseTreeGroupOwner))
		//{	
		#ifdef SANI_SEQUENCE_GRAMMAR_ORIG
		if(verifyActivatedNeuronWithMaxWordIndexCoverage(forwardPropogationSentenceData, forwardPropogationSignalData, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage, activationPathWordCurrentParseTreeGroupOwner, testWordIndicesAllowed, testMinNumComponents, candidateCoveragePartial, missingOrVariableStartComponentFound))
		{	
		#endif
		#ifdef SANI_PREVENT_RECORDING_TOP_LEVEL_NEURON_IF_RECORDING_MAX_COVERAGE_NEURON
		if(!(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage))
		{			
		#endif
			#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
			int maxWeight;
			SANIPropagateOperations.calculatePerformanceWeightOfParseTree(activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSentenceData, &maxWeight);
			if(maxWeight >= parseTreeMaxWeight)
			{
			#endif				
				int wComparisonInt;
				if(forwardPropogationSentenceData->parseSentenceReverse)
				{
					wComparisonInt = 0;	//start of sentence found
				}
				else
				{
					wComparisonInt = forwardPropogationSentenceData->sentenceContents->size()-1;	//end of sentence punctuation mark found
				}
				if(forwardPropogationWordData->w == wComparisonInt)	//start of sentence found
				{
					int topLevelParseTreeGroupWordIndexCoverage = SANIPropagateOperations.calculateCoverage(activationPathWordCurrentParseTreeGroupOwner);

					if(topLevelParseTreeGroupWordIndexCoverage == forwardPropogationSentenceData->sentenceContents->size())
					{
						#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_ONLY_ONE_TOP_LEVEL_NEURON_FOUND_PER_SENTENCE
						if(forwardPropogationSentenceData->toplevelGroupActivationFound)
						{
							cerr << "SANIPropagateCompactClass::propagateWordThroughNetworkGroupComponent - topLevelGroup && already found (forwardPropogationSentenceData->toplevelGroupActivationFound)" << endl;
							exit(EXIT_ERROR);
						}
						else
						{
						#endif
						
							#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR
							int leafSize = SANIPropagateOperations.countParseTreeLeafSize(activationPathWordCurrentParseTreeGroupOwner);
							int maxLeafSize = 0;
							int maxDepth = 0;
							#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
							SANIPropagateOperations.getNeuralNetworkDepth(ownerGroup, &maxDepth);
							#else
							SANIPropagateOperations.countNeuralNetworkMaxLeafSizeAndDepth(ownerGroup, &maxLeafSize, &maxDepth);
							#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
							SANIPropagateOperations.countNeuralNetworkMaxLeafSizeAndDepthReset(ownerGroup);
							#endif
							#endif
							cout << "topLevelGroup" << endl;
							cout << "\tparseTree leafSize = " << leafSize << endl;
							cout << "\tneuralNetwork maxLeafSize = " << maxLeafSize << endl;
							cout << "\tneuralNetwork maxDepth = " << maxDepth << endl;
							#endif


							#ifdef SANI_SEQUENCE_WORDCONNECTIVITY_VERIFICATION
							if(!SANIPropagateOperations.updatePerformanceGroup(activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSentenceData, layer))
							{
								cerr << "SANI_SEQUENCE_WORDCONNECTIVITY_VERIFICATION; SANIPropagateCompactClass::propagateWordThroughNetworkGroupComponent error: !SANIPropagateOperations.updatePerformance" << endl;
								cerr << "forwardPropogationSentenceData->performance = " << forwardPropogationSentenceData->performance << endl;
								cerr << "forwardPropogationSentenceData->sentenceContents->size() = " << forwardPropogationSentenceData->sentenceContents->size() << endl;
								SANIPropagateOperations.printParseTree(activationPathWordCurrentParseTreeGroupOwner, 0);
								exit(EXIT_ERROR);
							}
							#endif

							topLevelGroup = true;

							forwardPropogationSentenceData->finishedPassingSentenceWords = true;

							forwardPropogationSentenceData->toplevelGroupActivationFound = true;

							parseTreeMaxWeight = maxWeight;

							#ifdef SANI_PARSE_SAVE_PARSE_TREE
							#ifdef SANI_REPLICATE_TOP_LEVEL_PARSE_TREE
							SANIPropagateOperations.deleteParseTree(topLevelParseTreeGroupLocalCompact, 0);
							topLevelParseTreeGroupLocalCompact = SANIPropagateOperations.replicateParseTree(activationPathWordCurrentParseTreeGroupOwner, 0);
							#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS_POS_TYPES
							//printBackpropParseTree(topLevelParseTreeGroupLocalCompact, 3);
							#endif
							#else
							//OLD: topLevelParseTreeGroupLocalCompact = activationPathWordCurrentParseTreeGroupOwner;
							//copy currentParseTreeGroupTemp so it cant be overwritten;
							topLevelParseTreeGroupLocalCompact = GIAposRelTranslatorRules.copyGroup(activationPathWordCurrentParseTreeGroupOwner);
							#endif
							#endif

							#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS_POS_TYPES
							cout << "topLevelGroup" << endl;
							#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
							cout << "parseTreeMaxWeight1 = " << parseTreeMaxWeight << endl;
							#endif
							cout << "topLevelParseTreeGroupLocalCompact->groupName = " << activationPathWordCurrentParseTreeGroupOwner->groupName << endl;
							printBackpropParseTree(activationPathWordCurrentParseTreeGroupOwner, 3);
							#endif
						#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_ONLY_ONE_TOP_LEVEL_NEURON_FOUND_PER_SENTENCE
						}
						#endif
					}
				}

			#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
			}
			#endif
		#ifdef SANI_PREVENT_RECORDING_TOP_LEVEL_NEURON_IF_RECORDING_MAX_COVERAGE_NEURON
		}
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_ORIG
		}	
		#endif
				
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
		if(!missingOrVariableStartComponentFound)	//prevents propagation up if variableStartComponentFound
		{
		#endif	

			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS
			if(activationPathWordCurrentParseTreeGroupOwner->components.size() == 1)
			{
				cout << "SANIPropagateCompactClass::propagateWordThroughNetworkGroupComponent: activationSequenceCompleted; activationPathWordCurrentParseTreeGroupOwner->components.size() == 1" << endl;
				cout << "ownerGroup->components.size() = " << ownerGroup->components.size() << endl;
				exit(EXIT_ERROR);
			}
			#endif

			if(!topLevelGroup)
			{
				#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_SUPPORT_RECURSION
				ownerGroup->neuronPropagated = true;
				#endif
								
				//activation sequence completed (and not top level group), propagate next layer up
				if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroup, semanticRelationReturnEntityForwardPropogationSignalDataNew, forwardPropogationWordData, forwardPropogationSentenceData, (layer+1), activationPathWordCurrentParseTreeGroupOwner))
				{
					//result = true;
				}

				#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_SUPPORT_RECURSION
				ownerGroup->neuronPropagated = false;
				#endif
			}
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
		}
		#endif
		//}
	}

	return result;
}


#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
bool SANIPropagateCompactClass::verifyMissingOrVariableStartComponentFound(GIAtranslatorVariablesClass* translatorVariables, int layer, SANIGroupNeuralNetwork* ownerGroup, SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, const bool firstActiveComponentInGroup, SANIComponentNeuralNetwork* previousActiveComponent, SANIComponentNeuralNetwork* finalActiveComponent, bool* existingActivationFoundStartComponent, bool* missingStartComponentFound, bool* variableStartComponentFound, const bool componentWordConnectivityTests, bool* missingOrVariableStartComponentFound, const int numberOfInactiveComponentsRemaining, bool* sequentialActivationFound, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup)
{
	bool result = true;
	
	//consider moving componentWordConnectivityTests test into SANIPropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady setting of missingOrVariableStartComponentFound
	if(componentWordConnectivityTests)
	{
		*sequentialActivationFound = true;	//redundant
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS_VARIABLE
		if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
		{
			if(!*missingStartComponentFound)
			{
				if(*missingOrVariableStartComponentFound)
				{
					//guaranteed: testComponent->w != 0 [ie second+ component]

					*missingOrVariableStartComponentFound = false;	//the first component found was variable and correct
					if(numberOfInactiveComponentsRemaining == 0)
					{
						if(*activationSequenceCompleted != true)
						{
							cout << "SANIPropagateCompactClass::propagateWordThroughNetworkGroupComponentWrapper warning: (numberOfInactiveComponentsRemaining == 0) && (activationSequenceCompleted != true)" << endl;
						}
						*activationSequenceCompleted = true;	//this be already marked as true by SANIPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady
					}
				}
			}
		}
		#endif
	}

	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
	if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
	{
		if(*missingOrVariableStartComponentFound)
		{
			//guaranteed: (missingStartComponentFound || missingStartComponentFound) && testComponent->w != 0 [ie second+ component]

			//test verifyActivatedNeuronWithMaxWordIndexAllowed:
			bool firstLastWordIndexTestHypotheticalWithoutVariableStartComponent = false; 
			SANIGroupParseTree activatedNeuronCandidate;	//activatedNeuronCandidate is only used for both verifyActivatedNeuronWithMaxWordIndexAllowed purposes and updating ownerGroup->currentParseTreeGroupTemp->parseTreeMinWordIndex/parseTreeMaxWordIndex in the case a variable first component is detected
			activatedNeuronCandidate.parseTreeMaxWordIndex = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
			activatedNeuronCandidate.parseTreeMinWordIndex = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
			//updateParseTreeMaxMinWordIndexOfParentBasic will update activatedNeuronCandidate based on activationPathWordCurrentParseTreeGroup only (existing group activation parseTreeMaxWordIndex/parseTreeMinWordIndex will be disgarded as the first component is variable)			
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS_VARIABLE
			if(firstActiveComponentInGroup || ((previousActiveComponent != NULL) && (ownerGroup->currentParseTreeGroupTemp->components.size() == 1)))	//ie if missing start component || variable start component	//ie no components have previously been activated (firstActiveComponentInGroup) || 1 component has previously been activated ((previousActiveComponent != NULL) && (ownerGroup->currentParseTreeGroupTemp->components.size() == 1))
			{
			#endif
				updateParseTreeMaxMinWordIndexOfParentBasic(&activatedNeuronCandidate, forwardPropogationWordData, activationPathWordCurrentParseTreeGroup);
				if(verifyActivatedNeuronWithMaxWordIndexAllowed(forwardPropogationSentenceData, forwardPropogationSignalData, &activatedNeuronCandidate, true))
				{
					firstLastWordIndexTestHypotheticalWithoutVariableStartComponent = true;
				}
				else
				{
					*missingOrVariableStartComponentFound = false;
					#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_SIMPLIFIED
					*sequentialActivationFound = false;
					#endif
				}
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS_VARIABLE
			}
			else
			{
				cerr << "potential algorithm error; if(missingOrVariableStartComponentFound); the following should always be true: if(firstActiveComponentInGroup || ((previousActiveComponent != NULL) && (ownerGroup->currentParseTreeGroupTemp->components.size() == 1))" << endl;
				exit(EXIT_ERROR);
			}
			#endif

			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS_VARIABLE
			if(firstLastWordIndexTestHypotheticalWithoutVariableStartComponent)
			{
				if(*missingOrVariableStartComponentFound)
				{
					if(!(*sequentialActivationFound))
					{
						if((previousActiveComponent != NULL) && (ownerGroup->currentParseTreeGroupTemp->components.size() == 1))	//only 1 component has been activated	//ie (!*missingStartComponentFound)
						{
							//variable first component detected
							//word indices do not align (ie !sequentialActivationFound), so assume first component was miscellaneously activated (mark it as variable) 

							*variableStartComponentFound = true;
							if(sequentialActivationConnectivityTests(translatorVariables, testComponent, ownerGroup, &activatedNeuronCandidate, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, activationPathWordCurrentParseTreeGroup, *existingActivationFoundStartComponent, variableStartComponentFound))	//CHECKTHIS
							{
								/*update sequentialActivationFound value->true under the assumption the first component is being modified with a new (variable) connection [therefore wordIndex alignment with second component is not required]. 
								This is only required for propagateWordThroughNetworkGroupComponent:updateActivatedNeuronWithMaxWordIndexCoverage to execute [ie the detection/designation of a fully activatedNeuronWithMaxWordIndexCoverage with variable first component]*/
								*sequentialActivationFound = true;	
								
								//ownerGroup->currentParseTreeGroupTemp->variableStartComponentFound = true;
								ownerGroup->currentParseTreeGroupTemp->parseTreeMinWordIndex = activatedNeuronCandidate.parseTreeMinWordIndex;
								ownerGroup->currentParseTreeGroupTemp->parseTreeMaxWordIndex = activatedNeuronCandidate.parseTreeMaxWordIndex;
							}
							else
							{
								*missingOrVariableStartComponentFound = false;
							}
						}
					}	
				}
			}
			#endif
		}

		if(*missingOrVariableStartComponentFound)
		{
			ownerGroup->currentParseTreeGroupTemp->missingOrVariableStartComponentFound = true;
		}
		/*
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS
		if(missingStartComponentFound)
		{
			ownerGroup->currentParseTreeGroupTemp->missingStartComponentFound = true;
		}
		#endif
		*/
	}
	#endif
	
	return result;
}
bool SANIPropagateCompactClass::verifyMissingOrVariableStartComponentFoundAllowedWordIndices(GIAtranslatorVariablesClass* translatorVariables, int layer, SANIGroupNeuralNetwork* ownerGroup, SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, const bool firstActiveComponentInGroup, SANIComponentNeuralNetwork* previousActiveComponent, SANIComponentNeuralNetwork* finalActiveComponent, bool* existingActivationFoundStartComponent, bool* missingStartComponentFound, bool* variableStartComponentFound, const bool componentWordConnectivityTests, bool* missingOrVariableStartComponentFound, const int numberOfInactiveComponentsRemaining, bool* sequentialActivationFound, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup)
{
	bool result = true;
	
	//#ifndef SANI_SEQUENCE_GRAMMAR_ORIG
	//moved GIA3j5aTEMP36 from old SANIPropagateCompactClass::propagateWordThroughNetworkGroupComponent:if(activationSequenceCompleted), verifyActivatedNeuronWithMaxWordIndexCoverage checks
	//CHECKTHIS - the activation tree being fed into the missing or variable start component must encapsulate the entire word index segment from the start of the global propagation required wordIndex (activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed):
	bool strictStartingCondition = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS_VARIABLE
	if(*missingStartComponentFound || (ownerGroup->currentParseTreeGroupTemp->missingOrVariableStartComponentFound))	//or: || missingOrVariableStartComponentFound
	#else
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_SIMPLIFIED
	if(*missingOrVariableStartComponentFound)
	#else
	if(*missingStartComponentFound)
	#endif
	#endif
	{
		strictStartingCondition = true;	
	}
	SANIGroupParseTree activatedNeuronCandidate = *(ownerGroup->currentParseTreeGroupTemp);	//activatedNeuronCandidate is only used for verifyActivatedNeuronWithMaxWordIndexAllowed purposes
	if(firstActiveComponentInGroup)
	{
		activatedNeuronCandidate.parseTreeMaxWordIndex = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
		activatedNeuronCandidate.parseTreeMinWordIndex = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
		//updateParseTreeMaxMinWordIndexOfParentBasic will update activatedNeuronCandidate based on activationPathWordCurrentParseTreeGroup only
	}
	updateParseTreeMaxMinWordIndexOfParentBasic(&activatedNeuronCandidate, forwardPropogationWordData, activationPathWordCurrentParseTreeGroup);
	if(!verifyActivatedNeuronWithMaxWordIndexAllowed(forwardPropogationSentenceData, forwardPropogationSignalData, &activatedNeuronCandidate, strictStartingCondition))
	{
		(*sequentialActivationFound) = false;
	}	
	//#endif
	
	return result;
}
#endif
#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_LAST_COMPONENTS
bool SANIPropagateCompactClass::propagateVariableEndComponentRemoveLastParseTreeComponent(GIAtranslatorVariablesClass* translatorVariables, int layer, SANIGroupNeuralNetwork* ownerGroup, SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, SANIComponentNeuralNetwork* previousActiveComponent, SANIComponentNeuralNetwork* finalActiveComponent, bool* existingActivationFoundEndComponent, const bool componentWordConnectivityTests, bool* missingOrVariableEndComponentFound, bool* sequentialActivationFound, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup)
{
	bool result = true;

	if(componentWordConnectivityTests)
	{
		*sequentialActivationFound = true;	//redundant
	}
		
	if(*existingActivationFoundEndComponent)
	{
		//test verifyActivatedNeuronWithMaxWordIndexAllowed:
		
		bool firstLastWordIndexTestHypotheticalWithVariableEndComponent = false; 
				
		SANIGroupParseTree activatedNeuronCandidate;	//activatedNeuronCandidate is only used for both verifyActivatedNeuronWithMaxWordIndexAllowed purposes and updating ownerGroup->currentParseTreeGroupTemp->parseTreeMinWordIndex/parseTreeMaxWordIndex in the case a variable first component is detected

		if(ownerGroup->currentParseTreeGroupTemp->components.size() == 0)
		{
			cerr << "SANIPropagateCompactClass::propagateVariableEndComponentRemoveLastParseTreeComponent error: (ownerGroup->currentParseTreeGroupTemp->components.size() == 0)" << endl;
			exit(EXIT_ERROR);
		}
		
		//this code is derived from SANIPropagateCompactClass::verifyMissingOrVariableStartComponentFound;
		#ifdef SANI_PARSE_SAVE_PARSE_TREE
		//get first component in parseTreeGroup:
		SANIComponentParseTree* firstComponentParseTree = SANIPropagateOperations.getFirstComponent(forwardPropogationSentenceData, ownerGroup->currentParseTreeGroupTemp, true);
		if(firstComponentParseTree->parseTreeGroupRef != NULL)
		{
			activatedNeuronCandidate.parseTreeMaxWordIndex = firstComponentParseTree->parseTreeGroupRef->parseTreeMaxWordIndex;
			activatedNeuronCandidate.parseTreeMinWordIndex = firstComponentParseTree->parseTreeGroupRef->parseTreeMinWordIndex;		
		}
		else
		{
			activatedNeuronCandidate.parseTreeMaxWordIndex = firstComponentParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
			activatedNeuronCandidate.parseTreeMinWordIndex = firstComponentParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;	
		}
		#else
		//SANIComponentNeuralNetwork* firstComponent = SANIPropagateOperations.getFirstComponent(forwardPropogationSentenceData, ownerGroup, true);
		cerr << "SANIPropagateCompactClass::propagateVariableEndComponentRemoveLastParseTreeComponent currently requires SANI_PARSE_SAVE_PARSE_TREE" << endl;
		exit(EXIT_ERROR);
		#endif
		
		updateParseTreeMaxMinWordIndexOfParentBasic(&activatedNeuronCandidate, forwardPropogationWordData, activationPathWordCurrentParseTreeGroup);
		if(verifyActivatedNeuronWithMaxWordIndexAllowed(forwardPropogationSentenceData, forwardPropogationSignalData, &activatedNeuronCandidate, true))
		{
			firstLastWordIndexTestHypotheticalWithVariableEndComponent = true;
		}
		else
		{
			*existingActivationFoundEndComponent = false;
			*sequentialActivationFound = false;
		}

		if(firstLastWordIndexTestHypotheticalWithVariableEndComponent)
		{
			//this code is derived from SANIPropagateCompactClass::propagateWordThroughNetworkGroupComponent (first section) and could be moved there (but would require rederivation of activatedNeuronCandidate)
			if(*existingActivationFoundEndComponent)
			{
				*activationSequenceCompleted = true;	//added GIA3l8e

				//readjust ownerGroup->currentParseTreeGroupTemp including parseTreeMaxWordIndex/parseTreeMinWordIndex (remove last component) 

				#ifdef SANI_DEBUG_PROPAGATE
				cout << "******** reduceGroupParseTreeGroupRef, groupIndex = " << ownerGroup->groupIndex << endl;
				#endif
				SANIGroupParseTree* parseTreeGroupRecord = ownerGroup->currentParseTreeGroupTemp;	//record reference to ownerGroup->currentParseTreeGroupTemp before resetting

				//SANIPropagateOperations.resetGroupActivation(ownerGroup);	//not required as last component values will shortly be overwritten
				SANIPropagateOperations.resetGroupParseTreeGroupRef(ownerGroup, false);	

				*(ownerGroup->currentParseTreeGroupTemp) = *(parseTreeGroupRecord);	//replicate	//CHECKTHIS
				//or ownerGroup->currentParseTreeGroupTemp = new SANIGroupParseTree(*(parseTreeGroupRecord));	//NO: not allowed because ownerGroup->currentParseTreeGroupTemp has already been added to ownerGroup->parseTreeGroupMemory?

				SANIGroupParseTree* currentParseTreeGroupTemp = ownerGroup->currentParseTreeGroupTemp;	//new ownerGroup->currentParseTreeGroupTemp
				currentParseTreeGroupTemp->components.clear();
				SANIComponentParseTree* newFirstParseComponent = new SANIComponentParseTree(*SANIPropagateOperations.getFirstComponent(forwardPropogationSentenceData, parseTreeGroupRecord, true)); 	//replicate first parse tree component
				if(forwardPropogationSentenceData->parseSentenceReverse)
				{
					//currentParseTreeGroupTemp->components.push_front(newParseComponent);
					currentParseTreeGroupTemp->components.insert(currentParseTreeGroupTemp->components.begin(), newFirstParseComponent);
				}
				else
				{
					currentParseTreeGroupTemp->components.push_back(newFirstParseComponent);
				}

				#ifdef SANI_FREE_MEMORY
				parseTreeGroupListCompact.push_back(ownerGroup->currentParseTreeGroupTemp);
				#endif	
				#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
				//ownerGroup->parseTreeGroupMemory.push_back(ownerGroup->currentParseTreeGroupTemp);	//not required (as memory is only currently used for groups with first component active)
				#endif

				ownerGroup->currentParseTreeGroupTemp->parseTreeMinWordIndex = activatedNeuronCandidate.parseTreeMinWordIndex;
				ownerGroup->currentParseTreeGroupTemp->parseTreeMaxWordIndex = activatedNeuronCandidate.parseTreeMaxWordIndex;
			}
		}
	}
	
	return result;
}
#endif	
		



bool SANIPropagateCompactClass::updateActivatedNeuronWithMaxWordIndexCoverage(SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* currentParseTreeGroupTemp, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool missingStartComponentFound, const bool missingOrVariableStartComponentFound, const bool variableStartComponentFound, const bool candidateCoveragePartial)
{
	bool result = true;
	
	bool activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet;
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS_SIMPLIFY
	activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet = currentParseTreeGroupTemp->missingOrVariableStartComponentFound;	//or missingOrVariableStartComponentFound?
	#else
	if(candidateCoveragePartial)
	{
		activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet = currentParseTreeGroupTemp->missingOrVariableStartComponentFound;
	}
	else
	{
		activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet = missingOrVariableStartComponentFound;
	}
	#endif
	#endif
	
	bool testWordIndicesAllowed = true;
	bool testMinNumComponents = true;
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
	if(activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet)
	{
		testWordIndicesAllowed = true;
		testMinNumComponents = false;
	}
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENTS
	if(candidateCoveragePartial)
	{
		//cout << "\t1 candidateCoveragePartial" << endl;
		testWordIndicesAllowed = true;
		testMinNumComponents = false;
	}
	#endif
				
	if(verifyActivatedNeuronWithMaxWordIndexCoverage(forwardPropogationSentenceData, forwardPropogationSignalData, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage, currentParseTreeGroupTemp, testWordIndicesAllowed, testMinNumComponents, candidateCoveragePartial, missingOrVariableStartComponentFound))
	{		
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_WORD_INDEX_COVERAGE
		cout << "verifyActivatedNeuronWithMaxWordIndexCoverage" << endl;
		#endif

		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_ENFORCE_FIRST_COMPONENT_NOT_MISSING
		if(!missingStartComponentFound)
		{
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_ENFORCE_FIRST_COMPONENT_NOT_VARIABLE
		if(!missingOrVariableStartComponentFound)
		{
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENTS
		if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableEndComponent || !candidateCoveragePartial)
		{
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS
		if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent || !missingOrVariableStartComponentFound)
		{
		#endif
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_WORD_INDEX_COVERAGE
			cout << "(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent || !missingOrVariableStartComponentFound)" << endl;
			#endif
			
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENTS_LAST_COMP_REQUIRE
			int variableComponentMaxDepth = 0;
			int variableComponentMaxLeafSize = 0;
			if(candidateCoveragePartial)
			{
				SANIComponentNeuralNetwork* lastComponent = SANIPropagateOperations.getFirstComponent(forwardPropogationSentenceData, ownerGroup, false);
				if(lastComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
				{
					cerr << "SANIPropagateCompactClass::updateActivatedNeuronWithMaxWordIndexCoverage error: (lastComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)" << endl;
					exit(EXIT_ERROR);
				}
				#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
				SANIPropagateOperations.getNeuralNetworkDepth(lastComponent, &variableComponentMaxDepth);
				#else
				SANIPropagateOperations.countNeuralNetworkMaxLeafSizeAndDepth(lastComponent, &variableComponentMaxLeafSize, &variableComponentMaxDepth);
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
				SANIPropagateOperations.countNeuralNetworkMaxLeafSizeAndDepthReset(lastComponent);
				#endif
				#endif
			}
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_WORD_INDEX_COVERAGE
			cout << "\n\t\tvariableComponentMaxDepth = " << variableComponentMaxDepth << endl;
			cout << "\t\tforwardPropogationSentenceData->variableFirstComponentMaxDepth = " << forwardPropogationSentenceData->variableFirstComponentMaxDepth << endl;
			cout << "\t\tmissingOrVariableStartComponentFound = " << missingOrVariableStartComponentFound << endl;			
			//cout << "SANIPropagateOperations.countParseTreeLeafSize(currentParseTreeGroupTemp) = " << SANIPropagateOperations.countParseTreeLeafSize(currentParseTreeGroupTemp) << endl;
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENTS_LAST_COMP_REQUIRE_MATCHING_DEPTH
			if(!candidateCoveragePartial || variableComponentMaxDepth == forwardPropogationSentenceData->variableFirstComponentMaxDepth)
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENTS_LAST_COMP_REQUIRE_SIZEABLE_SUBTREE
			if(!candidateCoveragePartial || variableComponentMaxDepth >= SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENTS_LAST_COMP_REQUIRE_SIZEABLE_SUBTREE_MIN_NEURONS)
			#endif
			{
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_LAST_COMP_REQUIRE_SIZEABLE_SUBTREE
			if(!missingOrVariableStartComponentFound || SANIPropagateOperations.countParseTreeLeafSize(currentParseTreeGroupTemp) >= SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_LAST_COMP_REQUIRE_SIZEABLE_SUBTREE_MIN_NEURONS)
			{
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_FIRST_COMP_REQUIRE
			int variableComponentMaxDepth = 0;
			int variableComponentMaxLeafSize = 0;
			if(missingOrVariableStartComponentFound)
			{
				SANIComponentNeuralNetwork* startComponent = SANIPropagateOperations.getFirstComponent(forwardPropogationSentenceData, ownerGroup, true);
				if(startComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
				{
					cerr << "SANIPropagateCompactClass::updateActivatedNeuronWithMaxWordIndexCoverage error: (startComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)" << endl;
					//cout << "candidateCoveragePartial = " << candidateCoveragePartial << endl;
					exit(EXIT_ERROR);
				}
				#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
				SANIPropagateOperations.getNeuralNetworkDepth(startComponent, &variableComponentMaxDepth);
				#else
				SANIPropagateOperations.countNeuralNetworkMaxLeafSizeAndDepth(startComponent, &variableComponentMaxLeafSize, &variableComponentMaxDepth);
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
				SANIPropagateOperations.countNeuralNetworkMaxLeafSizeAndDepthReset(startComponent);
				#endif
				#endif
			}
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_WORD_INDEX_COVERAGE
			cout << "\n\t\tvariableComponentMaxDepth = " << variableComponentMaxDepth << endl;
			cout << "\t\tforwardPropogationSentenceData->variableFirstComponentMaxDepth = " << forwardPropogationSentenceData->variableFirstComponentMaxDepth << endl;
			cout << "\t\tmissingOrVariableStartComponentFound = " << missingOrVariableStartComponentFound << endl;			
			//cout << "SANIPropagateOperations.countParseTreeLeafSize(currentParseTreeGroupTemp) = " << SANIPropagateOperations.countParseTreeLeafSize(currentParseTreeGroupTemp) << endl;
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_FIRST_COMP_REQUIRE_MATCHING_DEPTH
			if(!missingOrVariableStartComponentFound || variableComponentMaxDepth == forwardPropogationSentenceData->variableFirstComponentMaxDepth)	//ORIGSPEC //intermediary: variableComponentMaxDepth < forwardPropogationSentenceData->variableFirstComponentMaxDepth
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_FIRST_COMP_REQUIRE_SIZEABLE_SUBTREE
			if(!missingOrVariableStartComponentFound || variableComponentMaxDepth >= SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_FIRST_COMP_REQUIRE_SIZEABLE_SUBTREE_MIN_NEURONS)
			#endif
			{
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_RANDOMISE
			double randomProb = SANIPropagateOperations.generateRandomNumber();
			if(randomProb > SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_RANDOMISE_PROB)
			{
			#endif
				//cout << "\t2 candidateCoveragePartial = " << candidateCoveragePartial << endl;
				
				//cout << "missingStartComponentFound = " << missingStartComponentFound << endl;
				
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR
				cout << "++++++++++++++++++++++++++++++++++++++++++ forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage" << endl;
				cout << "\tactivatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet = " << activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet << endl;		
				#endif
				//cout << "\tactivatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet = " << activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet << endl;		
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_WORD_INDEX_COVERAGE
				cout << "\tcandidateCoveragePartial = " << candidateCoveragePartial << endl;
				cout << "\tforwardPropogationWordData->w = " << forwardPropogationWordData->w << endl;
				#endif
				//cout << "candidateCoveragePartial = " << candidateCoveragePartial << endl;
				
				forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage = currentParseTreeGroupTemp;
				forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial = candidateCoveragePartial;
				#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
				forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent = activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet;
				#endif
				/*
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS
				forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageMissingStartComponent = activatedNeuronWithMaxWordIndexCoverageMissingStartComponentSet;	//NOTUSED
				#endif
				*/
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_RANDOMISE
			}
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_FIRST_COMP_REQUIRE
			}
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_LAST_COMP_REQUIRE_SIZEABLE_SUBTREE
			}
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENTS_LAST_COMP_REQUIRE
			}
			#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS
		}
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENTS
		}
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_ENFORCE_FIRST_COMPONENT_NOT_VARIABLE
		}
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_ENFORCE_FIRST_COMPONENT_NOT_MISSING
		}
		#endif
	}
	
	return result;
}



bool SANIPropagateCompactClass::sequentialActivationConnectivityTests(GIAtranslatorVariablesClass* translatorVariables, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* currentParseTreeGroupTemp, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const bool existingActivationFoundStartComponent, const bool existingActivationFoundEndComponent, const bool skipWordConnectivityTests)
{	
	bool sequentialActivationFound = false;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP
	if(upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponentWrapper(forwardPropogationSentenceData, forwardPropogationSignalData, forwardPropogationWordData, currentComponent, ownerGroup, activationPathWordCurrentParseTreeGroup, existingActivationFoundStartComponent))
	{
	#endif
		bool passBasicWordConnectivityTest = true;
		if(!skipWordConnectivityTests)
		{
			#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
			#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_PRECHECKS		
			#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_PRECHECKS2
			if(currentComponent != SANIPropagateOperations.getFirstComponent(forwardPropogationSentenceData, ownerGroup, true))
			{
			#endif
				if(!existingActivationFoundStartComponent)
				{
					bool groupHasPreceedingComponent = false;
					if(currentParseTreeGroupTemp->components.size() > 0)
					{
						groupHasPreceedingComponent = true;
					}
					if(groupHasPreceedingComponent)
					{
			#endif
						passBasicWordConnectivityTest = false;
						if(SANIPropagateOperations.componentWordConnectivityTestsWrapper(ownerGroup, currentParseTreeGroupTemp, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData, existingActivationFoundStartComponent, existingActivationFoundEndComponent))
						{
							//cout << "passBasicWordConnectivityTest" << endl;
							passBasicWordConnectivityTest = true;
						}
			#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_PRECHECKS			
					}
				}
			#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_PRECHECKS2
			}
			#endif
			#endif
			#endif
		}
		
		if(passBasicWordConnectivityTest)
		{			
			/*
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
			Note if variableStartComponentFound, then componentWordConnectivityTestsWrapper is not required to be reexecuted, since if the previous component was variable (ie miscellaneously activated) and components->size()==1, no word index connectivity test is required to be conducted. 
			If the first component was variable, but components->size() >= 2, then the previous component would be valid and would already have been tested when componentWordConnectivityTestsWrapper was executed the first time
			#endif
			*/
		
			sequentialActivationFound = true;

		}
	#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP
	}
	#endif	
	
	return sequentialActivationFound;
}	


	
void SANIPropagateCompactClass::updateParseTreeMaxMinWordIndexOfParent(SANIGroupParseTree* parentGroup, SANIComponentParseTree* parentComponent, SANIGroupParseTree* childGroup)
{
	#ifdef SANI_PARSE_GENERATE_PARSE_TREE
	if(parentComponent->parseTreeGroupRef != NULL)
	#else
	if(childGroup != NULL)
	#endif
	{	
		//has children
		#ifdef SANI_PARSE_GENERATE_PARSE_TREE
		childGroup = parentComponent->parseTreeGroupRef;	//not required
		#endif
		if(childGroup->parseTreeMaxWordIndex > parentGroup->parseTreeMaxWordIndex)
		{
			parentGroup->parseTreeMaxWordIndex = childGroup->parseTreeMaxWordIndex;
		}
		if(childGroup->parseTreeMinWordIndex < parentGroup->parseTreeMinWordIndex)
		{
			parentGroup->parseTreeMinWordIndex = childGroup->parseTreeMinWordIndex;
		}
		
		#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
		if(childGroup->parseTreeMaxWeight > parentGroup->parseTreeMaxWeight)
		{
			parentGroup->parseTreeMaxWeight = childGroup->parseTreeMaxWeight;
		}
		if(childGroup->parseTreeMinWeight < parentGroup->parseTreeMinWeight)
		{
			parentGroup->parseTreeMinWeight = childGroup->parseTreeMinWeight;
		}
		parentGroup->parseTreeTotalWeight = parentGroup->parseTreeTotalWeight + childGroup->parseTreeTotalWeight;
		#endif
	}
	else
	{
		if(parentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex > parentGroup->parseTreeMaxWordIndex)
		{
			parentGroup->parseTreeMaxWordIndex = parentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
		}
		if(parentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex < parentGroup->parseTreeMinWordIndex)
		{
			parentGroup->parseTreeMinWordIndex = parentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
		}
	
		/*//FUTURE; take into account component weights;
		#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
		if(parentComponent->parseTreeMaxWeight > parentGroup->parseTreeMaxWeight)
		{
			parentGroup->parseTreeMaxWeight = parentComponent->parseTreeMaxWeight;
		}
		if(parentGroup->parseTreeMinWeight < parentGroup->parseTreeMinWeight)
		{
			parentGroup->parseTreeMinWeight = parentComponent->parseTreeMinWeight;
		}
		parentGroup->parseTreeTotalWeight = parentGroup->parseTreeTotalWeight + parentComponent->parseTreeTotalWeight;	
		#endif	
		*/
	}
}

void SANIPropagateCompactClass::updateParseTreeMaxMinWordIndexOfParentBasic(SANIGroupParseTree* parentGroup, SANIForwardPropogationWordData* forwardPropogationWordData, SANIGroupParseTree* childGroup)
{
	if(childGroup != NULL)
	{	
		//has children
		if(childGroup->parseTreeMaxWordIndex > parentGroup->parseTreeMaxWordIndex)
		{
			parentGroup->parseTreeMaxWordIndex = childGroup->parseTreeMaxWordIndex;
		}
		if(childGroup->parseTreeMinWordIndex < parentGroup->parseTreeMinWordIndex)
		{
			parentGroup->parseTreeMinWordIndex = childGroup->parseTreeMinWordIndex;
		}
		
		/*
		#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
		if(childGroup->parseTreeMaxWeight > parentGroup->parseTreeMaxWeight)
		{
			parentGroup->parseTreeMaxWeight = childGroup->parseTreeMaxWeight;
		}
		if(childGroup->parseTreeMinWeight < parentGroup->parseTreeMinWeight)
		{
			parentGroup->parseTreeMinWeight = childGroup->parseTreeMinWeight;
		}
		parentGroup->parseTreeTotalWeight = parentGroup->parseTreeTotalWeight + childGroup->parseTreeTotalWeight;
		#endif
		*/
	}
	else
	{
		if(forwardPropogationWordData->wordReference->translatorSentenceWordIndex > parentGroup->parseTreeMaxWordIndex)
		{
			parentGroup->parseTreeMaxWordIndex = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
		}
		if(forwardPropogationWordData->wordReference->translatorSentenceWordIndex < parentGroup->parseTreeMinWordIndex)
		{
			parentGroup->parseTreeMinWordIndex = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
		}
	
		/*//FUTURE; take into account component weights;
		#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
		if(parentComponent->parseTreeMaxWeight > parentGroup->parseTreeMaxWeight)
		{
			parentGroup->parseTreeMaxWeight = parentComponent->parseTreeMaxWeight;
		}
		if(parentGroup->parseTreeMinWeight < parentGroup->parseTreeMinWeight)
		{
			parentGroup->parseTreeMinWeight = parentComponent->parseTreeMinWeight;
		}
		parentGroup->parseTreeTotalWeight = parentGroup->parseTreeTotalWeight + parentComponent->parseTreeTotalWeight;	
		#endif	
		*/
	}
}



bool SANIPropagateCompactClass::printBackpropParseTree(SANIGroupParseTree* group, int level)
{
	bool print = true;
	bool performancePreprocess = false;
	int performanceNOTUSED = 0;
	SANIPropagateOperations.traceBackpropParseTree(group, 1, print, performancePreprocess, &performanceNOTUSED, NULL);
	SANIPropagateOperations.resetNeuronBackprop(group, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
}









bool SANIPropagateCompactClass::verifyActivatedNeuronWithMaxWordIndexCoverage(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIGroupParseTree* activatedNeuronBaseline, SANIGroupParseTree* activatedNeuronCandidate, const bool testWordIndicesAllowed, const bool testMinNumComponents, const bool candidateCoveragePartial, const bool candidateCoverageVariable)
{
	bool result = false;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDINDEX_VERIFICATION
	cout << "verifyActivatedNeuronWithMaxWordIndexCoverage START" << endl;
	#endif
	#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
	cout << "verifyActivatedNeuronWithMaxWordIndexCoverage START" << endl;
	#endif
					
	#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_ENSURE_ALL_NEURONS_IN_SUBGROUP_ARE_NOT_USED_BY_ANY_NEW_CANDIDATE
	bool candidateNeuronInCompleteHistory = false;
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
	if(activatedNeuronCandidate->groupRef->marked)
	{
		candidateNeuronInCompleteHistory = true;
		//cout << "candidateNeuronInCompleteHistory" << endl;
	}
	#endif
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_HISTORY
	for(int i=0; i<forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory.size(); i++)
	{
		if(activatedNeuronCandidate->groupRef == forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory[i])
		{
			candidateNeuronInCompleteHistory = true;	
		}
	}
	#endif
	if(!candidateNeuronInCompleteHistory)
	{
	#endif		
		#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
		cout << "!candidateNeuronInCompleteHistory" << endl;
		#endif

		//if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)	//assumed true
		//{
		#ifdef SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
		if(verifyNeverSplitGroupBetweenTwoIdenticalComponents(forwardPropogationSentenceData, activatedNeuronCandidate, candidateCoveragePartial))
		{
		#endif			
			#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
			cout << "pass verifyNeverSplitGroupBetweenTwoIdenticalComponents" << endl;
			#endif
			
			int activatedNeuronCandidateCoverage = 0;
			int activatedNeuronCandidateMaxWeight = 0;
			#ifdef SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS
			bool activatedNeuronCandidateMinNumComponentsTest = false;
			#endif
			if(activatedNeuronCandidate != NULL)
			{				
				activatedNeuronCandidateCoverage = SANIPropagateOperations.calculateCoverage(activatedNeuronCandidate);
				#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
				SANIPropagateOperations.calculatePerformanceWeightOfParseTree(activatedNeuronCandidate, forwardPropogationSentenceData, &activatedNeuronCandidateMaxWeight);
				#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS
				//cout << "activatedNeuronCandidate->components.size() = " << activatedNeuronCandidate->components.size() << endl;
				if(activatedNeuronCandidate->components.size() >= SANI_SEQUENCE_GRAMMAR_MIN_NUM_COMPONENTS)
				{
					activatedNeuronCandidateMinNumComponentsTest = true;
				}
				#endif

			}
			int activatedNeuronBaselineCoverage = 0;
			int activatedNeuronBaselineMaxWeight = 0;
			if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
			{
				if(activatedNeuronBaseline != NULL)
				{
					activatedNeuronBaselineCoverage = SANIPropagateOperations.calculateCoverage(activatedNeuronBaseline);
					#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
					//activatedNeuronBaselineMaxWeight = activatedNeuronBaselineCoverage->parseTreeMaxWeight;
					SANIPropagateOperations.calculatePerformanceWeightOfParseTree(activatedNeuronBaseline, forwardPropogationSentenceData, &activatedNeuronBaselineMaxWeight);
					#endif
				}
			}

			bool firstLastWordIndexTest = false;
			if(testWordIndicesAllowed)
			{	
				bool strictStartingCondition = true;
				firstLastWordIndexTest = verifyActivatedNeuronWithMaxWordIndexAllowed(forwardPropogationSentenceData, forwardPropogationSignalData, activatedNeuronCandidate, strictStartingCondition);
			}
			else
			{
				firstLastWordIndexTest = true;
			}
			
			#ifdef SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS
			if(!testMinNumComponents || activatedNeuronCandidateMinNumComponentsTest)
			{
			#endif				
				if(firstLastWordIndexTest)
				{	
					bool passBaselineComparisonTests = true;
					if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
					{						
						passBaselineComparisonTests = false;
		
						#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
						cout << "(firstWordIndexTest && lastWordIndexTest)" << endl;
						cout << "activatedNeuronCandidateCoverage = " << activatedNeuronCandidateCoverage << endl;
						cout << "activatedNeuronBaselineCoverage = " << activatedNeuronBaselineCoverage << endl;		
						#endif
						#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDINDEX_VERIFICATION
						cout << "(firstWordIndexTest && lastWordIndexTest)" << endl;
						cout << "activatedNeuronCandidateCoverage = " << activatedNeuronCandidateCoverage << endl;
						cout << "activatedNeuronBaselineCoverage = " << activatedNeuronBaselineCoverage << endl;
						#endif

						bool passBaselineCoverageTests = false;
						#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENTS_PREFERENCE_THEM
						if(candidateCoveragePartial && !candidateCoverageVariable)
						{	
							if(activatedNeuronCandidateCoverage == activatedNeuronBaselineCoverage)
							{
								passBaselineCoverageTests = true;
							}
						}	
						#endif
						#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_PREFERENCE_THEM
						if(candidateCoverageVariable && !candidateCoveragePartial)
						{	
							if(activatedNeuronCandidateCoverage == activatedNeuronBaselineCoverage)
							{
								passBaselineCoverageTests = true;
							}
						}
						#endif
						if(activatedNeuronCandidateCoverage > activatedNeuronBaselineCoverage)
						{
							passBaselineCoverageTests = true;
						}
						#ifndef SANI_SEQUENCE_GRAMMAR_IGNORE_PARTIAL_DETECTIONS
						if(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial && !(candidateCoveragePartial) && !(candidateCoverageVariable))
						{
							if(activatedNeuronCandidateCoverage == activatedNeuronBaselineCoverage)
							{
								passBaselineCoverageTests = true;	//allow fully active activatedNeuronCandidate to override partially active activatedNeuronBaseline, assuming they have the same coverage	//CHECKTHIS
							}
						}
						#endif
						#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
						if(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent && !(candidateCoverageVariable))
						{
							if(activatedNeuronCandidateCoverage == activatedNeuronBaselineCoverage)
							{
								passBaselineCoverageTests = true;	//allow activatedNeuronCandidate without missingOrVariableStartComponentFound to override activatedNeuronBaseline with missingOrVariableStartComponentFound, assuming they have the same coverage	//CHECKTHIS
							}
						}
						#endif

						if(passBaselineCoverageTests)
						{
							#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
							cout << "(activatedNeuronCandidateCoverage > activatedNeuronBaselineCoverage)" << endl;
							#endif
							#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDINDEX_VERIFICATION
							cout << "(activatedNeuronCandidateCoverage > activatedNeuronBaselineCoverage)" << endl;
							#endif
							#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
							#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_FIX
							if((activatedNeuronCandidateCoverage != activatedNeuronBaselineCoverage) || (activatedNeuronCandidateMaxWeight >= activatedNeuronBaselineMaxWeight))
							#else
							if(activatedNeuronCandidateMaxWeight >= activatedNeuronBaselineMaxWeight)
							#endif
							{
								//SANIPropagateOperations.setPerformanceWeightOfParseTree(activatedNeuronBaselineCoverage, maxWeight);
								//activatedNeuronBaselineCoverage->parseTreeMaxWeight = maxWeight;
							#endif
								passBaselineComparisonTests = true;
							#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
							}
							#endif
						}

					}
					if(passBaselineComparisonTests)
					{
						#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDINDEX_VERIFICATION
						cout << "verifyActivatedNeuronWithMaxWordIndexCoverage PASSED" << endl;
						#endif
						#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
						cout << "verifyActivatedNeuronWithMaxWordIndexCoverage PASSED" << endl;
						#endif
						result = true;
					}

				}
			#ifdef SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS
			}
			#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
		}
		#endif
		//}
	#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_ENSURE_ALL_NEURONS_IN_SUBGROUP_ARE_NOT_USED_BY_ANY_NEW_CANDIDATE
	}
	#endif
		
	return result;
}
	
bool SANIPropagateCompactClass::verifyActivatedNeuronWithMaxWordIndexAllowed(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIGroupParseTree* activatedNeuronCandidate, const bool strictStartingCondition)
{
	bool result = false;

	int activatedNeuronCandidateMinWordIndex = activatedNeuronCandidate->parseTreeMinWordIndex;
	int activatedNeuronCandidateMaxWordIndex = activatedNeuronCandidate->parseTreeMaxWordIndex;

	bool firstWordIndexTest = false;
	bool lastWordIndexTest = false;

	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		if(strictStartingCondition)
		{
			if(activatedNeuronCandidateMaxWordIndex == forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed)
			{
				firstWordIndexTest = true;
			}
		}
		else
		{
			if(activatedNeuronCandidateMaxWordIndex <= forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed)
			{
				firstWordIndexTest = true;
			}		
		}
		if(activatedNeuronCandidateMinWordIndex >= forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed)
		{
			lastWordIndexTest = true;
		}
	}
	else
	{
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDINDEX_VERIFICATION
		cout << "\tactivatedNeuronCandidateMinWordIndex = " << activatedNeuronCandidateMinWordIndex << endl;
		cout << "\tactivatedNeuronCandidateMaxWordIndex = " << activatedNeuronCandidateMaxWordIndex << endl;
		cout << "\tforwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = " << forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed << endl;
		cout << "\tforwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = " << forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed << endl;
		#endif
		if(strictStartingCondition)
		{
			if(activatedNeuronCandidateMinWordIndex == forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed)
			{
				firstWordIndexTest = true;
			}
		}
		else
		{
			if(activatedNeuronCandidateMinWordIndex >= forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed)
			{
				firstWordIndexTest = true;
			}		
		}
		if(activatedNeuronCandidateMaxWordIndex <= forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed)
		{
			lastWordIndexTest = true;
		}
	}

	if(firstWordIndexTest && lastWordIndexTest)
	{
		result = true;
	}	

	return result;
}
		

bool SANIPropagateCompactClass::verifyActivatedNeuronsAtLeastOne(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverage, SANIGroupNeuralNetwork* newNeuronSequenceGroup1)
{
	bool result = false;
	
	if(verifyActivatedNeuronsAtLeastOneBasic(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage))
	{
		bool pass = true;
		if(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial)
		{			
			#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING
			if(newNeuronSequenceGroup1 == activatedNeuronWithMaxWordIndexCoverage->groupRef) 
			{
				//intrasentenceMatch = true;
				pass = false;
			}
			#endif
		}
		if(pass)
		{
			result = true;
		}
	}
	
	return result;
}

#ifdef SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
bool SANIPropagateCompactClass::verifyNeverSplitGroupBetweenTwoIdenticalComponents(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverage, const bool candidateCoveragePartial)
{
	bool result = false;
	#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
	cout << "verifyNeverSplitGroupBetweenTwoIdenticalComponents START" << endl;
	#endif
	
	if(verifyActivatedNeuronsAtLeastOneBasic(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage))
	{
		bool pass = true;
		#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
		cout << "verifyActivatedNeuronsAtLeastOneBasic" << endl;
		#endif
		
		//condition1: dont select a matched neuron activatedNeuronWithMaxWordIndexCoverage if it stops half way between a repeated section of POS type x in the word sequence
		if(neuronFirstOrLastActivatedComponentWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage))
		{			
			//prevent activatedNeuronWithMaxWordIndexCoverage from being selected by verifyActivatedNeuronWithMaxWordIndexCoverage
			pass = false;
			#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
			cout << "neuronFirstOrLastActivatedComponentWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence"<< endl;		
			#endif
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
			cout << "SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS: condition1: activatedNeuronWithMaxWordIndexCoverage, activatedNeuronWithMaxWordIndexCoveragePartial = " << activatedNeuronWithMaxWordIndexCoverage->activatedNeuronWithMaxWordIndexCoveragePartial << endl;
			//exit(EXIT_ERROR);
			#endif
		}
			
		if(candidateCoveragePartial)
		{			
			//condition2: dont select a matched neuron activatedNeuronWithMaxWordIndexCoverage and split it if the final activated component groupRef (ie of the matched sequence equals) in activatedNeuronWithMaxWordIndexCoverage equals the next unactivated component groupRef in activatedNeuronWithMaxWordIndexCoverage
			int lastActivatedIndex = INT_DEFAULT_VALUE;
			int firstUnactivatedIndex = INT_DEFAULT_VALUE;
			bool partialActivationConfirmed = identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage, &lastActivatedIndex, &firstUnactivatedIndex);
			
			if(partialActivationConfirmed)	//OLD: partial activation test required in case where activatedNeuronWithMaxWordIndexCoverage is recorded by SANIPropagateCompactClass::propagateWordThroughNetworkGroupComponentWrapper but is subsequently fully activated
			{	
				if(lastActivatedIndex == firstUnactivatedIndex)
				{
					cout << "(lastActivatedIndex == firstUnactivatedIndex)" << endl;
					exit(EXIT_ERROR);
				}
				SANIComponentNeuralNetwork* lastActivatedComponent = (activatedNeuronWithMaxWordIndexCoverage->groupRef)->components[lastActivatedIndex];		//BAD: activatedNeuronWithMaxWordIndexCoverage->components[lastActivatedIndex]->componentRef;
				SANIComponentNeuralNetwork* firstNonActivatedComponent = (activatedNeuronWithMaxWordIndexCoverage->groupRef)->components[firstUnactivatedIndex];	//BAD: activatedNeuronWithMaxWordIndexCoverage->components[firstUnactivatedIndex]->componentRef;
				#ifdef SANI_SEQUENCE_GRAMMAR
				//assume lastActivatedComponent/firstNonActivatedComponent->groupRef == null, so cannot be compared
				//assume !SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS (ie ANNbackGroupConnectionList contains single neurons) 
				SANIGroupNeuralNetwork* lastActivatedComponentSource = lastActivatedComponent->ANNbackGroupConnectionList[0];
				SANIGroupNeuralNetwork* firstNonActivatedComponentSource = firstNonActivatedComponent->ANNbackGroupConnectionList[0];
				int lastActivatedComponentSourceSize = lastActivatedComponent->ANNbackGroupConnectionList.size();
				int firstNonActivatedComponentSourceSize = firstNonActivatedComponent->ANNbackGroupConnectionList.size();
				if(((lastActivatedComponentSourceSize == 1) && (firstNonActivatedComponentSourceSize == 1)) && (lastActivatedComponentSource == firstNonActivatedComponentSource))
				#else
				if(lastActivatedComponent->groupRef == firstNonActivatedComponent->groupRef)
				#endif
				{
					//cout << "(lastActivatedComponent->groupRef == firstNonActivatedComponent->groupRef)" << endl;
					
					//cout << "lastActivatedComponent->groupRef->groupIndex = " << lastActivatedComponent->groupRef->groupIndex;
					//cout << "firstNonActivatedComponent->groupRef->groupIndex = " << firstNonActivatedComponent->groupRef->groupIndex;
					
					//prevent activatedNeuronWithMaxWordIndexCoverage from being selected by verifyActivatedNeuronWithMaxWordIndexCoverage
					pass = false;	
					#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
					cout << "SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS: condition2" << endl;
					#endif
				}
			}
		}
		
		if(pass)
		{
			result = true;
		}
	}
	
	return result;
}

bool SANIPropagateCompactClass::neuronFirstOrLastActivatedComponentWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverage)
{
	bool result = false;
	
	//condition1: dont select a matched neuron activatedNeuronWithMaxWordIndexCoverage if it stops half way between a repeated section of POS type x in the word sequence
	int currentMaxWordIndexInSequence;
	int nextWordIndexInSequence;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		currentMaxWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex;
		nextWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex - 1;
	}
	else
	{
		currentMaxWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex;
		nextWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex + 1;
	}
	int currentMinWordIndexInSequence;
	int previousWordIndexInSequence;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		currentMinWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex;
		previousWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex + 1;
	}
	else
	{
		currentMinWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex;
		previousWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex - 1;
	}
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
	cout << "\n\n activatedNeuronWithMaxWordIndexCoverage->components.size() = " << activatedNeuronWithMaxWordIndexCoverage->components.size() << endl;	
	cout << "currentMaxWordIndexInSequence = " << currentMaxWordIndexInSequence << endl;
	cout << "nextWordIndexInSequence = " << nextWordIndexInSequence << endl;
	cout << "currentMinWordIndexInSequence = " << currentMinWordIndexInSequence << endl;
	cout << "previousWordIndexInSequence = " << previousWordIndexInSequence << endl;
	#endif
	
	if(((currentMaxWordIndexInSequence < forwardPropogationSentenceData->firstLayer->size()) && (currentMaxWordIndexInSequence >= 0)) && ((nextWordIndexInSequence < forwardPropogationSentenceData->firstLayer->size()) && (nextWordIndexInSequence >= 0)))
	{//boundary test
		#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		vector<SANIGroupNeuralNetwork*>* currentMaxWordIndexInSequenceFirstLayerNeurons = (*forwardPropogationSentenceData->firstLayer)[currentMaxWordIndexInSequence];
		vector<SANIGroupNeuralNetwork*>* nextWordIndexInSequenceFirstLayerNeurons = (*forwardPropogationSentenceData->firstLayer)[nextWordIndexInSequence];			
		for(int i=0; i<currentMaxWordIndexInSequenceFirstLayerNeurons.size(); i++)
		{
			SANIGroupNeuralNetwork* currentMinWordIndexInSequenceFirstLayerNeuron = (*currentMinWordIndexInSequenceFirstLayerNeurons)[i];
			SANIGroupNeuralNetwork* previousWordIndexInSequenceFirstLayerNeuron = (*previousWordIndexInSequenceFirstLayerNeurons)[i];		
			cerr << "neuronFirstOrLastActivatedComponentWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence hasnt yet been coded for SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY" << endl;
			exit(EXIT_ERROR);
		#else
			SANIGroupNeuralNetwork* currentMaxWordIndexInSequenceFirstLayerNeuron = (*forwardPropogationSentenceData->firstLayer)[currentMaxWordIndexInSequence];
			SANIGroupNeuralNetwork* nextWordIndexInSequenceFirstLayerNeuron = (*forwardPropogationSentenceData->firstLayer)[nextWordIndexInSequence];	
		#endif
			if(currentMaxWordIndexInSequenceFirstLayerNeuron == nextWordIndexInSequenceFirstLayerNeuron)
			{
				result = true;
			}
		#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		}
		#endif
	}
	if(((currentMinWordIndexInSequence < forwardPropogationSentenceData->firstLayer->size()) && (currentMinWordIndexInSequence >= 0)) && ((previousWordIndexInSequence < forwardPropogationSentenceData->firstLayer->size()) && (previousWordIndexInSequence >= 0)))
	{//boundary test
		#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		vector<SANIGroupNeuralNetwork*>* currentMinWordIndexInSequenceFirstLayerNeurons = (*forwardPropogationSentenceData->firstLayer)[currentMinWordIndexInSequence];
		vector<SANIGroupNeuralNetwork*>* previousWordIndexInSequenceFirstLayerNeurons = (*forwardPropogationSentenceData->firstLayer)[previousWordIndexInSequence];
		for(int i=0; i<currentMaxWordIndexInSequenceFirstLayerNeurons.size(); i++)
		{
			SANIGroupNeuralNetwork* currentMinWordIndexInSequenceFirstLayerNeuron = (*currentMinWordIndexInSequenceFirstLayerNeurons)[i];
			SANIGroupNeuralNetwork* previousWordIndexInSequenceFirstLayerNeuron = (*previousWordIndexInSequenceFirstLayerNeurons)[i];			
			cerr << "neuronFirstOrLastActivatedComponentWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence hasnt yet been coded for SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY" << endl;
			exit(EXIT_ERROR);		
		#else		
		SANIGroupNeuralNetwork* currentMinWordIndexInSequenceFirstLayerNeuron = (*forwardPropogationSentenceData->firstLayer)[currentMinWordIndexInSequence];
		SANIGroupNeuralNetwork* previousWordIndexInSequenceFirstLayerNeuron = (*forwardPropogationSentenceData->firstLayer)[previousWordIndexInSequence];	
		#endif
			if(currentMinWordIndexInSequenceFirstLayerNeuron == previousWordIndexInSequenceFirstLayerNeuron)
			{
				result = true;
			}
		#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		}
		#endif
	}
	
	return result;
}
#endif

bool SANIPropagateCompactClass::verifyActivatedNeuronsAtLeastOneBasic(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverage)
{
	bool result = false;
	
	if(activatedNeuronWithMaxWordIndexCoverage != NULL)
	{
		result = true;
	}
	
	return result;
}



bool SANIPropagateCompactClass::identifyComponentIndexLastActivatedIndex(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex)
{
	bool result = false;
	int numberOfActivatedComponents = parseTreeGroupNeuron->components.size();
	SANIGroupNeuralNetwork* groupOrig = parseTreeGroupNeuron->groupRef;
	if(parseTreeGroupNeuron->components.size() > 0)
	{
		result = true;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			*lastActivatedIndex = groupOrig->components.size()-numberOfActivatedComponents;
		}
		else
		{
			*lastActivatedIndex = numberOfActivatedComponents-1;
		}
	}
	return result;
}

#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
//CHECKTHIS;
//ordered (do not assume normal order): "first" and "last" definitions respect (forwardPropogationSentenceData->parseSentenceReverse)
//partialActivationConfirmed: tests whether there is at least 1 unactivated component succeeding the activated component (not for SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION case where missingStartComponentFound, but all remaining components are active)
bool SANIPropagateCompactClass::identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex, int* firstUnactivatedIndex)
{
	bool partialActivationConfirmed = false;
	
	int firstActivatedIndexUnordered;
	int lastActivatedIndexUnordered;
	if(identifyComponentIndexFirstAndLastActivatedIndexUnordered(forwardPropogationSentenceData, parseTreeGroupNeuron, &firstActivatedIndexUnordered, &lastActivatedIndexUnordered))
	{
		SANIGroupNeuralNetwork* groupOrig = parseTreeGroupNeuron->groupRef;

		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			*lastActivatedIndex = firstActivatedIndexUnordered;
			*firstUnactivatedIndex = firstActivatedIndexUnordered-1;			
			if(*firstUnactivatedIndex >= 0)
			{
				partialActivationConfirmed = true;
			}
		}
		else
		{
			*lastActivatedIndex = lastActivatedIndexUnordered;
			*firstUnactivatedIndex = lastActivatedIndexUnordered+1;
			if(*firstUnactivatedIndex < groupOrig->components.size())
			{
				partialActivationConfirmed = true;
			}
		}
	}
	
	return partialActivationConfirmed;
}

//unordered (assume normal order): "first" and "last" definitions do not respect (forwardPropogationSentenceData->parseSentenceReverse)
bool SANIPropagateCompactClass::identifyComponentIndexFirstAndLastActivatedIndexUnordered(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* firstActivatedIndex, int* lastActivatedIndex)
{
	bool result = false;
	
	if(parseTreeGroupNeuron->components.size() > 0)
	{
		int numberOfActivatedComponents = parseTreeGroupNeuron->components.size();
		SANIComponentNeuralNetwork* firstActivatedComponent = parseTreeGroupNeuron->components[0]->componentRef;
		*firstActivatedIndex = firstActivatedComponent->componentIndex;
		*lastActivatedIndex = *firstActivatedIndex + numberOfActivatedComponents-1;
		
		result = true;
	}
	
	return result;
}
#else

//preconditions: note no components->size() boundary check required as identifyComponentIndexFirstUnactivatedIndex can only be executed on partiallyActivatedNeuronWithMaxWordIndexCoverage (never fullyActivatedNeuronWithMaxWordIndexCoverage)
//partialActivationConfirmed: tests whether there is at least 1 unactivated component succeeding the activated component (not for SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION case where missingStartComponentFound, but all remaining components are active)
bool SANIPropagateCompactClass::identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex, int* firstUnactivatedIndex)
{
	bool partialActivationConfirmed = false;
	
	int numberOfActivatedComponents = parseTreeGroupNeuron->components.size();
	SANIGroupNeuralNetwork* groupOrig = parseTreeGroupNeuron->groupRef;
	if(parseTreeGroupNeuron->components.size() < groupOrig->components.size())
	{
		partialActivationConfirmed = true;
		
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			*lastActivatedIndex = groupOrig->components.size()-numberOfActivatedComponents;
			*firstUnactivatedIndex = groupOrig->components.size()-1-numberOfActivatedComponents;
		}
		else
		{
			*lastActivatedIndex = numberOfActivatedComponents-1;
			*firstUnactivatedIndex = numberOfActivatedComponents;
		}
	}
	
	return partialActivationConfirmed;
}


//unordered (assume normal order): "first" and "last" definitions do not respect (forwardPropogationSentenceData->parseSentenceReverse)
bool SANIPropagateCompactClass::identifyComponentIndexLastActivatedIndexUnordered(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex)
{
	bool result = false;
	if(parseTreeGroupNeuron->components.size() > 0)
	{
		int numberOfActivatedComponents = parseTreeGroupNeuron->components.size();
		SANIComponentNeuralNetwork* firstActivatedComponent = parseTreeGroupNeuron->components[0]->componentRef;
		int firstActivatedIndex = firstActivatedComponent->componentIndex;
		if(firstActivatedIndex == 0)
		{
			result = true;
			*lastActivatedIndex = *firstActivatedIndex + numberOfActivatedComponents-1;
		}
		else
		{
			cerr << "!SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION: SANIPropagateCompactClass::identifyComponentIndexLastActivatedIndexUnordered error:  (firstActivatedIndex != 0)" << endl;
			exit(EXIT_ERROR);
		}
	}
	return result;
}
#endif



#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP
bool SANIPropagateCompactClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponentWrapper(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIComponentNeuralNetwork* component, SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const bool existingActivationFoundStartComponent)
{
	bool result = false;
	
	SANIGroupParseTree* ownerGroupParseTreeGroup = ownerGroup->currentParseTreeGroupTemp;
	if((ownerGroupParseTreeGroup->components.size() == 0) || existingActivationFoundStartComponent)
	{		
		int firstWordIndexInSequence;
		//expect to find upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent if current group->parseTreeMinWordIndex/parseTreeMaxWordIndex doesnt encapsulate firstWordIndexInSequence

		int wordIndexMax = -1;
		int wordIndexMin = REALLY_LARGE_INT;
		SANIGroupParseTree* parseTreeGroupToFindWordIndexMin;
		SANIGroupParseTree* parseTreeGroupToFindWordIndexMax;
		int wordIndexMinMin;
		int wordIndexMaxMax;
		int wordIndexMinToFindAbove;		
		int wordIndexMaxToFindAbove;
		int wordIndexLastAboveEffectiveAfterGroupReset = INT_DEFAULT_VALUE;

		bool execute = false;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			cout << "\nSANIPropagateCompactClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponentWrapper start:" << endl;
			#endif
			
			#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
			firstWordIndexInSequence = forwardPropogationSentenceData->sentenceContents->size()-1-forwardPropogationSignalData->firstIndexInSequence;
			#else
			firstWordIndexInSequence = forwardPropogationSentenceData->sentenceContents->size()-1;
			#endif

			parseTreeGroupToFindWordIndexMax = activationPathWordCurrentParseTreeGroup;
			wordIndexMaxMax = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;	//forwardPropogationWordData->w;
			if(parseTreeGroupToFindWordIndexMax != NULL)
			{
				if(!SANIPropagateOperations.getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax, true, &wordIndexMax, 0))
				{

				}
			}
			
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			cout << "wordIndexMaxMax = " << wordIndexMaxMax << endl;
			cout << "wordIndexMax = " << wordIndexMax << endl;
			#endif
			
			wordIndexMax = SHAREDvars.maxInt(wordIndexMax, wordIndexMaxMax);
			
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			cout << "wordIndexMax (maxInt) = " << wordIndexMax << endl;
			#endif
			
			if(wordIndexMax < firstWordIndexInSequence)
			{
				wordIndexMaxToFindAbove = firstWordIndexInSequence;
				wordIndexMinToFindAbove = wordIndexMax+1;
			
				//need to disregard wordIndexMin/Max of last activated component in uppergroups (assuming it was effectively activated by current component)
				if(existingActivationFoundStartComponent)	//ie ownerGroupParseTreeGroup != NULL
				{
					wordIndexLastAboveEffectiveAfterGroupReset = ownerGroupParseTreeGroup->parseTreeMaxWordIndex+1;//CHECKTHIS; ??
				}
				
				execute = true;
			}
		}
		else
		{
			#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
			firstWordIndexInSequence = forwardPropogationSignalData->firstIndexInSequence;
			#else
			firstWordIndexInSequence = 0;
			#endif

			parseTreeGroupToFindWordIndexMin = activationPathWordCurrentParseTreeGroup;
			wordIndexMinMin = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;	//forwardPropogationWordData->w;
			if(parseTreeGroupToFindWordIndexMin != NULL)
			{
				if(!SANIPropagateOperations.getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMin, false, &wordIndexMin, 0))
				{

				}
			}
			wordIndexMin = SHAREDvars.minInt(wordIndexMin, wordIndexMinMin);
			if(wordIndexMin > firstWordIndexInSequence)
			{
				wordIndexMinToFindAbove = firstWordIndexInSequence;
				wordIndexMaxToFindAbove = wordIndexMin-1;
			
				//need to disregard wordIndexMin/Max of last activated component in uppergroups (assuming it was effectively activated by current component)
				if(existingActivationFoundStartComponent)	//ie ownerGroupParseTreeGroup != NULL
				{
					wordIndexLastAboveEffectiveAfterGroupReset = ownerGroupParseTreeGroup->parseTreeMinWordIndex-1;	
				}
				
				execute = true;
			}
		}	
		
		if(execute)
		{
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			cout << "wordIndexMaxToFindAbove = " << wordIndexMaxToFindAbove << endl;
			cout << "wordIndexMinToFindAbove = " << wordIndexMinToFindAbove << endl;
			cout << "wordIndexLastAboveEffectiveAfterGroupReset = " << wordIndexLastAboveEffectiveAfterGroupReset << endl;
			#endif
				
			if(upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent(forwardPropogationSentenceData, wordIndexMinToFindAbove, wordIndexMaxToFindAbove, wordIndexLastAboveEffectiveAfterGroupReset, 0, ownerGroup, false, false))
			{
				result = true;
			}
			else
			{
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
				cout << "!upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent" << endl;
				#endif
			}
		}
		else
		{
			result = true;
		}
	}
	else
	{
		result = true;
	}
						
	return result;
}

//bool SANIPropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool findMaxOrMinWordIndex, const int wordIndexMaxOrMinToFind, int level, SANIGroupNeuralNetwork* group)
bool SANIPropagateCompactClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const int wordIndexMinToFindAbove, const int wordIndexMaxToFindAbove, const int wordIndexLastAboveEffectiveAfterGroupReset, int level, SANIGroupNeuralNetwork* group, bool wordIndexMinFound, bool wordIndexMaxFound)
{
	bool result = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
	bool keepExecuting = true;
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
	wordIndexMinFound = false;
	wordIndexMaxFound = false;
	#endif
	
	for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
		if(keepExecuting)
		{
			#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED2
			bool keepExecutingLocal = true;
			#endif
		#endif
			
			SANIComponentNeuralNetwork* currentComponent = (group->ANNfrontComponentConnectionList)[i];
			SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;

			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			SANIPropagateOperations.printParseTreeDebugIndentation(level);
			cout << "ownerGroup->groupIndex = " << ownerGroup->groupIndex << endl;
			#endif

			SANIGroupParseTree* ownerGroupParseTree = ownerGroup->currentParseTreeGroupTemp;
			int parseTreeMinWordIndexEffectiveAfterGroupReset = ownerGroupParseTree->parseTreeMinWordIndex;
			int parseTreeMaxWordIndexEffectiveAfterGroupReset = ownerGroupParseTree->parseTreeMaxWordIndex;

			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			SANIPropagateOperations.printParseTreeDebugIndentation(level);
			cout << "parseTreeMinWordIndex = " << parseTreeMinWordIndexEffectiveAfterGroupReset << endl;
			SANIPropagateOperations.printParseTreeDebugIndentation(level);
			cout << "parseTreeMaxWordIndex = " << parseTreeMaxWordIndexEffectiveAfterGroupReset << endl;
			#endif


			#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
			bool groupHasPreceedingComponent = false;
			if(forwardPropogationSentenceData->parseSentenceReverse)
			{
				if(currentComponent->componentIndex < ownerGroup->components.size()-1)
				{
					groupHasPreceedingComponent = true;
				}
			}
			else
			{
				 if(currentComponent->componentIndex > 0)
				 {
			 		groupHasPreceedingComponent = true;
				 }
			}
			#endif

			if(currentComponent->neuronComponentConnectionActive)
			{//currentComponent was activated by the group being reset - so determine the ownerGroup's expected [older] parseTreeMinWordIndex/parseTreeMaxWordIndex values assuming the group is reset
				if(wordIndexLastAboveEffectiveAfterGroupReset != INT_DEFAULT_VALUE)
				{
					if(forwardPropogationSentenceData->parseSentenceReverse)
					{
						if(parseTreeMinWordIndexEffectiveAfterGroupReset < wordIndexLastAboveEffectiveAfterGroupReset)
						{
							parseTreeMinWordIndexEffectiveAfterGroupReset = wordIndexLastAboveEffectiveAfterGroupReset;
						}
					}
					else
					{
						if(parseTreeMaxWordIndexEffectiveAfterGroupReset > wordIndexLastAboveEffectiveAfterGroupReset)
						{
							parseTreeMaxWordIndexEffectiveAfterGroupReset = wordIndexLastAboveEffectiveAfterGroupReset;
						}
					}
				}
			}

			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			SANIPropagateOperations.printParseTreeDebugIndentation(level);
			cout << "parseTreeMinWordIndexEffectiveAfterGroupReset = " << parseTreeMinWordIndexEffectiveAfterGroupReset << endl;
			SANIPropagateOperations.printParseTreeDebugIndentation(level);
			cout << "parseTreeMaxWordIndexEffectiveAfterGroupReset = " << parseTreeMaxWordIndexEffectiveAfterGroupReset << endl;
			#endif

			if(forwardPropogationSentenceData->parseSentenceReverse)
			{
				//assume will find wordIndexMin match before wordIndexMax match when propagating up network - CHECKTHIS
				#ifndef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
				if(!wordIndexMinFound)
				{
				#endif
					if(parseTreeMinWordIndexEffectiveAfterGroupReset == wordIndexMinToFindAbove)
					{
						wordIndexMinFound = true;
						#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "SANIPropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - wordIndexMinFound" << endl;
						#endif
					}
					#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED2
					else
					{
						if(groupHasPreceedingComponent)
						{
							keepExecutingLocal = false;
						}
					}
					#endif
				#ifndef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
				}
				if(wordIndexMinFound)
				{
				#endif
					if(parseTreeMaxWordIndexEffectiveAfterGroupReset == wordIndexMaxToFindAbove)
					{
						wordIndexMaxFound = true;
						#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "SANIPropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - wordIndexMaxFound" << endl;
						#endif
					}
				#ifndef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
				}
				#endif
			}
			else
			{
				//assume will find wordIndexMax match before wordIndexMin match when propagating up network - CHECKTHIS
				#ifndef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
				if(!wordIndexMaxFound)
				{
				#endif
					if(parseTreeMaxWordIndexEffectiveAfterGroupReset == wordIndexMaxToFindAbove)
					{
						wordIndexMaxFound = true;
						#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "SANIPropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - wordIndexMaxFound" << endl;
						#endif
					}
					#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED2
					else
					{
						if(groupHasPreceedingComponent)
						{
							keepExecutingLocal = false;
						}
					}
					#endif
				#ifndef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST					
				}
				if(wordIndexMaxFound)
				{
				#endif
					if(parseTreeMinWordIndexEffectiveAfterGroupReset == wordIndexMinToFindAbove)
					{
						wordIndexMinFound = true;
						#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "SANIPropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - wordIndexMinFound" << endl;
						#endif
					}
				#ifndef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST					
				}
				#endif
			}


			if(wordIndexMinFound && wordIndexMaxFound)
			{
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
				cout << "SANIPropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - (wordIndexMinFound && wordIndexMaxFound); result == true" << endl;
				#endif
				result = true;
				#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
				keepExecuting = false;
				#endif
			}
			else
			{
				#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED2
				if(keepExecutingLocal)
				{
				#endif
					if(upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent(forwardPropogationSentenceData, wordIndexMinToFindAbove, wordIndexMaxToFindAbove, wordIndexLastAboveEffectiveAfterGroupReset, level+1, ownerGroup, wordIndexMinFound, wordIndexMaxFound))
					{
						result = true;
					}
				#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED2
				}
				#endif
			}
		#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
		}
		#endif
	}
	
	return result;
}

#endif


#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
bool SANIPropagateCompactClass::printSANInetworkSVG(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes)
{
	bool result = true;
	
	string outputTextSVGFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + "IncrementalGeneration" + SHAREDvars.convertIntToString(incrementalGenerationIndex) + NEURAL_NETWORK_VISUALISATION_SVG_FILE_EXTENSION;
	cout << "SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK: outputTextSVGFileName = " << outputTextSVGFileName << endl;
	
	#ifdef SANI_ANN_DELAY_ANN_CONNECTIVITY_TILL_END
	SANIFormation.createANNconnectivity(SANIGroupTypes);
	#endif
	
	GIAneuralNetworkOperationsClass().generateNeuralNetFromGIAposRelTranslatorNet(translatorVariables);	//generate GIA NLP neural network

	int rasterImageWidth = 0;
	int rasterImageHeight = 0;
	bool ANNdisplayInOpenGL = false;
	bool ANNuseInputXMLFile = false;
	string ANNinputXMLFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_XML_FILE_EXTENSION;
	bool ANNuseOutputXMLFile = false;
	string ANNoutputXMLFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_XML_FILE_EXTENSION;
	bool ANNuseOutputLDRFile = false;
	string ANNoutputLDRFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_LDR_FILE_EXTENSION;
	bool ANNuseOutputSVGFile = true;
	string ANNoutputSVGFileName = outputTextSVGFileName;
	bool ANNuseOutputPPMFile = false;
	string ANNoutputPPMFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_PPM_FILE_EXTENSION;
	bool ANNuseOutputPPMFileRaytraced = false;
	string ANNoutputPPMFileNameRaytraced = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_PPM_RAYTRACED_FILE_EXTENSION;
	string ANNoutputTALFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_TAL_FILE_EXTENSION;
	bool ANNuseSprites = true;
	
	//small:
	int widthSVG = 3840;	//1920
	int heightSVG = 2160;	//1080	//1400
	
	//large/medium:
	//int widthSVG = 14400;
	//int heightSVG = 2430;
	
	ANNdisplay.outputNeuralNetworkToVectorGraphicsAndRaytrace(translatorVariables->firstInputNeuronInNetwork, ANNuseSprites, ANNuseOutputPPMFileRaytraced, ANNdisplayInOpenGL, ANNuseOutputLDRFile, ANNuseOutputSVGFile, ANNuseOutputPPMFile, ANNoutputLDRFileName, ANNoutputSVGFileName, ANNoutputPPMFileName, ANNoutputPPMFileNameRaytraced, ANNoutputTALFileName, rasterImageWidth, rasterImageHeight, widthSVG, heightSVG);

	#ifdef SANI_ANN_DELAY_ANN_CONNECTIVITY_TILL_END
	SANIFormation.createANNconnectivityReset(SANIGroupTypes);
	#endif
	
	incrementalGenerationIndex++;
	
	return result;
}
#endif


int SANIPropagateCompactClass::calculateNextIndexInSequenceProspective(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverageProspective)
{
	int indexInSequence = 0;
	
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		indexInSequence = forwardPropogationSentenceData->sentenceContents->size()-1 - (activatedNeuronWithMaxWordIndexCoverageProspective->parseTreeMinWordIndex - 1);
	}
	else
	{
		indexInSequence = activatedNeuronWithMaxWordIndexCoverageProspective->parseTreeMaxWordIndex + 1;
	}
	
	return indexInSequence;
}



#endif