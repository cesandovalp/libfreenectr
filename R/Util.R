RotateRaw = function( x )
{
  return( t( apply( matrix( x, ncol = 480 ), 2, rev ) ) )
}

GetPhoto640x480 = function( )
{
  raw_data  = GetFrameRGB( )
  raw_index = ( 1:length( raw_data ) ) %% 3

  z1 = RotateRaw( raw_data[( raw_index == 1 )] )
  z2 = RotateRaw( raw_data[( raw_index == 2 )] )
  z3 = RotateRaw( raw_data[( raw_index == 0 )] )

  img_array = c( z1 / 255, z2 / 255, z3 / 255 )

  return( pixmapRGB( img_array, 480, 640, cellres = 1 ) )
}
