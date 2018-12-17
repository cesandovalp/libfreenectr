#include <Rcpp.h>
#include <string>

#include "SimpleDevice.hpp"
#include "VideoController.hpp"

using namespace Rcpp;

Freenect::Freenect    freenect;
VideoController*      controller;
SimpleFreenectDevice* device = NULL;

// [[Rcpp::export]]
void CreateSimpleDevice()
{
  if( device == NULL )
    device = &freenect.createDevice<SimpleFreenectDevice>( 0 );
}

// [[Rcpp::export]]
void SetTiltDegrees( int angle )
{
  device->setTiltDegrees( angle );
}

// [[Rcpp::export]]
NumericVector GetAccelerometers()
{
  NumericVector result;

  double x, y, z;

  device->updateState();
  device->getState().getAccelerometers( &x, &y, &z );
  
  result = NumericVector::create( _["x"] = x, _["y"] = y, _["z"] = z );
  
  return result;
}

// [[Rcpp::export]]
double GetTiltDegrees()
{
  device->updateState();
  
  return device->getState().getTiltDegs();
}

// [[Rcpp::export]]
void SetLed( std::string status )
{
  if( status == "red" )
  {
    device->setLed( LED_RED );
    return;
  }
  if( status == "green" )
  {
    device->setLed( LED_GREEN );
    return;
  }
  if( status == "yellow" )
  {
    device->setLed( LED_YELLOW );
    return;
  }
  if( status == "blink green" )
  {
    device->setLed( LED_BLINK_GREEN );
    return;
  }
  if( status == "blink red yellow" )
  {
    device->setLed( LED_BLINK_RED_YELLOW );
    return;
  }
  if( status == "off" )
  {
    device->setLed( LED_OFF );
    return;
  }
}

// [[Rcpp::export]]
int DeviceCount()
{
  return freenect.deviceCount();
}

// [[Rcpp::export]]
void StartVideo()
{
  device->startVideo();
}

// [[Rcpp::export]]
void StartDepth()
{
  device->startDepth();
}

// [[Rcpp::export]]
NumericVector GetFrameRGB()
{
  std::vector<uint8_t> buffer;

  buffer.resize( device->width * device->height * 3 );

  device->updateState();
  device->GetFrameRGB( buffer );

  return NumericVector( buffer.begin(), buffer.end() );
}

// [[Rcpp::export]]
NumericVector GetFrameDepth()
{
  std::vector<uint16_t> buffer;

  buffer.resize( 640 * 480 * 3 );

  device->updateState();
  device->GetFrameDepth( buffer );

  return NumericVector( buffer.begin(), buffer.end() );
}

void DrawGLScene()
{
  controller->DrawGLScene();
}

void ResizeGLScene( int width, int height )
{
  controller->ResizeGLScene( width, height );
}

// [[Rcpp::export]]
void StartGLUTVideo( bool RGB = true, bool Depth = false)
{
  int g_argc = 0;

  glutInit( &g_argc, NULL );
  controller = new VideoController( device );

  if( RGB )
  {
    controller->GLThread( 0, 0, device->width, device->height, DrawGLScene, ResizeGLScene, "LibFreenect RGB" );
  }
  if( Depth )
  {
    controller->GLThread( 0, 0, 640, 480, DrawGLScene, ResizeGLScene, "LibFreenect Depth" );
  }
  
  controller->InitGL();
  glutMainLoop();
}

// [[Rcpp::export]]
NumericVector GetResolution()
{
  return NumericVector::create( _["width"] = device->width, _["height"] = device->height );
}

// [[Rcpp::export]]
void SetHighResolution()
{
  device->SetHighResolution();
  device->updateState();
}

// [[Rcpp::export]]
void SetMediumResolution()
{
  device->SetMediumResolution();
  device->updateState();
}
