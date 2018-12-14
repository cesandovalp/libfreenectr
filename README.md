# libfreenectr

# Installation

Install the development version from GitHub:

```sh
$ git clone --recursive https://github.com/cesandovalp/libfreenectr
$ R CMD build libfreenecte;
$ R CMD INSTALL libfreenect_0.1.tar.gz
```

# Examples

## SetTiltDegrees
```{r}
library(libfreenectr)

CreateSimpleDevice()

SetTiltDegrees( 30 )
Sys.sleep( 2 )
SetTiltDegrees( 0 )
Sys.sleep( 2 )
SetTiltDegrees( -30 )
Sys.sleep( 2 )
SetTiltDegrees( 0 )

```

## SetTiltDegrees
```{r}
library(libfreenectr)

CreateSimpleDevice()

SetLed( "red" )
Sys.sleep( 2 )
SetLed( "green" )
Sys.sleep( 2 )
SetLed( "off" )

```

## Take a photo
```{r}
library(libfreenectr)
library(pixmap)

CreateSimpleDevice()
StartVideo()
photo = GetPhoto640x480( )

plot( photo )

```
