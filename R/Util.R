RotateRaw = function( x )
{
  resolution = GetResolution()
  return( t( apply( matrix( x, ncol = resolution[2] ), 2, rev ) ) )
}

GetPhoto = function( )
{
  resolution = GetResolution()
  raw_data  = GetFrameRGB( ) / 255
  if( summary( raw_data )["Median"] == 0 )
    raw_data  = GetFrameRGB( ) / 255
  raw_index = ( 1:length( raw_data ) ) %% 3

  z1 = RotateRaw( raw_data[( raw_index == 1 )] )
  z2 = RotateRaw( raw_data[( raw_index == 2 )] )
  z3 = RotateRaw( raw_data[( raw_index == 0 )] )

  img_array = c( z1, z2, z3 )

  return( pixmapRGB( img_array, resolution[2], resolution[1], cellres = 1 ) )
}
