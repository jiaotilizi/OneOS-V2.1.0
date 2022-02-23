# Baidu IoT 序列化C函数库

该文件夹包含如下组件 
* 一个数据序列化、反序列化函数库
* 一些例子来说明如何使用该序列化/反序列化库。

## 特性
* 该库可以序列化你要发给云端的数据，同时反序列化从云端接收到的数据。
* 用户只需要简单地（通过“宏”）为设备定义一个 "model" 即可。
* 可以方便地实现Baidu IoT设备孪生。

## 使用C序列化函数库

Baidu IoT 设备端SDK使用了该C序列化函数库，关于如何在不同多平台（例如Linux, mbed, Windows等）使用该SDK可以参考[这里][device-sdk]。

## 示例

该工程中包含了很多简单的例子来指导您如何和使用该库。详细请参考[这里][samples]。


[samples]: ./samples/
[device-sdk]: ../iothub_client/