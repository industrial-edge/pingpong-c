# Databus Ping Pong implementation in C





- [Databus Ping Pong implementation in C](#databus-ping-pong-implementation-in-c)
  - [Description](#description)
    - [Overview](#overview)
  - [Installation](#installation)
  - [Usage](#usage)
  - [Documentation](#documentation)
  - [Contribution](#contribution)
  - [Licence and Legal Information](#licence-and-legal-information)

## Description
This repository contains the source files to build the Databus Ping Pong application example implemented in C. Please refer to the [main repository](https://github.com/industrial-edge/databus-ping-pong) for more information about this application example.

### Overview

This implementation of the Databus Ping Pong application example uses a multi-stage process for building the docker image to keep the image size as small as possible. The two ``FROM`` Statements in the [Dockerfile](src/Dockerfile) separate the build process into two stages.
This example uses th [Eclipse Paho MQTT C library](https://github.com/eclipse/paho.mqtt.c) to enable it to connect to the IE Databus. 
The fist container is compiling the source code of the application and the paho library which then get copied to the second container which will be the final image for the application. Please refer to the the [docker documentation](https://docs.docker.com/develop/develop-images/multistage-build/) for more information regarding multi-stage builds.



## Installation

Please refer to the [Installation](https://github.com/industrial-edge/databus-ping-pong/blob/main/docs/Installation.md) section of the main repository on how to build and deploy the application to a Industrial Edge Device.
When creating a configuration for the application in the Industrial Edge Management System, you can use the the [mqtt-config.json](cfg-data/mqtt-config.json) file from this repository.

## Usage

Please refer to the [Testing](https://github.com/industrial-edge/databus-ping-pong/blob/main/docs/Installation.md#testing-the-application-using-simatic-flow-creator) section of the main repository on how to build and deploy the application to a Industrial Edge Device.

## Documentation

  
You can find further documentation and help in the following links
  - [Industrial Edge Hub](https://iehub.eu1.edge.siemens.cloud/#/documentation)
  - [Industrial Edge Forum](https://www.siemens.com/industrial-edge-forum)
  - [Industrial Edge landing page](https://new.siemens.com/global/en/products/automation/topic-areas/industrial-edge/simatic-edge.html)
  - [Industrial Edge GitHub page](https://github.com/industrial-edge)
  
## Contribution

Thanks for your interest in contributing. Anybody is free to report bugs, unclear documentation, and other problems regarding this repository in the Issues section or, even better, is free to propose any changes to this repository using Merge Requests.

## Licence and Legal Information

Please read the [Legal information](LICENSE.md).