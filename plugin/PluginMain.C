#include <UT/UT_DSOVersion.h>

#include <UT/UT_Math.h>
#include <UT/UT_Interrupt.h>
#include <GU/GU_Detail.h>
#include <GU/GU_PrimPoly.h>
#include <GU/GU_PrimSphere.h>
#include <CH/CH_LocalVariable.h>
#include <PRM/PRM_Include.h>
#include <PRM/PRM_SpareData.h>
#include <OP/OP_Operator.h>
#include <OP/OP_OperatorTable.h>

#include <limits.h>
#include "PluginMain.h"

#include <UT/UT_NTStreamUtil.h>
#include <UT/UT_IStream.h>
#include <CMD/CMD_Args.h>
#include <PI/PI_ResourceManager.h>
#include <MOT/MOT_Director.h>


using namespace HDK_Sample;

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Register your Nodes <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

/*
* Register the SOP nodes. This function is called from Houdini's Engine.
* This basically adds your Node to the list of nodes that Houdini maintains.
* You can currently leave it as it is. If you change the name of your node be sure to update here.
*/
void newSopOperator(OP_OperatorTable* table) {

	table->addOperator(
		new OP_Operator("MyCustomNode", // Internal name
			"MyCustomNode", // The name shown in Houdini's UI
			MyCustomNode::myConstructor, // How to build the SOP (basically a pointer to a function that creates an instance of your node
			MyCustomNode::mParameterList, // My parameters : ANy parameters such as GUI elements you want to send
			0, // Min # of sources
			0, // Max # of sources
			MyCustomNode::mLocalVariables, // Local variables : Not really sure, better to leave as it
			OP_FLAG_GENERATOR) // Flag it as generator
	);
}


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> MyCustomNode Start <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

/*
* Declare the GUI parameters to the nodes
*/
// Declare: variableName(<internal name>, <GUI name>);
static PRM_Name someIntVariable("myInt", "My Int");
static PRM_Name someFloatVariable("myFloat", "Velocity Blend");
static PRM_Name someStringVariable("myString", "Max Velocity");

// How to declare a button : variableName(<internal name>, <GUI name>);
static PRM_Name generateCommandName("generateCommand", "Generate");


/*
* Declare the defaults of the defined parameters. This will the initial value in the GUI
*/

static PRM_Default someIntVariableDefault(42);
static PRM_Default someFloatVariableDefault(4.2);
static PRM_Default someStringVariableDefault(0.0, "Sup Bruh!");

/*
* Package the declared parameters so that it can be used to add to the operator table
*/
PRM_Template MyCustomNode::mParameterList[] = {
	
	PRM_Template(PRM_INT, PRM_Template::PRM_EXPORT_MIN, 1, &someIntVariable, &someIntVariableDefault, 0),
	PRM_Template(PRM_FLT, PRM_Template::PRM_EXPORT_MIN, 1, &someFloatVariable, &someFloatVariableDefault, 0),
	PRM_Template(PRM_STRING, PRM_Template::PRM_EXPORT_MIN, 1, &someStringVariable, &someStringVariableDefault, 0),

	PRM_Template(PRM_CALLBACK, 1, &generateCommandName, 0, 0, 0, MyCustomNode::generateCallback),

	PRM_Template()
};

// Here's how we define local variables for the SOP.
enum {
	VAR_PT,
	// Point number of the star
	VAR_NPT // Number of points in the star
};

CH_LocalVariable MyCustomNode::mLocalVariables[] = {
	{ "PT", VAR_PT, 0 }, // The table provides a mapping
	{ "NPT", VAR_NPT, 0 }, // from text string to integer token
	{ 0, 0, 0 },
};

bool MyCustomNode::evalVariableValue(fpreal& val, int index, int thread) {

	// Not one of our variables, must delegate to the base class.
	return SOP_Node::evalVariableValue(val, index, thread);
}

// Creates an Instance of our class
OP_Node* MyCustomNode::myConstructor(OP_Network* net, const char* name, OP_Operator* op) {
	return new MyCustomNode(net, name, op);
}

// Our constructor
MyCustomNode::MyCustomNode(OP_Network* net, const char* name, OP_Operator* op) : SOP_Node(net, name, op) {}

// Destructor
MyCustomNode::~MyCustomNode() {}


unsigned MyCustomNode::disableParms() {
	return 0;
}

// Important function - Called when ever GUi is modified
OP_ERROR MyCustomNode::cookMySop(OP_Context& context) {

	// You can print to Houdini's console like this. Very useful
	// This is C style print. 
	// Printing  : printf(" some ints %d %f %s \n", someInt, someFloat, someString);
	printf("Agent Node is Cooked >>>> \n");

	fpreal now = context.getTime();
	
	// Read your inputs from GUI like this

	UT_String readingSomeString;
	READMYSTRING(readingSomeString, now);
	int readingSomeInt = READMYINT(now);
	float readingSomeFloat = READMYFLOAT(now);

	printf("You inputted INT as %d\n", readingSomeInt);
	printf("You inputted float as %f\n", readingSomeFloat);
	printf("You inputted String as %s\n", readingSomeString.toStdString());


	OP_AutoLockInputs inputs(this);
	// Check if locking caused an error
	if (inputs.lock(context) >= UT_ERROR_ABORT)
		return error();

	OP_Node::flags().timeDep = 1;
	
	// VERY USEFUL - can fetch the current frame the user has scrubbed to 
	CH_Manager *chman = OPgetDirector()->getChannelManager();
	// Gets the current frame
	fpreal currframe = chman->getSample(context.getTime());

	// You can get Instances of sibling nodes like this:


	OP_NodeList myNodeList;

	// Get the Parent Node
	OP_Node *parent = getParent();

	if (parent != nullptr) {

		parent->getAllChildren(myNodeList);

		for (int i = 0; i < myNodeList.size(); ++i) {
			// do something with myNodeList[i]
			// It will be an instance of OP_Node, so cast it to your required node type
		}
	}


	
	return error();
}

int MyCustomNode::generateCallback(void* data, int index, float time, const PRM_Template*) {

	fpreal now = time;
	
	// Fetch an instance of self
	MyCustomNode* me = (MyCustomNode*)data;
	
	OP_Context myContext(time);

	// me->doSomeThing()

	// OR 

	me->cookMySop(myContext); // recooks your sop, so you can read inputs again and do your thing
	
	return 1;
	
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Agent Node End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<