package com.parissakalaee.sensorIoT.viewmodels

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.parissakalaee.sensorIoT.data.SensorReading
import com.parissakalaee.sensorIoT.data.SensorRepository
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.update
import kotlinx.coroutines.launch
import kotlin.random.Random

class DashboardViewModel : ViewModel() {

    private val repository = SensorRepository()
    private val _sensors = MutableStateFlow(repository.getSensors())
    val sensors: StateFlow<List<SensorReading>> = _sensors

    init {
        println("ViewModel created: ${this.hashCode()}")
        startSimulatingValues()
    }

    override fun onCleared() {
        super.onCleared()
        println("ViewModel cleared: ${this.hashCode()}")
    }

    private fun startSimulatingValues() {
        viewModelScope.launch {
            while (true) {
                delay(1000)
                val temp = readTemperatureSensor()
                updateSensor("Temp_XD", temp)
            }
        }

        viewModelScope.launch {
            while (true) {
                delay(2000)
                val humidity = readHumiditySensor()
                updateSensor("Humid_FR", humidity)
            }
        }

        viewModelScope.launch {
            while (true) {
                delay(100)
                val pressure = readPressureSensor()
                updateSensor("Pres_uY", pressure)
            }
        }
    }

    fun readTemperatureSensor(): Double{
        return Random.nextDouble(15.0, 30.0)
    }

    fun readHumiditySensor(): Double{
        return Random.nextDouble(30.0, 80.0)
    }

    fun readPressureSensor(): Double{
        return Random.nextDouble(980.0, 1020.0)
    }

    fun toggleSensorConnection(sensorId: String) {
        _sensors.update { sensors ->
            sensors.map { sensor ->
                if (sensor.id == sensorId) {
                    sensor.copy(isConnected = !sensor.isConnected)
                } else {
                    sensor
                }
            }
        }
    }

    fun updateSensor(sensorId: String, newValue: Double) {
        _sensors.update { sensors ->
            sensors.map { sensor ->
                if (sensor.id == sensorId && sensor.isConnected) {
                    sensor.copy(
                        value = newValue,
                        timestamp = System.currentTimeMillis()
                    )
                } else {
                    sensor
                }
            }
        }
    }

}

