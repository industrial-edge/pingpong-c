# Databus Ping Pong C

This application example contains the source files to build a Databus Ping Pong application implemented in C.

- [Databus Ping Pong C](#databus-ping-pong-c)
  - [Description](#description)
    - [Overview](#overview)
    - [General task](#general-task)
  - [Requirements](#requirements)
    - [Used components](#used-components)
    - [Further requirements](#further-requirements)
  - [Installation](#installation)
  - [Usage](#usage)
  - [Documentation](#documentation)
  - [Contribution](#contribution)
  - [Licence and Legal Information](#licence-and-legal-information)

## Description

### Overview

This application example shows how to connect to the IE Databus via MQTT and how to publish and subscribe data using an implementation in C.
The IE Flow Creator is used to exchange data between different topics within the IE Databus.

![Use Case](/docs/DataFlow.png)

This implementation example uses the ``ubuntu:20.04`` image as base, the [Eclipse Paho MQTT C library](https://github.com/eclipse/paho.mqtt.c) to enable the connection to the IE Databus and ``cmake`` for building the code.
Here a multi-stage process for building the docker image is used to keep the image size as small as possible. The two ``FROM`` Statements in the [Dockerfile](src/Dockerfile) separate the build process into two stages.
The fist one is compiling the source code to an executable which then gets copied to the second stage. This stage finally creates the application container, where the executable runs. Please refer to the the [docker documentation](https://docs.docker.com/develop/develop-images/multistage-build/) for more information regarding multi-stage builds.

This example also shows two ways of configuring the application:

- configuration via file upload (fix configuration file)
- configuration via system app Configuration Service (custom configuration UI with JSON Forms)

### General task

The application includes a MQTT client to subscribe to one topic of the IE Databus and waits to receive data. When data arrives, it publishes a corresponding answer to a second topic of the IE Databus. If it receives the string "Ping", it will answer with "Pong" and the other way around.

![Use Case](/docs/Usecase.png)

The names of the IE Databus topics as well as the credentials used by the application can be configured via different options, otherwise environmental variables included in the docker-compose file are used.

## Requirements

### Used components

- Industrial Edge Management V1.3.0-58 / V1.3.10
  - IE Databus Configurator V1.3.5
  - IE Databus V1.3.5
  - IE Flow Creator V1.1.2
  - IE App Configuration Service V1.0.7
- Industrial Edge Device (IED) V1.2.0-56
- Industrial Edge App Publisher V1.3.7
- Docker Engine V20.10.8
- Docker Compose V1.28.5

### Further requirements

- IED is onboarded to a IEM
- IE Databus Configurator is deployed to the IEM
- IE Configuration Service is deployed to the IEM
- IE Databus is deployed to the IED
- IE Flow Creator is deployed to the IED

## Installation

Please refer to the [Installation](https://github.com/industrial-edge/pingpong-csharp/blob/main/docs/Installation.md#installation) section of the *Ping Pong in C#* repository on how to build and deploy the application to an IED. Just use the source files from this repository and follow the instructions.

- [Build application](https://github.com/industrial-edge/pingpong-csharp/blob/main/docs/Installation.md#build-application)
- [Configuring the Industrial Edge Databus](https://github.com/industrial-edge/pingpong-csharp/blob/main/docs/Installation.md#configuring-the-industrial-edge-databus)
- [Create configuration for the application](https://github.com/industrial-edge/pingpong-csharp/blob/main/docs/Installation.md#create-configuration-for-the-application)
- [Upload the application to the Industrial Edge Management](https://github.com/industrial-edge/pingpong-csharp/blob/main/docs/Installation.md#create-configuration-for-the-application)
- [Configuring and deploying the application to a Industrial Edge Device](https://github.com/industrial-edge/pingpong-csharp/blob/main/docs/Installation.md#configuring-and-deploying-the-application-to-a-industrial-edge-device)
- [Testing the application using Simatic Flow Creator](https://github.com/industrial-edge/pingpong-csharp/blob/main/docs/Installation.md#testing-the-application-using-simatic-flow-creator)

## Usage

Once the application is successfully deployed to the IED, it can be tested using the IE Flow Creator.

On the IED restart the PingPong application, to ensure the right configuration is used. Then open the app IE Flow Creator and set it up as following:

- Connect an "inject" node with a "mqtt out" node
- Connect a "mqtt in" node with a "debug" node
- Configure the mqtt-nodes to connect to the databus (mqtt broker, username, password)
- Set the topics of the mqtt-nodes according to the configuration of the application (here: "topic1" to publish to, "topic2" to subscribe to)

Deploy the flow and test by injecting a string payload into the mqtt in node. If the string is "Ping", the application will answer with "Pong". If the string is "Pong" the application will answer with "Ping".

The finished flow is available [here](/src/Flow_Pingpong_Test.json) and can be imported into the IE Flow Creator.

![Flow Creator](/docs/FlowCreator.png)

## Documentation

You can find further documentation and help in the following links:

- [Industrial Edge Hub](https://iehub.eu1.edge.siemens.cloud/#/documentation)
- [Industrial Edge Forum](https://www.siemens.com/industrial-edge-forum)
- [Industrial Edge landing page](https://new.siemens.com/global/en/products/automation/topic-areas/industrial-edge/simatic-edge.html)
- [Industrial Edge GitHub page](https://github.com/industrial-edge)

## Contribution

Thanks for your interest in contributing. Anybody is free to report bugs, unclear documentation, and other problems regarding this repository in the Issues section or, even better, is free to propose any changes to this repository using Merge Requests.

## Licence and Legal Information

Please read the [Legal information](LICENSE.md).
