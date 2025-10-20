package com.parissakalaee.sensorIoT.data

class SensorRepository {

    fun getSensors() = listOf(
        SensorReading("Temp_XD", "Temperature", 3.4, "°C"),
        SensorReading("Humid_FR", "Humidity", 60.0, "%"),
        SensorReading("Pres_uY", "Pressure", 15.0, "mbar")
    )
}