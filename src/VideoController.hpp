#pragma once

#include <thread> 

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

typedef void (*DrawCallback)();
typedef void (*ResizeCallback)( int, int );

class VideoController
{
  SimpleFreenectDevice* device;
  GLuint gl_rgb_tex;
  
  public:
  
    VideoController( SimpleFreenectDevice* device ) : device(device)
    {
      InitGL();
    }
  
    void InitGL()
    {
      glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
      glClearDepth( 1.0 );
      glDepthFunc( GL_LESS );
      glDisable( GL_DEPTH_TEST );
      glEnable( GL_BLEND );
      glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      glShadeModel( GL_SMOOTH );
      glGenTextures( 1, &gl_rgb_tex );
      glBindTexture( GL_TEXTURE_2D, gl_rgb_tex );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
      ResizeGLScene( device->width, device->height );
    }

    int GLThread( int x, int y, DrawCallback draw, ResizeCallback resize )
    {
      int g_argc = 0;
      int window_id = 0;

      glutInit( &g_argc, NULL );

      glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH );
      glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS );
      glutInitWindowSize( device->width, device->height );
      glutInitWindowPosition( x, y );

      window_id = glutCreateWindow( "LibFreenect" );

      glutDisplayFunc( draw );
      glutIdleFunc( draw );
      glutReshapeFunc( resize );
      //glutKeyboardFunc( &keyPressed );

      InitGL();

      glutMainLoop();

      return window_id;
    }

    void DrawGLScene()
    {
      static std::vector<uint8_t> rgb( device->width * device->height * 3 );

      device->updateState();

      device->GetFrameRGB( rgb );

      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
      glLoadIdentity();

      glEnable( GL_TEXTURE_2D );

      glBindTexture( GL_TEXTURE_2D, gl_rgb_tex );
      if( device->getVideoFormat() == FREENECT_VIDEO_RGB || device->getVideoFormat() == FREENECT_VIDEO_YUV_RGB )
        glTexImage2D( GL_TEXTURE_2D, 0, 3, device->width, device->height, 0, GL_RGB, GL_UNSIGNED_BYTE, &rgb[0] );
      else
        glTexImage2D( GL_TEXTURE_2D, 0, 1, device->width, device->height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, &rgb[0] );

      glBegin( GL_TRIANGLE_FAN );
      glColor4f( 255.0f, 255.0f, 255.0f, 255.0f );
      glTexCoord2f( 0, 0 ); glVertex3f(   0,   0, 0 );
      glTexCoord2f( 1, 0 ); glVertex3f( device->width,   0, 0 );
      glTexCoord2f( 1, 1 ); glVertex3f( device->width, device->height, 0 );
      glTexCoord2f( 0, 1 ); glVertex3f(   0, device->height, 0 );
      glEnd();

      glutSwapBuffers();
    }
    
    void ResizeGLScene( int width, int height )
    {
      glViewport( 0, 0, width, height );
      glMatrixMode( GL_PROJECTION );
      glLoadIdentity();
      glOrtho( 0, device->width, device->height, 0, -1.0f, 1.0f );
      glMatrixMode( GL_MODELVIEW );
    } 
};
