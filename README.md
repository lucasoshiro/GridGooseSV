# GridGooseSV

[![GitHub actions](https://github.com/lucasoshiro/GridGooseSV/actions/workflows/linux.yml/badge.svg)](https://github.com/lucaoshiro/GridGooseSV/actions)
[![GitHub actions](https://github.com/lucasoshiro/GridGooseSV/actions/workflows/mac.yml/badge.svg)](https://github.com/lucaoshiro/GridGooseSV/actions)
[![GitHub actions](https://github.com/lucasoshiro/GridGooseSV/actions/workflows/windows.yml/badge.svg)](https://github.com/lucaoshiro/GridGooseSV/actions)

Native implementation of IEC61850 protocols for the
[ns-3](https://www.nsnam.org/) network simulator, based on
[libiec61850](https://libiec61850.com/).

## Features

This `ns-3` module provides:

- A low level API for the IEC61850 protocols, which you can use for implement 
  your own applications inside IEC61850;
- High level applications, which you can configure for using in your 
  simulations.

The low level API is, basically, the same provided by `libiec61850`. This way,
you can follow the
[libiec61850 API documentation](https://support.mz-automation.de/doc/libiec61850/c/latest/index.html).

### Built-in protocols

- [x] SV
- [x] GOOSE
- [ ] MMS

### Built-in applications

- [x] SV Publisher
- [x] SV Subscriber
- [x] GOOSE Publisher
- [x] GOOSE Subscriber
- [ ] MMS Server
- [ ] MMS Client

### Built-in examples

- [x] Simple SV publisher/subscriber 
- [x] SV Load test
- [x] Realtime SV publisher/subscriber
- [x] Simple GOOSE publisher/subscriber

## Installing

### Supported plaftorms

- [x] Linux
- [x] macOS
- [x] Windows

### Dependencies

This module was developed using `ns-3 3.44`. Probably it will work with any
`ns-3` versions based on CMake (i.e. >= 3.36). 

### Installation steps

1. Download `ns-3`, following the [official documentation](https://www.nsnam.org/docs/installation/html/index.html).
   Note that it's not required to have `bake` installed;
  - Suggestion: clone directly from GitLab:
    ~~~bash
    git clone git@gitlab.com:nsnam/ns-3-dev.git
    ~~~
2. Clone this repository inside the `contrib` directory in ns-3;
3. Configure `ns-3` using the `optimized` profile, enabling tests and examples:
   ~~~bash
   ./ns3 configure --build-profile=optimized --enable-examples --enable-tests
   ~~~
4. Build `ns-3` with GridGooseSV:
   ~~~bash
   ./ns3 build -j $(nproc)
   ~~~

And that's it. No further steps!

> Note: some warnings are expected. Because of them, only the `optimized`
> build profile is supported. They don't affect the working of this module,
> but, of course, they need to be solved. By now we haven't had enough
> time to fix them, but contributions are welcome!

## Testing

We provided some test to assert that everything is working as expected. You can
run them by calling:

~~~bash
./test.py --suite="gridgoosesv" 
~~~

in `ns-3` top-level directory.

## Using

### Low-level API

### `ns-3` Applications

## Implementation details

In order to get everything working as expected, we needed to:

1. Make the C code from `libiec61850` be compatible with C++
2. Create a new `libiec61850` HAL for using `ns-3` simulated stuff (e.g. 
   sockets and kernel) instead of real resources provided by operating systems 
   and hardware.

Porting the `libiec61850` code to C++ required some adjustments due to some
corner cases where C is not compatible with C++. Also, we had some name conflicts
between `ns-3` names and `libiec61850` names, and that's why we used namespaces.
This way, the `libiec61850` code that was changed here can't be used upstream, as
our changes made it a C++ code not compatible with C (yeah, the opposite).

Given that `libiec61850` is built on top of a HAL (Hardware Abstraction Layer,
which in this case would be better named a PAL - Platform Abstraction Layer)
that allows it to run in different platforms (by default, Linux, Mac, BSD and
Windows), most of the job was writing a new layer over `ns-3`. However,
`libiec61850` was created to be used in real situations, in real time and in a
single machine; here we're in a simulated environment dealing with simulated time,
with several simulated machines (called _nodes_ in `ns-3`).

This way, we needed to:

- Replace the real sockets by `ns-3` sockets;
- Use the config path of the `NetDevice`s as the interface names (`libiec61850`
  expects the interface names provided by the operating system);
- Replace the time readings by the simulated time readings;
- Replace the `sleep`s by event scheduling.
