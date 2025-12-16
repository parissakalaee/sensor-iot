package com.parissakalaee.sensorIoT.data

import kotlinx.coroutines.flow.Flow
import javax.inject.Inject
import javax.inject.Singleton

@Singleton
class SensorRepository @Inject constructor(
    private val mqttRepository: MqttRepository
) {
    fun getSensors() = listOf(
        SensorReading("Temp_XD", "Temperature", 0.0, "°C"),
        SensorReading("Humid_FR", "Humidity", 0.0, "%"),
        SensorReading("Pres_uY", "Pressure", 0.0, "mbar")
    )

    fun observeLiveData(): Flow<SensorReading> {
        return mqttRepository.observeSensorData()
    }
}