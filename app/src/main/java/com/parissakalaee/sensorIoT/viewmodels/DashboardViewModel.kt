package com.parissakalaee.sensorIoT.viewmodels

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.parissakalaee.sensorIoT.data.SensorReading
import com.parissakalaee.sensorIoT.data.SensorRepository
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch
import kotlin.random.Random

class DashboardViewModel : ViewModel() {

    init {
        startSimulatingValues()
    }

    private val repository = SensorRepository()
    private val _sensors = MutableStateFlow(repository.getSensors())
    val sensors: StateFlow<List<SensorReading>> = _sensors

    private fun startSimulatingValues() {
        viewModelScope.launch {
            while (true) {
                delay(2000)
                _sensors.value = _sensors.value.map { sensor ->
                    if(sensor.isConnected) {
                        when (sensor.id) {
                            "Temp_XD" -> {
                                sensor.copy(
                                    value = Random.nextDouble(15.0, 30.0),
                                    timestamp = System.currentTimeMillis()
                                )
                            }

                            "Humid_FR" -> {
                                sensor.copy(
                                    value = Random.nextDouble(30.0, 80.0),
                                    timestamp = System.currentTimeMillis()
                                )
                            }

                            "Pres_uY" -> {
                                sensor.copy(
                                    value = Random.nextDouble(980.0, 1020.0),
                                    timestamp = System.currentTimeMillis()
                                )
                            }

                            else -> sensor
                        }
                    } else {
                        sensor
                    }
                }
            }
        }
    }

    fun toggleSensorConnection(sensorId: String) {
        _sensors.value = _sensors.value.map { sensor ->
            if (sensor.id == sensorId) {
                sensor.copy(isConnected = !sensor.isConnected)
            } else {
                sensor
            }
        }
    }
}

