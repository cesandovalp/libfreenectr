# libfreenectr

# Installation

Install the development version from GitHub:

```sh
$ git clone --recursive https://github.com/cesandovalp/libfreenectr
$ R CMD build libfreenectr;
$ R CMD INSTALL libfreenectr_0.1.tar.gz
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

## SetLed
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
photo = GetPhoto( )
plot( photo )

SetHighResolution()
photo = GetPhoto( )
plot( photo )

```

## Take a depth photo
```{r}
library(libfreenectr)
library(pixmap)

CreateSimpleDevice()
StartDepth()
photo = GetDepth( )
plot( photo )

```

## StartVideo
```{r}
library(libfreenectr)

CreateSimpleDevice()
StartVideo()
StartGLUTVideo( )

```

## StartVideo Max Resolution
```{r}
library(libfreenectr)

CreateSimpleDevice()
StartVideo()
SetHighResolution()
StartGLUTVideo( )

```
