/*******************************************************************************
*                                                                              *
*   PrimeSense NITE 1.3 - Single Control Sample                                *
*   Copyright (C) 2010 PrimeSense Ltd.                                         *
*                                                                              *
*******************************************************************************/

//-----------------------------------------------------------------------------
// Headers
//-----------------------------------------------------------------------------
// General headers
#include <stdio.h>
// OpenNI headers
#include <XnOpenNI.h>
// NITE headers
#include <XnVSessionManager.h>
#include "XnVMultiProcessFlowClient.h"
#include <XnVWaveDetector.h>

//ROS headers
#include <ros/ros.h>
#include <ros/package.h> //we use this to get file paths

#include <stdlib.h>
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include <poe_kinect/pointerpos.h>
#include <poe_kinect/gestures.h>

#include <sstream>

#include <XnOpenNI.h>
#include <XnCodecIDs.h>
#include <XnCppWrapper.h>


// xml to initialize OpenNI

//TODO: Figure out relative path references
//const char* pack_path = (ros::package::getPath("poe_kinect") + "/src/Data/Sample-Tracking.xml").c_str();
//#define SAMPLE_XML_FILE pack_path

#define SAMPLE_XML_FILE "Data/Sample-Tracking.xml"
#define SAMPLE_XML_FILE_LOCAL "Sample-Tracking.xml"

using std::string;


xn::Context g_Context;
xn::ScriptNode g_ScripeNode;
xn::DepthGenerator g_DepthGenerator;
xn::SceneAnalyzer g_SceneAnalyzer;
xn::Recorder* g_pRecorder;


//Below we declare the variables we're going to use to store our outbount ROS messages
int xpos;
int ypos;
int zpos;
bool wave;
bool sess_start;
bool sess_end;

//-----------------------------------------------------------------------------
// Callbacks
//-----------------------------------------------------------------------------

// Callback for when the focus is in progress
void XN_CALLBACK_TYPE SessionProgress(const XnChar* strFocus, const XnPoint3D& ptFocusPoint, XnFloat fProgress, void* UserCxt)
{
  printf("Session progress (%6.2f,%6.2f,%6.2f) - %6.2f [%s]\n", ptFocusPoint.X, ptFocusPoint.Y, ptFocusPoint.Z, fProgress,  strFocus);
}
// callback for session start
void XN_CALLBACK_TYPE SessionStart(const XnPoint3D& ptFocusPoint, void* UserCxt)
{
  printf("Session started. Please wave (%6.2f,%6.2f,%6.2f)...\n", ptFocusPoint.X, ptFocusPoint.Y, ptFocusPoint.Z);
  sess_start = true; //we set session start TRUE, allows us to generate a "hello" response
}
// Callback for session end
void XN_CALLBACK_TYPE SessionEnd(void* UserCxt)
{
  printf("Session ended. Please perform focus gesture to start session\n");
  sess_end = true; //we set session end TRUE, allows us to generate a "goodbye" response
}
// Callback for wave detection
void XN_CALLBACK_TYPE OnWaveCB(void* cxt)
{
  printf("Wave!\n");
  wave = true; //we set a wave flag, allows us to generate a generic interaction response
}
// callback for a new position of any hand
void XN_CALLBACK_TYPE OnPointUpdate(const XnVHandPointContext* pContext, void* cxt)
{ printf("%d: (%f,%f,%f) [%f]\n", pContext->nID, pContext->ptPosition.X, pContext->ptPosition.Y, pContext->ptPosition.Z, pContext->fTime);
  //We pull data from the NiTE API, and pipe these to our outbound message holders
  xpos = pContext->ptPosition.X;
  ypos = pContext->ptPosition.Y;
  zpos = pContext->ptPosition.Z;
}


//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------

XnBool fileExists(const char *fn)
{
  XnBool exists;
  xnOSDoesFileExist(fn, &exists);
  return exists;
}


// this sample can run either as a regular sample, or as a client for multi-process (remote mode)
int main(int argc, char** argv)
{
  xn::Context context;
  xn::ScriptNode scriptNode;
  XnVSessionGenerator* pSessionGenerator;
  XnBool bRemoting = FALSE;

  XnStatus rc = XN_STATUS_OK;
  xn::EnumerationErrors errors;

  rc = g_Context.InitFromXmlFile(SAMPLE_XML_FILE, g_ScripeNode, &errors);
  //CHECK_ERRORS(rc, errors, "InitFromXmlFile");
  //CHECK_RC(rc, "InitFromXml");

  rc = g_Context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_DepthGenerator);
  //CHECK_RC(rc, "Find depth generator");
  rc = g_Context.FindExistingNode(XN_NODE_TYPE_SCENE, g_SceneAnalyzer);
  //CHECK_RC(rc, "Find scene analyzer");

  rc = g_Context.StartGeneratingAll();
  //CHECK_RC(rc, "StartGenerating");

  // Main loop
  while (!xnOSWasKeyboardHit())
  {
  }

  return 0;
}
