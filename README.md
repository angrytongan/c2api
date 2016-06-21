# c2api

C example for talking to [Concept2] ergometers over USB independent of
the Concept2 SDK libraries.

## Why?
* To understand how to talk to Concept2 ergs.
* Simple reference for the CSAFE protocol.
* Absence of simple command line application for talking to ergs.

## Status
* All commands from the spec are in.
* API is pretty loose and needs more work.
* Implemented commands in the CLI app are tested; others should work.

## Specification
Based on the Concept2 Communication Interface Definition 0.15 available
in the [Concept2 SDK].

## Dependencies
* [hidapi] for USB HID interface.

## Platforms
* Builds on OS X.
* Should build on Linux.

## Building
```
$ # install hidapi
$ export HIDAPI_INCDIR=/path/to/hidapi/include
$ export HIDAPI_LIBDIR=/path/to/hidapi/lib
$ make
```

## Usage
Set time of monitor to local time:
```
$ ./c2 -s
```

Set workout time of 0:15:00:
```
$ ./c2 -r -t 0:15:0 -p 0 -y idle -y haveid -y inuse
```

Set workout distance of 500m:
```
$ ./c2 -r -d 500 -p 0 -y idle -y haveid -y inuse
```

Get erg and workout info:
```
$ ./c2 -g erg -g workoout
```

Go back to ready state:
```
$ ./c2 -y finished -y ready
```

Help:
```
$ ./c2 -h
```

## Inspiration:
* [easy-erg]
* [PyRow]

[Concept2]: http://www.concept2.com.au/
[Concept2 SDK]: http://www.concept2.com.au/service/software/software-development-kit
[hidapi]: https://github.com/signal11/hidapi
[easy-erg]: https://github.com/kdahlhaus/easy-erg
[PyRow]: https://github.com/uvd/PyRow
