#include <Rcpp.h>
#include <string>

#include "libfreenect.hpp"

#if defined(__APPLE__)
#include <GLUT/glut.h>
#include <GLUT/freeglut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

using namespace Rcpp;
Freenect::Freenect freenect;

// [[Rcpp::export]]
SEXP CreateSimpleDevice( )
{
  Freenect::FreenectDevice* result = &freenect.createDevice<Freenect::FreenectDevice>( 0 );
  return XPtr<Freenect::FreenectDevice>( result );
}

// [[Rcpp::export]]
void SetTiltDegrees( SEXP device, int angle )
{
  Freenect::FreenectDevice* device_ = XPtr<Freenect::FreenectDevice>( device );
  device_->setTiltDegrees( angle );
}

// [[Rcpp::export]]
NumericVector GetAccelerometers( SEXP device )
{
  NumericVector result;

  double x, y, z;

  Freenect::FreenectDevice* device_ = XPtr<Freenect::FreenectDevice>( device );
  device_->updateState();
  device_->getState().getAccelerometers( &x, &y, &z );
  
  result = NumericVector::create( _["x"] = x, _["y"] = y, _["z"] = z );
  
  return result;
}

// [[Rcpp::export]]
double GetTiltDegrees( SEXP device )
{
  Freenect::FreenectDevice* device_ = XPtr<Freenect::FreenectDevice>( device );
  device_->updateState();
  
  return device_->getState().getTiltDegs();
}

// [[Rcpp::export]]
void SetLed( SEXP device, std::string status )
{
  Freenect::FreenectDevice* device_ = XPtr<Freenect::FreenectDevice>( device );
  if( status == "red" )
  {
    device_->setLed( LED_RED );
    return;
  }
  if( status == "green" )
  {
    device_->setLed( LED_GREEN );
    return;
  }
  if( status == "yellow" )
  {
    device_->setLed( LED_YELLOW );
    return;
  }
  if( status == "blink green" )
  {
    device_->setLed( LED_BLINK_GREEN );
    return;
  }
  if( status == "blink red yellow" )
  {
    device_->setLed( LED_BLINK_RED_YELLOW );
    return;
  }
  if( status == "off" )
  {
    device_->setLed( LED_OFF );
    return;
  }
}
