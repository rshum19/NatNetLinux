/*  File    : sfun_NatNet_cpp.cpp
 *  Abstract:
 *
 *      Example of an C++ S-function which stores an C++ object in
 *      the pointers vector PWork.
 *
 *  Copyright 1990-2013 The MathWorks, Inc.
 */

#include <iostream>
#include "mex.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <NatNetLinux/NatNet.h>
#include <NatNetLinux/CommandListener.h>
#include <NatNetLinux/FrameListener.h>

#include <boost/program_options.hpp>
#include <time.h>

class Globals
{
public:
   
   // Parameters read from the command line
   static uint32_t localAddress;
   static uint32_t serverAddress;
   
   // State of the main() thread.
   static bool run;
};

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  sfun_counter_cpp

/*
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */
#include "simstruc.h"

#define IS_PARAM_DOUBLE(pVal) (mxIsNumeric(pVal) && !mxIsLogical(pVal) &&\
!mxIsEmpty(pVal) && !mxIsSparse(pVal) && !mxIsComplex(pVal) && mxIsDouble(pVal))

/*====================*
 * S-function methods *
 *====================*/

#define MDL_CHECK_PARAMETERS
#if defined(MDL_CHECK_PARAMETERS)  && defined(MATLAB_MEX_FILE)
/*
 * Check to make sure that each parameter is 1-d and positive
 */
static void mdlCheckParameters(SimStruct *S)
{

    const mxArray *pVal0 = ssGetSFcnParam(S,0);

    if ( !IS_PARAM_DOUBLE(pVal0)) {
        ssSetErrorStatus(S, "Parameter to S-function must be a double scalar");
        return;
    } 
}
#endif


/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *    The sizes information is used by Simulink to determine the S-function
 *    block's characteristics (number of inputs, outputs, states, etc.).
 */
static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 1);  /* Number of expected parameters */
#if defined(MATLAB_MEX_FILE)
    if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) {
        mdlCheckParameters(S);
        if (ssGetErrorStatus(S) != NULL) {
            return;
        }
    } else {
        return; /* Parameter mismatch will be reported by Simulink */
    }
#endif
    ssSetSFcnParamTunable(S, 0, 0);

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    // Configure Input ports
    int NUM_INPUT_PORTS = 1;
    if (!ssSetNumInputPorts(S, NUM_INPUT_PORTS)) return;
    for (int i=0; i<NUM_INPUT_PORTS; i++) {
        ssSetInputPortWidth(S,i,4);
        ssSetInputPortDirectFeedThrough(S,i,1);
    }
   
    // Configure Output ports
    int NUM_OUTPUT_PORTS = 1;
    if (!ssSetNumOutputPorts(S, NUM_OUTPUT_PORTS)) return;
    ssSetOutputPortWidth(S, 0, 1);

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 1); // reserve element in the pointers vector
    ssSetNumModes(S, 0); // to store a C++ object
    ssSetNumNonsampledZCs(S, 0);

    ssSetSimStateCompliance(S, USE_CUSTOM_SIM_STATE);

    ssSetOptions(S, 0);
}



/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    This function is used to specify the sample time(s) for your
 *    S-function. You must register the same number of sample times as
 *    specified in ssSetNumSampleTimes.
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, mxGetScalar(ssGetSFcnParam(S, 0)));
    ssSetOffsetTime(S, 0, 0.0);
    ssSetModelReferenceSampleTimeDefaultInheritance(S);
}

#define MDL_START  /* Change to #undef to remove function */
#if defined(MDL_START) 
  /* Function: mdlStart =======================================================
   * Abstract:
   *    This function is called once at start of model execution. If you
   *    have states that should be initialized once, this is the place
   *    to do it.
   *    
   *    Persistent variables are intialized inside this function
   */
  static void mdlStart(SimStruct *S)
  {
      /*
      ssGetPWork(S)[0] = (void *) new Globals; // store new C++ object in the
  
        /*uint32_t Globals::localAddress = 0;
        uint32_t Globals::serverAddress = 0;
        bool Globals::run = false;
      
      
      // Version numer of the NatNet protocol, as reported by the server
      unsigned char natNetMajor;
      unsigned char natNetMajor;
      
      // Initialize Sockets
      int sdCommand;
      int sdData;
      
      // Get addresses
      //local-addr = ;
      //server-addr = ;
      
      // Set addresses
      Globals::localAddress = local-addr;
      Globals:serverAddress = sever-addr;
      
      // Use this socket address to send commands to the server
      struct sockaddr_in serverCommands = NatNet::createAddress(Globals::serverAddress, NatNet::commandPort);

      // Create sockets
      sdCommand = NatNet::createCommandSocket( Globals::localAddress );
      sdData = NatNet::createDataSocket( Globals::localAddress );
      
      // Start the CommandListener in a new thread
      CommandListener commandListener(sdCommand);
      commandListener.start();
   
      // Send a ping packet to the server so that it sends us the NatNet version
      // in its response to commandListener.
      
      // Wait here for ping response to give a us the NatNet version
      
      */
  }                                            // pointers vector
#endif /*  MDL_START */

/* Function: mdlOutputs =======================================================
 * Abstract:
 *    In this function, you compute the outputs of your S-function
 *    block.
 *    Main loop function
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    counter *c = (counter *) ssGetPWork(S)[0];   // retrieve C++ object from
    real_T  *y = ssGetOutputPortRealSignal(S,0); // the pointers vector and use
    y[0] = c->output();                          // member functions of the
    UNUSED_ARG(tid);                             // object
    
    // Start up a FrameListener in a new thread
    
    
    
}                                                

#ifdef MATLAB_MEX_FILE
/* For now mdlG[S]etSimState are only supported in normal simulation */

/* Define to indicate that this S-Function has the mdlG[S]etSimState mothods */
#define MDL_SIM_STATE

/* Function: mdlGetSimState =====================================================
 * Abstract:
 *
 */
static mxArray* mdlGetSimState(SimStruct* S)
{
    counter* c = (counter*) ssGetPWork(S)[0];
    mxArray* outSS = mxCreateDoubleMatrix(1,1,mxREAL);
    mxGetPr(outSS)[0] = c->getX();
    return outSS;
}
/* Function: mdlGetSimState =====================================================
 * Abstract:
 *
 */
static void mdlSetSimState(SimStruct* S, const mxArray* ma)
{
    counter* c = (counter*) ssGetPWork(S)[0];
    c->setX(mxGetPr(ma)[0]);
}

#endif


/* Function: mdlTerminate =====================================================
 * Abstract:
 *    In this function, you should perform any actions that are necessary
 *    at the termination of a simulation.  For example, if memory was
 *    allocated in mdlStart, this is the place to free it.
 */
static void mdlTerminate(SimStruct *S)
{
    counter *c = (counter *) ssGetPWork(S)[0]; // retrieve and destroy C++
    delete c;                                  // object in the termination
}                                              // function
/*======================================================*
 * See sfuntmpl.doc for the optional S-function methods *
 *======================================================*/

/*=============================*
 * Required S-function trailer *
 *=============================*/

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif

