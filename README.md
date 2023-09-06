# ESP-IDF Queue example

This example shows how you use a queues in ESP-IDF.  In this example we create a single queue through which we pass a `message_t` struct.  We also create a `consumer_task` and a `producer_task` to demonstrate how you can use tasks.

## Prerequisites

This project uses [Platformio](https://platformio.org/install), so make sure you have it installed.

## Building and running

To build this project just issue the command:

```shell
pio run
```

In order to build the project, upload it to the ESP32 and start the serial monitor, you can use the command:

```shell
pio run -t upload -t monitor
```

*If the platformio tool cannot find the correct serial device you may have to modify the
`platformio.ini` file* or provide a serial device on the command line with `--upload-port` and `--upload-port`*.
