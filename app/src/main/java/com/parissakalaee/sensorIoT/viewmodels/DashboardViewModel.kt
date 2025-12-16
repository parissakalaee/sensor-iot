package com.parissakalaee.sensorIoT.viewmodels

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.parissakalaee.sensorIoT.data.SensorReading
import com.parissakalaee.sensorIoT.data.SensorRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.update
import kotlinx.coroutines.launch
import javax.inject.Inject

@HiltViewModel
class DashboardViewModel @Inject constructor(
    private val repository: SensorRepository
) : ViewModel() {

    private val _sensors = MutableStateFlow(repository.getSensors())
    val sensors: StateFlow<List<SensorReading>> = _sensors

    init {
        println("ViewModel created: ${this.hashCode()}")
        startCollectingMqttData()
    }

    override fun onCleared() {
        super.onCleared()
        println("ViewModel cleared: ${this.hashCode()}")
    }

    private fun startCollectingMqttData() {
        viewModelScope.launch {
            repository.observeLiveData().collect { sensorReading ->
                updateSensor(sensorReading.id, sensorReading.value)
            }
        }
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