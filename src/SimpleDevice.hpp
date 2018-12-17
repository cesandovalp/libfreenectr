#pragma once

#include <mutex>
#include "libfreenect.hpp"

class SimpleFreenectDevice : public Freenect::FreenectDevice
{
  public:
    int width;
    int height;

    SimpleFreenectDevice( freenect_context* _ctx, int _index ) : Freenect::FreenectDevice( _ctx, _index )
    {
      SetMediumResolution();
      buffer_depth.resize( freenect_find_video_mode( FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB ).bytes );
      gamma.resize( 2048 );
      
      for( unsigned int i = 0 ; i < 2048 ; i++ )
      {
        float v = i / 2048.0;
        v = std::pow( v, 3 ) * 6;
        gamma[i] = v * 6 * 256;
      }
    }
    
    void SetMediumResolution()
    {
      buffer_video.resize( freenect_find_video_mode( FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB ).bytes );
      setVideoFormat( FREENECT_VIDEO_RGB, FREENECT_RESOLUTION_MEDIUM );
      width  = 640;
      height = 480;
    }
    
    void SetHighResolution()
    {
      buffer_video.resize( freenect_find_video_mode( FREENECT_RESOLUTION_HIGH, FREENECT_VIDEO_RGB ).bytes );
      setVideoFormat( FREENECT_VIDEO_RGB, FREENECT_RESOLUTION_HIGH );
      width  = 1280;
      height = 1024;
    }

    void VideoCallback( void* video, uint32_t timestamp )
    {
      std::lock_guard<std::mutex> guard( video_mutex );
      uint8_t* rgb = static_cast<uint8_t*>( video );
      copy( rgb, rgb + getVideoBufferSize(), buffer_video.begin() );
      video_frame = true;
    }
    
    void DepthCallback( void* _depth, uint32_t timestamp )
    {
      std::lock_guard<std::mutex> guard( depth_mutex );
      uint16_t* depth = static_cast<uint16_t*>( _depth );
      for( unsigned int i = 0 ; i < 640 * 480 ; i++ )
      {
        int pval = gamma[depth[i]];
        int lb = pval & 0xff;
        switch( pval >> 8 )
        {
          case 0:
            buffer_depth[3 * i + 0] = 255;
            buffer_depth[3 * i + 1] = 255 - lb;
            buffer_depth[3 * i + 2] = 255 - lb;
          break;
          case 1:
            buffer_depth[3 * i + 0] = 255;
            buffer_depth[3 * i + 1] = lb;
            buffer_depth[3 * i + 2] = 0;
          break;
          case 2:
            buffer_depth[3 * i + 0] = 255 - lb;
            buffer_depth[3 * i + 1] = 255;
            buffer_depth[3 * i + 2] = 0;
          break;
          case 3:
            buffer_depth[3 * i + 0] = 0;
            buffer_depth[3 * i + 1] = 255;
            buffer_depth[3 * i + 2] = lb;
          break;
          case 4:
            buffer_depth[3 * i + 0] = 0;
            buffer_depth[3 * i + 1] = 255 - lb;
            buffer_depth[3 * i + 2] = 255;
          break;
          case 5:
            buffer_depth[3 * i + 0] = 0;
            buffer_depth[3 * i + 1] = 0;
            buffer_depth[3 * i + 2] = 255 - lb;
          break;
          default:
            buffer_depth[3 * i + 0] = 0;
            buffer_depth[3 * i + 1] = 0;
            buffer_depth[3 * i + 2] = 0;
          break;
        }
      }
      depth_frame = true;
    }

    bool GetFrameRGB( std::vector<uint8_t> &buffer )
    {
      std::lock_guard<std::mutex> guard( video_mutex );

      if( !video_frame )
        return false;

      buffer.swap( buffer_video );

      video_frame = false;

      return true;
    }
    
    bool GetFrameDepth( std::vector<uint16_t> &buffer )
    {
      std::lock_guard<std::mutex> guard( depth_mutex );

      if( !depth_frame )
        return false;
      
      buffer.swap( buffer_depth );

      depth_frame = false;
      return true;
    }

  protected:
    std::vector<uint8_t>  buffer_video;
    std::vector<uint16_t> buffer_depth;
    std::vector<uint16_t> gamma;
    std::mutex video_mutex;
    std::mutex depth_mutex;
    bool video_frame;
    bool depth_frame;
};
