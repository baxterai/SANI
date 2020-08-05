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
 * File Name: SANIComponentClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1m3a 01-August-2020
 * Requirements: requires plain text file
 * Description: SANI (Sequentially Activated Neuronal Input neural network) Component Class
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_COMPONENT_CLASS
#define HEADER_SANI_COMPONENT_CLASS

#include "GIAglobalDefs.hpp"
#include "GIApreprocessorWordClass.hpp"
#ifdef SANI
#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#endif

#ifdef GIA_POS_REL_TRANSLATOR_RULES

#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_UNKNOWN (0)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS (1)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT (2)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS (3)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_NUMBER_OF_TYPES (4)
static string SANIGroupsComponentStringTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_NUMBER_OF_TYPES] = {"unknown", "LRPexternalWordLists", "explicit", "tokens"};


#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_REFERENCE_SET_TYPE_HYBRID_UNKNOWN (0)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_REFERENCE_SET_TYPE_HYBRID_LOGICSET (1)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_REFERENCE_SET_TYPE_HYBRID_LOGICSETDELIMITER (2)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_REFERENCE_SET_TYPE_HYBRID_REFERENCESET (3)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_REFERENCE_SET_TYPE_HYBRID_REFERENCESETDELIMITER (4)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_REFERENCE_SET_TYPE_HYBRID_SUBREFERENCESET (5)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_REFERENCE_SET_TYPE_HYBRID_SUBREFERENCESETDELIMITER (6)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_REFERENCE_SET_TYPE_HYBRID_NUMBER_OF_TYPES (7)
static string SANIGroupsComponentReferenceSetTypeHybridTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_REFERENCE_SET_TYPE_HYBRID_NUMBER_OF_TYPES] = {"unknown", "logicReferenceSet", "logicReferenceSetDelimiter", "referenceSet", "referenceSetDelimiter", "subReferenceSet", "subReferenceSetDelimiter"};
#endif

#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_UNKNOWN (0)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING (1)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP (2)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR (3)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT (4)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_NUMBER_OF_TYPES (5)
static string SANIGroupsComponentTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_NUMBER_OF_TYPES] = {"unknown", "string", "group", "or", "repeat"};

#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_UNKNOWN (0)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_LIST (1)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_SUBJECT (2)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_OBJECT (3)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_DELIMITER (4)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_DELIMITER_OR_SUBJECT (5)	//artificial GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RETURN_FUNCTION_NAME_DYNAMIC_INDEX_TESTS	//detect nearest preceeeding determiner and if not find nearest preceeding subject
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_NUMBER_OF_TYPES (6)
static string SANIGroupsComponentSemanticRelationIndexTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_NUMBER_OF_TYPES] = {"unknown", "list", "subject", "object", "delimiter", "delimiterOrSubject"};


class SANIGroupType;
class SANIGroupNeuralNetwork;
class SANIGroupParseTree;
#ifdef SANI_PARSE_SIMULTANEOUS_BIO
class GIAposRelTranslatorParserForwardPropogationSignalData;
#endif

class SANIComponent
{
public:
	SANIComponent(void);
	~SANIComponent(void);

	#ifdef SANI_SEQUENCE_GRAMMAR
	vector<SANIGroupNeuralNetwork*> ANNbackGroupConnectionList;
	#ifdef SANI_SEQUENCE_GRAMMAR_WEIGHTS
	double componentStrength;
	#endif
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
	int referenceSetTypeHybrid;
	#endif
	
	int componentType;
	int componentIndex;	//component index in group - required for shortcutting between base component and parseTreeGroup component
	
	string groupTypeRefName;
	string groupRefName;
	
	int stringType;	//explicit, tokens, LRPexternalWordLists
	string word;	//explicit
	string wordPOStype;	//LRPexternalWordLists
	string tokenLayer;	//tokens
	string tokenClass;	//tokens
	string tokenType;	//tokens
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_MISSING
	bool missing;	//special case used to specify that the string component is expected to be missing (at the designated position)
	#endif
	#ifdef SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS_EFFICIENT
	bool stringTypeExplicitAddToExplicitWordTempPOS;
	#endif
	
	vector<int> semanticRelationIndexType;
	bool semanticRelationReturnEntity;
	#ifdef SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
	bool semanticRelationReturnConnectionDynamic;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RETURN_FUNCTION_NAME
	string semanticRelationReturnFunctionName;		//must be the same for every component in the group
	int semanticRelationReturnFunctionNameIndexType;	//must be the same for every component in the group
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RECORD_AND_CONNECT
	bool semanticRelationRecord;
	bool semanticRelationConnect;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_OPTIONAL
	bool optional;	//the component is optional
	#endif

	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_QUERY
	bool queryComparisonVariable;
	bool isAuxiliaryQuery;
	bool isWhichOrEquivalentWhatQuery;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_ALIASES
	bool isNameQuery;
	bool isName;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NUMBER_OF
	bool isNumberOf;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	int wordVerbVariantType;
	int wordNounVariantType;
	#endif		
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NUMBER_EXPLETIVES
	bool isExpletive;
	#endif	
	
	#ifdef SANI
	bool neuronComponentConnectionActive;
	GIApreprocessorPlainTextWord* neuronComponentConnectionActiveWordRecord;
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
	vector<GIApreprocessorPlainTextWord*> neuronComponentConnectionActiveWordRecordMemory;
	#endif
	#endif
	GIApreprocessorPlainTextWord* candidateStringMatch;
	SANIGroupType* groupTypeRef;	
	SANIGroupNeuralNetwork* groupRef;
	
	#ifdef SANI_PARSE_SIMULTANEOUS_BIO
	GIAposRelTranslatorParserForwardPropogationSignalData* parserForwardPropogationSignalData;	//semantic relation function parameters of parseTreeGroupRef->semanticRelationReturnEntityForwardPropogationSignalData
	#endif

};


class SANIComponentNeuralNetwork: public SANIComponent
{
public:
	SANIComponentNeuralNetwork(void);
	~SANIComponentNeuralNetwork(void);
	
	
	vector<SANIComponentNeuralNetwork*> subComponents;	//for componentType or/repeat only
	
	#ifdef SANI
	SANIGroupNeuralNetwork* ownerGroup;
	bool isSubcomponent;
	SANIComponentNeuralNetwork* ownerComponent;	//if isSubComponent==true only
	#ifdef SANI_ANN
	vector<ANNneuronConnection*> ANNbackNeuronConnectionList;
	#endif
	#ifdef SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
	vector<int> optimumPathwayWordList;
	#endif
	#endif
};


class SANIComponentParseTree: public SANIComponent
{
public:
	SANIComponentParseTree(void);
	~SANIComponentParseTree(void);

	//parse tree variables:
	#ifdef SANI_PARSE_GENERATE_PARSE_TREE
	SANIGroupParseTree* parseTreeGroupRef;
	#endif
	SANIComponentNeuralNetwork* componentRef;	//backup of original component (non-parse-tree)
	int wordPOStypeInferred;
	#ifdef SANI_LIGHT_BIO2
	int groupFrontComponentConnectionListIndex;
	int numberOfWordsCurrentlyParsed;
	#endif
	#ifdef SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
	int wordIndex;
	#endif
};



class SANIComponentClass
{
	public: bool componentHasSubcomponents(SANIComponent* component);	
};


#endif

#endif