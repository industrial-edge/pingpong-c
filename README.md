# Databus Ping Pong C

This application example contains the source files to build a Databus Ping Pong application implemented in C.

- [Databus Ping Pong C](#databus-ping-pong-c)
  - [Description](#description)
    - [Overview](#overview)
    - [General task](#general-task)
  - [Requirements](#requirements)
    - [Prerequisites](#prerequisites)
    - [Used components](#used-components)
  - [Installation](#installation)
  - [Usage](#usage)
  - [Documentation](#documentation)
  - [Contribution](#contribution)
  - [License and Legal Information](#license-and-legal-information)
  - [Disclaimer](#disclaimer)

## Description

### Overview

This application example shows how to connect to the Databus via MQTT and how to publish and subscribe data using an implementation in C.
The Flow Creator is used to exchange data between different topics within the Databus.

![Use Case](/docs/graphics/Overview.png)

This implementation example uses the ``ubuntu:20.04`` image as base, the [Eclipse Paho MQTT C library](https://github.com/eclipse/paho.mqtt.c) to enable the connection to the Databus and ``cmake`` for building the code.

Here a multi-stage process for building the docker image is used to keep the image size as small as possible. The two ``FROM`` Statements in the [Dockerfile](src/Dockerfile) separate the build process into two stages.
The fist one is compiling the source code to an executable which then gets copied to the second stage. This stage finally creates the application container, where the executable runs. Please refer to the the [docker documentation](https://docs.docker.com/develop/develop-images/multistage-build/) for more information regarding multi-stage builds.

This example also shows two ways of configuring the application:

- configuration via file upload (fixed configuration file)
- configuration via system app Configuration Service (custom configuration UI with JSON Forms)

### General task

The application includes a MQTT client to subscribe to one topic of the Databus and waits to receive data. When data arrives, it publishes a corresponding answer to a second topic of the Databus. If it receives the string "Ping", it will answer with "Pong" and the other way around.

![Use Case](/docs/graphics/PingPongFlow.png)

The names of the Databus topics as well as the credentials used by the application can be configured via different options, otherwise environmental variables included in the docker-compose file are used.

## Requirements

###  Prerequisites

- Access to an Industrial Edge Management (IEM) with onboarded Industrial Edge Device (IED)
- IEM: Installed apps: Databus Configurator, IE App Configuration Service
- IED: Installed apps: Databus, Flow Creator

### Used components

- Industrial Edge Management V1.5.2-4 / V1.12.10
  - IE App Configuration Service V1.2.2
  - Databus Configurator V2.1.0-3
- Industrial Edge Device (IED) V1.9.0-27  
  - Databus V2.1.0-4
  - Flow Creator V1.12.0-1
- Industrial Edge App Publisher V1.10.5
- Docker Engine V20.10.10
- Docker Compose V1.29.2
- Web browser (Chrome)

## Installation

Please refer to the [Installation](https://github.com/industrial-edge/pingpong-csharp/blob/main/docs/Installation.md#installation) section of the *Ping Pong in C#* repository on how to build and deploy the application to an IED. Just use the source files from this repository and follow the instructions:

- [Build application](https://github.com/industrial-edge/pingpong-csharp/blob/main/docs/Installation.md#build-application)
- [Configuring the Industrial Edge Databus](https://github.com/industrial-edge/pingpong-csharp/blob/main/docs/Installation.md#configuring-the-industrial-edge-databus)
- [Create configuration for the application](https://github.com/industrial-edge/pingpong-csharp/blob/main/docs/Installation.md#create-configuration-for-the-application)
- [Upload the application to the Industrial Edge Management](https://github.com/industrial-edge/pingpong-csharp/blob/main/docs/Installation.md#create-configuration-for-the-application)
- [Configuring and deploying the application to a Industrial Edge Device](https://github.com/industrial-edge/pingpong-csharp/blob/main/docs/Installation.md#configuring-and-deploying-the-application-to-a-industrial-edge-device)

## Usage

Once the application is successfully deployed to the IED, it can be tested using the Flow Creator.

On the IED restart the PingPong application, to ensure the right configuration is used. Then open the app Flow Creator and set it up as following:

- Connect an "inject" node with a "mqtt out" node
- Connect a "mqtt in" node with a "debug" node
- Configure the mqtt-nodes to connect to the databus (mqtt broker, username, password)
- Set the topics of the mqtt-nodes according to the configuration of the application (here: "topic1" to publish to, "topic2" to subscribe to)

Deploy the flow and test by injecting a string payload into the mqtt in node. If the string is "Ping", the application will answer with "Pong". If the string is "Pong" the application will answer with "Ping".

The finished flow is available [here](/src/Flow_Pingpong_Test.json) and can be imported into the Flow Creator.

![Flow Creator](/docs/graphics/FlowCreator.png)

## Documentation
 
- You can find further documentation and help in the following links
  - [Industrial Edge Hub](https://iehub.eu1.edge.siemens.cloud/#/documentation)
  - [Industrial Edge Forum](https://forum.mendix.com/link/space/industrial-edge)
  - [Industrial Edge landing page](https://new.siemens.com/global/en/products/automation/topic-areas/industrial-edge/simatic-edge.html)
  - [Industrial Edge GitHub page](https://github.com/industrial-edge)
  - [Industrial Edge documentation page](https://docs.eu1.edge.siemens.cloud/index.html)
  
## Contribution

Thank you for your interest in contributing. Anybody is free to report bugs, unclear documentation, and other problems regarding this repository in the Issues section.
Additionally everybody is free to propose any changes to this repository using Pull Requests.

If you are interested in contributing via Pull Request, please check the [Contribution License Agreement](Siemens_CLA_1.1.pdf) and forward a signed copy to [industrialedge.industry@siemens.com](mailto:industrialedge.industry@siemens.com?subject=CLA%20Agreement%20Industrial-Edge).

## License and Legal Information

Please read the [Legal information](LICENSE.txt).

## Disclaimer

IMPORTANT - PLEASE READ CAREFULLY:

This documentation describes how you can download and set up containers which consist of or contain third-party software. By following this documentation you agree that using such third-party software is done at your own discretion and risk. No advice or information, whether oral or written, obtained by you from us or from this documentation shall create any warranty for the third-party software. Additionally, by following these descriptions or using the contents of this documentation, you agree that you are responsible for complying with all third party licenses applicable to such third-party software. All product names, logos, and brands are property of their respective owners. All third-party company, product and service names used in this documentation are for identification purposes only. Use of these names, logos, and brands does not imply endorsement.
