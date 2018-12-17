RotateRaw = function( x, resolution )
{
  return( t( apply( matrix( x, ncol = resolution[2] ), 2, rev ) ) )
}

GetPhoto = function( )
{
  resolution = GetResolution()
  raw_data  = GetFrameRGB( )
  if( summary( raw_data )["Median"] == 0 )
    raw_data  = GetFrameRGB( )
  raw_index = ( 1:length( raw_data ) ) %% 3

  z1 = RotateRaw( raw_data[( raw_index == 1 )], resolution )
  z2 = RotateRaw( raw_data[( raw_index == 2 )], resolution )
  z3 = RotateRaw( raw_data[( raw_index == 0 )], resolution )

  img_array = c( z1, z2, z3 )

  return( pixmapRGB( img_array, resolution[2], resolution[1], cellres = 255 ) )
}

GetDepth = function( )
{
  resolution = c( 640, 480 )
  raw_data  = GetFrameDepth( )
  if( summary( raw_data )["Median"] == 0 )
    raw_data  = GetFrameDepth( )
  raw_index = ( 1:length( raw_data ) ) %% 3

  z1 = RotateRaw( raw_data[( raw_index == 1 )], resolution )
  z2 = RotateRaw( raw_data[( raw_index == 2 )], resolution )
  z3 = RotateRaw( raw_data[( raw_index == 0 )], resolution )

  img_array = c( z1, z2, z3 )

  return( pixmapRGB( img_array, 480, 640, cellres = 255 ) )
}
