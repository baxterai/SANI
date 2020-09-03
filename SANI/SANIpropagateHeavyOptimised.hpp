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
 * File Name: SANIpropagateHeavyOptimised.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1m5c 01-September-2020
 * Requirements: 
 * Description: Propagate Heavy Optimised - ~O(nlogn)
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_PROPAGATE_HEAVY_OPTIMISED
#define HEADER_SANI_PROPAGATE_HEAVY_OPTIMISED

#include "GIAglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#include "SANInodes.hpp"
#include "SANInodesGroupClass.hpp"
#include "SANInodesComponentClass.hpp"
#include "SANIpropagateOperations.hpp"
#include "SANIformation.hpp"
#ifdef SANI_PARSE
#include "GIAposRelTranslatorParserOperations.hpp"
#endif
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorWordClass.hpp"
#include "GIApreprocessorPOStagger.hpp"
#include "ANNdisplay.hpp"



#ifdef SANI_HEAVY
#ifdef SANI_HEAVY_OPTIMISED

class SANIpropagateHeavyOptimisedClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIApreprocessorPOStaggerClass GIApreprocessorPOStagger;
	private: GIApreprocessorSentenceClass GIApreprocessorSentenceClassObject;
	private: SANInodesClass SANInodes;
	private: SANInodesGroupClass SANInodesGroupClassObject;
	private: SANInodesComponentClass SANInodesComponentClassObject;
	private: SANIpropagateOperationsClass SANIpropagateOperations;
	private: SANIformationClass SANIformation;
	#ifdef SANI_PARSE
	private: GIAposRelTranslatorParserOperationsClass GIAposRelTranslatorParserOperations;
	#endif
	private: ANNdisplayClass ANNdisplay;

	public: bool executePosRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance);
		private: bool propagateWordThroughNetworkWordGroupIntro(GIAtranslatorVariablesClass* translatorVariables, const int w, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
			private: bool propagateWordThroughNetworkWordGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
				private: bool propagateWordThroughNetworkWordGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, SANIGroupNeuralNetwork* inputLayerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
		private: bool propagateWordThroughNetworkGroupIntro(GIAtranslatorVariablesClass* translatorVariables, SANIForwardPropogationSentenceData* forwardPropogationSentenceData);
			private: bool connectToPreviousActivationGroup(GIAtranslatorVariablesClass* translatorVariables, SANIGroupActivationMemory* activationPathWordFirstActivationMemoryGroup, SANIGroupActivationMemory* activationPathWordFirstActivationMemoryGroupNext, SANIForwardPropogationActivationPointData* forwardPropogationActivationPointData, SANIForwardPropogationActivationPointData* forwardPropogationActivationPointDataNext, SANIForwardPropogationSignalData* forwardPropogationSignalDataNext, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationWordData* forwardPropogationWordDataNext, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);		
			private: bool propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup);
				private: bool propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* ownerGroup, SANIGroupNeuralNetwork* ownerGroupOrig, SANIComponentNeuralNetwork* currentComponent, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner);
					private: bool propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIGroupNeuralNetwork* ownerGroupOrig, SANIComponentNeuralNetwork* ownerComponent, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, GIAposRelTranslatorDebug* debug);	
						private: bool propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIGroupNeuralNetwork* ownerGroupOrig, SANIComponentNeuralNetwork* ownerComponent, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, const bool repeatDetected, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, GIAposRelTranslatorDebug* debug);
			
	private: bool printBackpropParseTree(SANIGroupParseTree* group, int level);

	private: void generateMemoryActiveGroup(SANIGroupNeuralNetwork* activationMemoryGroup, SANIGroupNeuralNetwork* groupOrig, SANIGroupNeuralNetwork* currentParseTreeGroupTemplate, SANIGroupActivationMemory** activationMemoryGroupNew, SANIGroupParseTree** currentParseTreeGroupNew, const bool copyParseTreeGroupComponents);
	private: void generateMemoryActiveGroup(SANIGroupNeuralNetwork* activationMemoryGroup, SANIGroupNeuralNetwork* groupOrig, SANIGroupParseTree* currentParseTreeGroup, SANIGroupActivationMemory** activationMemoryGroupNew, SANIGroupParseTree** currentParseTreeGroupNew, const bool copyParseTreeGroupComponents);

	private: void printComponents(vector<SANIComponentNeuralNetwork*>* components);

};

#endif
#endif

#endif