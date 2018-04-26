#pragma once

#include <SOP/SOP_Node.h>

#include <OP/OP_AutoLockInputs.h>
#include <OP/OP_Director.h>
#include <OP/OP_Operator.h>
#include <OP/OP_OperatorTable.h>

namespace HDK_Sample {

	/*
	* Your custom node. You can define any number of nodes here, but make sure you define them all here.
	* Every definition has to be inside namespace HDK_Sample
	*/

	class MyCustomNode : public SOP_Node {

	public:

		// Helper method that Houdini Engine will call to instantiate your node
		static OP_Node*  myConstructor(OP_Network*, const char*, OP_Operator*);

		// List of parameters and GUI variables
		static PRM_Template  mParameterList[];

		// Some local variables - not really used
		static CH_LocalVariable  mLocalVariables[];

	public:

		// Constructor
		MyCustomNode(OP_Network* net, const char* name, OP_Operator* op);

		// Destructor
		virtual ~MyCustomNode();

		// Not really sure - igore it
		virtual unsigned disableParms();

		// Called when ever GUI is changed - IMPORTANT
		virtual OP_ERROR cookMySop(OP_Context& context);

		// Not really sure - ignore it
		virtual bool evalVariableValue(fpreal& val, int index, int thread);

		// Not really sure - ignore it
		virtual bool  evalVariableValue(UT_String& v, int i, int thread) {
			return evalVariableValue(v, i, thread);
		}

	protected:
		static int generateCallback(void *data, int index, float time, const PRM_Template *);

	private:

		int readAnInt;
		float readAFloat;
		UT_String readAString;

		exint READMYINT(fpreal n) {
			return evalInt("myInt", 0, n); // "myInt" is the name of the variable as you define in param list
		}

		fpreal READMYFLOAT(fpreal t) {
			return evalFloat("myFloat", 0, t); // "myFloat" is the name of the variable as you define in param list
		}

		void READMYSTRING(UT_String& label, fpreal t) {
			evalString(label, "myString", 0, t); // "myString" is the name of the variable as you define in param list
		}

	};

}
