# GridGooseSV

[![GitHub actions](https://github.com/lucasoshiro/GridGooseSV/actions/workflows/push.yml/badge.svg)](https://github.com/lucaoshiro/GridGooseSV/actions)
[![GitHub actions](https://github.com/lucasoshiro/GridGooseSV/actions/workflows/mac.yml/badge.svg)](https://github.com/lucaoshiro/GridGooseSV/actions)

Native implementation of IEC 61850 protocols (GOOSE and SV) for the
[ns-3](https://www.nsnam.org/) network simulator, based on
[libiec61850](https://libiec61850.com/).

This module enables realistic simulation of Smart Grid communications for
performance and security analysis. It is described in the paper: **"GridGooseSV:
um Módulo do NS-3 para Simular Protocolos de Comunicação de Smart Grids
definidos na IEC 61850"** by Lucas Seiki Oshiro, Daniel Macêdo Batista, and
Natalia Castro Fernandes (in Portuguese).

## Repository Structure

The repository following the [ns-3 module structure](https://www.nsnam.org/docs/release/3.47/manual/html/new-modules.html):

- `model/`: Core implementation of the IEC 61850 protocols (based on `libiec61850`) and application classes
- `helper/`: ns-3 helper classes to simplify the setup of publishers and subscribers.
- `examples/`: Example scripts demonstrating various simulation scenarios and load tests.
- `test/`: Suite of unit and integration tests.
- `docker/`: Dockerfile and related files for containerized execution.
- `util/`: Common utilities for network address and device management.
- `doc/`: Additional documentation and resources.

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

### Supported plaftorms

- [x] Linux
- [x] macOS
- [ ] Windows

## Using pre-built Docker image

If you only want to see GridGooseSV in action, this is the way to go!
We provide a pre-built Docker image at
[Docker Hub](https://hub.docker.com/repository/docker/lucasoshiro/gridgoosesv/),
which you can run this way:

~~~bash
docker run -it lucasoshiro/gridgoosesv:latest
~~~

## Manual installation steps

Follow the steps here if you want to build ns-3 with GridGooseSV by yourself.
This is especially useful if you want to incorporate GridGooseSV to your
environment.

### Dependencies

We recommend using `ns-3 3.46`, but probably it will work with any `ns-3`
versions based on CMake (i.e. >= 3.36).

Be sure that you have the `ns-3` dependencies satisfied in the correct versions,
such as Python, GCC, Make and CMake.
Check them [here](https://www.nsnam.org/docs/installation/singlehtml/index.html#prerequisites).

Currently, only **Linux** and **Mac** are supported.

### Installation steps

1. Download `ns-3`, following the [official documentation](https://www.nsnam.org/docs/installation/html/index.html).
   Note that it's not required to have `bake` installed;
2. Clone this repository inside the `contrib` directory in ns-3;
3. Configure `ns-3` using the `optimized` profile, enabling tests and examples;
4. Build `ns-3` with GridGooseSV.

> Note: some warnings are expected. Because of them, only the `optimized`
> build profile is supported. They don't affect the working of this module,
> but, of course, they need to be solved. By now we haven't had enough
> time to fix them, but contributions are welcome!

The following script performs all those steps:

~~~bash
# Step 1
git clone https://gitlab.com/nsnam/ns-3-dev.git -b ns-3.44
cd ns-3-dev

# Step 2
git clone https://github.com/lucasoshiro/GridGooseSV.git contrib/GridGooseSV/

# Step 3
./ns3 configure --build-profile=optimized --enable-examples --enable-tests

# Step 4
./ns3 build
~~~

After running all those steps, you'll the executable examples in
`build/contrib/GridGooseSV/examples`.

### Building Docker image

If you want to build the Docker image, use the Dockerfile provided in the
`docker` directory:

~~~bash
cd docker
docker build -t gridgoosesv .
~~~

## Testing

We provided some test to assert that everything is working as expected. You can
run them by calling:

~~~bash
./test.py --suite="gridgoosesv"
~~~

in `ns-3` top-level directory.

A successful run indicates that the core components of GridGooseSV are properly
integrated.

## Examples and experiments

The examples are located in `examples` folder. They correspond to the
experiments described in the paper. After building ns-3 with GridGooseSV you'll
find their executables in `build/contrib/GridGooseSV/examples`.

Their names will be in the format `ns<ns-3 version>-gridgoosesv-<experiment name>-optimized`.

### GOOSE and SV Communication

Two of the mentioned example scenarios (named `simple_goose` and `simple_sv`)
demonstrate that `GridGooseSV` is able to generate GOOSE and SV traffic. In
order to run them, you'll only need to call in the terminal:

~~~bash
# simple_sv
./ns<ns-3 version>-gridgoosesv-simple_sv-optimized

# simple_goose
./ns<ns-3 version>-gridgoosesv-simple_goose-optimized
~~~

`simple_sv` sends SV messages from a publisher to a subscriber. Those messages
carry current measurements of a 3-phase alternate current power network. It
outputs the measurements received by the subscriber following a
semicolon-separated format, composed by the timestamp of the sample and current
measurements of each of the three phases.

`simple_goose` sends GOOSE periodic messages from a publisher to a subscriber.
It also simulates a critical event, where the interval between the messages
drops and increases following a geometric progression. It outputs the received
time of each message.

### SV load test

`sv_load_test` demonstrates the GridGooseSV capabilites of simulating several
SV publishers at the same time. It runs independent simulations, each one
doubling the number of publishers and measuring their elapsed time to simulate 1
second of SV traffic. Its output follows a semicolon-based format where each
line represent a different simulation, having as its fields its number of
publishers and its elapsed time.

### SV realtime

`sv_realtime` demonstrates the GridGooseSV capabilites in the `ns-3` realtime
scheduler, which can be used for emulation or hardware-in-the-loop
simulations. It performs several simulations with different numbers of
publishers generating SV traffic. The messages send and release timestamps
are logged into the output, which can be used to evaluate the impact of the
computational resources in message delays.

## Using

### Low-level API

The GOOSE and SV features from `libiec61850` are exposed as a low-level API
through these headers:

- `ns3/sv_publisher.h` ([Documentation]())
- `ns3/sv_subscriber.h` ([Documentation]())
- `ns3/goose_publisher.h` ([Documentation]())
- `ns3/goose_receiverr.h` ([Documentation]())

They are described in `libiec61850`'s documentation:

- [GOOSE](https://support.mz-automation.de/doc/libiec61850/c/latest/dir_9635cd859d6833098d90e9f407133914.html)
- [SV](https://support.mz-automation.de/doc/libiec61850/c/latest/dir_5e7a12e67607ea5cdbf2e5dd855be1e2.html)

You can also see the source code of the applications we provide as an example of
the use of those APIs.

> **IMPORTANT**: the `libiec61850` API is wrapped in the `libiec61850` namespace.
> This way, you should use `using namespace libiec61850` or prepend
> `libiec61850::` to each of the exposed types and functions!

### `ns-3` Applications

We provide four ready-to-use applications:

- SV Publisher
- SV Subscriber
- GOOSE Publisher
- GOOSE Receiver

You can also take a look at the examples to see how those applications can be
used.

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

## Security Concerns

The only risk that this software offers is that it can generate large files,
especially when generating `.pcap` files. Users who prefer an isolated
environment, can benefit from our pre-built Docker image or build a Docker
image by their own using our Dockerfile.

## Publication

GridGooseSV was developed under the master's research of
[Lucas Oshiro](https://github.com/lucasoshiro). It was described in the paper entitled
"GridGooseSV: um Módulo do NS-3 para Simular Protocolos de Comunicação de Smart
Grids definidos na IEC 61850", authored by Lucas Seiki Oshiro, Daniel Macêdo
Batista and Natalia Castro Fernandes, currently (Apr 2026) under submission.

## Considered Badges (SBRC)

The badges considered for this artifact evaluation are:

- SeloD
- SeloF
- SeloS
- SeloR

## License

This project is licensed under the terms of the license found in the `COPYING`
file.

## Funding

This software is one of the results of the project STARLING: Security and
Resource Allocation on B5G via Artificial Intelligence Techniques, funded by
FAPESP (proc. 21/06995-0). The development was also carried out with funding
from FAPESP proc. 2024/09448-9.
