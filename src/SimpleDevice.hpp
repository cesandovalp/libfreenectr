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

    bool GetFrameRGB( std::vector<uint8_t> &buffer )
    {
      std::lock_guard<std::mutex> guard( video_mutex );

      if( !video_frame )
        return false;

      buffer.swap( buffer_video );

      video_frame = false;

      return true;
    }

  protected:
    std::vector<uint8_t> buffer_video;
    std::mutex video_mutex;
    bool video_frame;
    bool depth_frame;
};
